#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "object.h"

namespace model {

class Projectile: public Object {
public:
    Projectile(const Point& point, const Vector& velocity);
    ~Projectile() override;

    float mass() const override;
};

using ProjectilePtr = std::shared_ptr<Projectile>;

} // namespace model

#endif // PROJECTILE_H
