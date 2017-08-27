#include "model_handler.h"

#include <ctime>
#include <cmath>
#include <cstdlib>

using namespace model;

namespace {
const double tickTime = 40.00;

const unsigned int asteroidNumber = 8;
const float projectileSpeed = 10.0;

const float smallAsteroidRadiusK =  1.5;
const float minLargeAsteroidRadius = 35.0;
const float maxLargeAsteroidRadius = minLargeAsteroidRadius * smallAsteroidRadiusK - 1.0;
const float minAsteroidSpeed = 1.5;
const float maxAsteroidSpeed = 6.5;

const float explosionK = 50000.0;
} // namespace

ModelHandler::ModelHandler(float worldWidth, float worldHeight):
        _isGameOver(false),
        _worldWidth(worldWidth),
        _worldHeight(worldHeight),
        _tickTime(0),
        _ship(std::make_shared<Ship>(Point(worldWidth / 2.0, worldHeight / 2.0))) {

    srand(time(0));
}

void ModelHandler::newGame() {
    _asteroids.clear();
    _projectiles.clear();
    _ship = std::make_shared<Ship>(Point(_worldWidth / 2.0, _worldHeight / 2.0));
    _isGameOver = false;
}

bool ModelHandler::isGameOver() const {
    return _isGameOver;
}

void ModelHandler::update(double deltaTime) {
    if (this->isGameOver()) return;

    _tickTime += deltaTime;
    while (_tickTime > tickTime) {

        if (_asteroids.size() < ::asteroidNumber) {
            this->addAsteroid();
        }

        for (ObjectPtr& obj: this->allObjects()) {
            obj->move();
        }

        this->checkObjects(&_asteroids);
        this->checkObjects(&_projectiles);

        if (!this->withinBoundaries(_ship)) {
            this->removeShip();
        }

        _tickTime -= tickTime;
    }
}

ShipPtr ModelHandler::ship() const {
    return _ship;
}

void ModelHandler::removeShip() {
    _isGameOver = true;
}

void ModelHandler::removeAsteroid(const AsteroidPtr& asteroid) {
    if (asteroid->collisionRadius() >= ::minLargeAsteroidRadius) {

        const model::Vector v1(asteroid->velocity()
                               + model::Vector(-asteroid->velocity().y, asteroid->velocity().x));
        const model::Vector v2(asteroid->velocity()
                               + model::Vector(asteroid->velocity().y, -asteroid->velocity().x));

        this->addAsteroid(asteroid, v1);
        this->addAsteroid(asteroid, v2);
    }
    _asteroids.remove(asteroid);
}

void ModelHandler::addAsteroid(const AsteroidPtr& asteroid) {
    _asteroids.push_back(asteroid);
}

std::list<AsteroidPtr> ModelHandler::asteroids() const {
    return _asteroids;
}

std::list<ProjectilePtr> ModelHandler::projectiles() const {
    return _projectiles;
}

void ModelHandler::removeProjectile(const ProjectilePtr& projectile) {
    _projectiles.remove(projectile);
}

void ModelHandler::addProjectile() {
    Vector projectileSpeed = _ship->direction() * ::projectileSpeed + ship()->velocity();
    _projectiles.push_back(std::make_shared<Projectile>(_ship->point(), projectileSpeed));
}

void ModelHandler::processHit(const ProjectilePtr& projectile, const AsteroidPtr& asteroid) {
    if (asteroid->collisionRadius() >= ::minLargeAsteroidRadius) {
        const Vector ox = asteroid->velocity().normaVector();
        Vector explosionVector(ox.y, -ox.x);

        if (ox * asteroid->velocity() < 0) {
            explosionVector = explosionVector * (-1.0);
        }

        const Vector v1(asteroid->velocity() + model::Vector(-asteroid->velocity().y, asteroid->velocity().x)
                        + explosionVector * (-::explosionK / asteroid->mass()));
        const Vector v2(asteroid->velocity() + model::Vector(asteroid->velocity().y, -asteroid->velocity().x)
                        + explosionVector * (::explosionK / asteroid->mass()));

        this->addAsteroid(asteroid, v1);
        this->addAsteroid(asteroid, v2);
    }
    _projectiles.remove(projectile);
    _asteroids.remove(asteroid);
}

std::list<ObjectPtr> ModelHandler::allObjects() const {
    std::list<ObjectPtr> objects;
    objects.insert(objects.end(), _asteroids.begin(), _asteroids.end());
    objects.insert(objects.end(), _projectiles.begin(), _projectiles.end());
    objects.push_back(_ship);
    return objects;
}

void ModelHandler::addAsteroid() {

    const float r = common::rangeRand(::minLargeAsteroidRadius, ::maxLargeAsteroidRadius);

    Point point;
    bool isCorrect = false;
    while (!isCorrect) {
        point = randPoint(r);
        isCorrect = true;
        for (const AsteroidPtr& asteroid : _asteroids) {
            const float d = model::distance(asteroid->point(), point);
            if (d < (asteroid->collisionRadius() + r)) {
                isCorrect = false;
                break;
            }
        }
    }

    const float distToCenter = ::distance(point.x, point.y, _worldWidth / 2.0, _worldHeight / 2.0);
    const float vx = (_worldWidth / 2.0 - point.x) / distToCenter
                     * common::rangeRand(::minAsteroidSpeed, ::maxAsteroidSpeed);
    const float vy = (_worldHeight / 2.0 - point.y) / distToCenter
                     * common::rangeRand(::minAsteroidSpeed, ::maxAsteroidSpeed);

    _asteroids.push_back(std::make_shared<Asteroid>(point, r, Vector(vx, vy)));
}

void ModelHandler::addAsteroid(const AsteroidPtr& oldAsteroid, const Vector& newVelocity) {
    Point point = oldAsteroid->point();
    point.move(newVelocity.normaVector() * oldAsteroid->collisionRadius());
    _asteroids.push_back(std::make_shared<Asteroid>(point, oldAsteroid->collisionRadius()
                                                           / ::smallAsteroidRadiusK, newVelocity));
}

Point ModelHandler::randPoint(float r) const {
    float x = 0;
    float y = 0;
    switch (rand() % 4) {
        case 0:
            x = common::rangeRand(0 - r, _worldWidth + r);
            y = 0 - r;
            break;
        case 1:
            x = common::rangeRand(0 - r, _worldWidth + r);
            y = _worldHeight - r;
            break;
        case 2:
            x = 0 - r;
            y = common::rangeRand(0 - r, _worldHeight + r);
            break;
        case 3:
            x = _worldWidth + r;
            y = common::rangeRand(0 - r, _worldHeight + r);
            break;
        default:
            break;
    }

    return Point(x, y);
}

bool ModelHandler::withinBoundaries(const ObjectPtr& obj) const {
    return (obj->x() > (0 - _worldWidth * 0.1) && obj->x() < (_worldWidth * 1.1)
            && obj->y() > (0 - _worldHeight * 0.1) && obj->y() < (_worldHeight * 1.1));
}

template<class T> void ModelHandler::checkObjects(std::list<T>* objects) {
    auto it = objects->begin();
    while (it != objects->end())     {
        if (!this->withinBoundaries(*it)) {
            it = objects->erase(it++);
        } else {
            ++it;
        }
    }
}
