#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include "../model/model_handler.h"
#include "../model/objects/asteroid.h"
#include "../model/objects/projectile.h"

namespace collision {

class CollisionHandler {
public:
    CollisionHandler(model::ModelHandler* const modelHandler);

    void update(double deltaTime);

private:
    CollisionHandler(const CollisionHandler&);
    CollisionHandler& operator=(const CollisionHandler&);

    void processAsteroidsCollision(const model::AsteroidPtr &obj1, const model::AsteroidPtr &obj2);
    bool checkDestroyAsteroid(const model::AsteroidPtr& asteroid,
                              const model::Vector& oldVx, const model::Vector& newVx);

    model::ModelHandler* const _modelHandler;
};

} // namespace collision

#endif // COLLISION_HANDLER_H
