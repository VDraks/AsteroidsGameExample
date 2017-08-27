#ifndef MODEL_HANDLER_H
#define MODEL_HANDLER_H

#include "objects/ship.h"
#include "objects/asteroid.h"
#include "objects/projectile.h"

namespace model {

class ModelHandler {
public:
    ModelHandler(float worldWidth, float worldHeight);

    void newGame();
    bool isGameOver() const;
    void update(double deltaTime);

    ShipPtr ship() const;
    void removeShip();

    std::list<AsteroidPtr> asteroids() const;
    void removeAsteroid(const AsteroidPtr& asteroid);
    void addAsteroid(const AsteroidPtr& asteroid);

    std::list<ProjectilePtr> projectiles() const;
    void removeProjectile(const ProjectilePtr& projectile);
    void addProjectile();

    void processHit(const ProjectilePtr& projectile, const AsteroidPtr& asteroid);

    std::list<ObjectPtr> allObjects() const;

private:
    ModelHandler(const ModelHandler&);
    ModelHandler& operator=(const ModelHandler&);

    void addAsteroid();
    void addAsteroid(const AsteroidPtr& oldAsteroid, const Vector& newVelocity);
    Point randPoint(float r) const;
    bool withinBoundaries(const ObjectPtr& obj) const;
    template<class T> void checkObjects(std::list<T>* objects);

    bool _isGameOver;
    float _worldWidth;
    float _worldHeight;
    double _tickTime;

    ShipPtr _ship;
    std::list<AsteroidPtr> _asteroids;
    std::list<ProjectilePtr> _projectiles;
};

} // namespace model

#endif // MODEL_HANDLER_H
