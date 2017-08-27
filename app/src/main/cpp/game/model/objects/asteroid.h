#ifndef ASTEROID_H
#define ASTEROID_H

#include "object.h"

namespace model {

class Asteroid: public Object {
public:
    Asteroid(const Point& point, float radius, const Vector& velocity);
    ~Asteroid() override;

    float mass() const override;

private:
    Points generateAsteroid(float radius) const;
};

using AsteroidPtr = std::shared_ptr<Asteroid>;

} // namespace model

#endif // ASTEROID_H
