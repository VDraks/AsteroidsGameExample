#include "collision_handler.h"

#include <cmath>

using namespace collision;

namespace {

float area(const model::Point& a, const model::Point& b, const model::Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool isIntersected(float a, float b, float c, float d) {
    if (a > b) std::swap(a, b);
    if (c > d) std::swap(c, d);
    return std::max(a,c) <= std::min(b,d);
}

// check intersection segments AB and CD
bool isIntersected(const model::Point& a, const model::Point& b,
                   const model::Point& c, const model::Point& d) {
    return isIntersected(a.x, b.x, c.x, d.x)
           && isIntersected(a.y, b.y, c.y, d.y)
           && area(a, b, c) * area(a, b, d) <= 0
           && area(c, d, a) * area(c, d, b) <= 0;
}

bool isIntersected(const model::ObjectPtr& obj1, const model::ObjectPtr& obj2) {
    float d = model::distance(obj1->point(), obj2->point());
    if (d > (obj1->collisionRadius() + obj2->collisionRadius())) return false;

    model::Point a = obj1->border().back();
    for (const model::Point& b : obj1->border()) {
        model::Point c = obj2->border().back();
        for (const model::Point& d : obj2->border()) {
            if (isIntersected(a, b, c, d)) return true;
            c = d;
        }
        a = b;
    }
    return false;
}

bool isIntersected(const model::ObjectPtr& obj, const model::Point& point) {
    float d = model::distance(obj->point(), point);
    if (d > obj->collisionRadius()) return false;

    model::Point a = obj->border().back();
    for (const model::Point& b : obj->border()) {
        if (isIntersected(a, b, point, obj->point())) return false;
        a = b;
    }
    return true;
}

const float asteroidDestroyMomentum = 100000.0;

} // namespace

CollisionHandler::CollisionHandler(model::ModelHandler* const modelHandler):
        _modelHandler(modelHandler) {

}

void CollisionHandler::update(double /*deltaTime*/) {

    auto asteroids = _modelHandler->asteroids();
    for (model::AsteroidPtr& asteroid: asteroids) {
        if (::isIntersected(asteroid, _modelHandler->ship())) {
            _modelHandler->removeShip();
        }

        for (const model::ProjectilePtr& projectile: _modelHandler->projectiles()) {
            if (::isIntersected(asteroid, projectile->point())) {
                _modelHandler->processHit(projectile, asteroid);
                break;
            }
        }
    }

    for (auto firstIt = asteroids.begin(); firstIt != asteroids.end(); ++firstIt) {
        auto secondIt = firstIt;
        ++secondIt;
        for (; secondIt != asteroids.end(); ++secondIt) {
            if (!::isIntersected(*firstIt, *secondIt)) continue;

            this->processAsteroidsCollision(*firstIt, *secondIt);
        }
    }
}

void CollisionHandler::processAsteroidsCollision(const model::AsteroidPtr& obj1,
                                                 const model::AsteroidPtr& obj2) {
    const model::Vector ox = model::Vector(obj1->x() - obj2->x(),
                                           obj1->y() - obj2->y()).normaVector();
    const model::Vector oy = model::Vector(-ox.y, ox.x).normaVector();

    const model::Vector vx1 = ox * (ox * obj1->velocity());
    const model::Vector vx2 = ox * (ox * obj2->velocity());

    const model::Vector vy1 = oy * (oy * obj1->velocity());
    const model::Vector vy2 = oy * (oy * obj2->velocity());

    const float m1 = obj1->mass();
    const float m2 = obj2->mass();

    const model::Vector vecP1 = vx1 * m1 + vx2 * m2;
    const float P1 = vecP1.norma() * (((ox * vecP1) > 0) ? 1.0 : -1.0);
    const float E1 = (vx1 * vx1) * (m1 / 2.0) + (vx2 * vx2) * (m2 / 2.0);

    const float sqrtD4 = sqrt(std::abs(m1 * m2 * (2.0 * E1 * (m1 + m2) - (P1 * P1))));

    model::Vector newVx1 = ox * (m1 * P1 + sqrtD4) / (m1 * (m2 + m1));
    model::Vector newVx2 = ox * (m2 * P1 - sqrtD4) / (m2 * (m2 + m1));

    if ((newVx1 - vx1).norma() < 0.0001) {
        newVx1 = ox * (m1 * P1 - sqrtD4) / (m1 * (m2 + m1));
        newVx2 = ox * (m2 * P1 + sqrtD4) / (m2 * (m2 + m1));
    }

    obj1->setVelocity(newVx1 + vy1);
    obj2->setVelocity(newVx2 + vy2);

    while (isIntersected(obj1, obj2)) {
        obj1->move();
        obj2->move();
    }

    this->checkDestroyAsteroid(obj1, vx1, newVx1);
    this->checkDestroyAsteroid(obj2, vx2, newVx2);
}

bool CollisionHandler::checkDestroyAsteroid(const model::AsteroidPtr& asteroid,
                                            const model::Vector& oldVx, const model::Vector& newVx) {

    if (((newVx - oldVx) * asteroid->mass()).norma() < ::asteroidDestroyMomentum) return false;

    _modelHandler->removeAsteroid(asteroid);
    return true;
}
