#include "asteroid.h"

#include <cstdlib>
#include <cmath>

using namespace model;

namespace {
const float ro = 0.1;

const float minRadiusK = 0.60;
const float maxRadiusK = 1.0;
} // namespace

Asteroid::Asteroid(const Point& point, float radius,  const Vector& speed):
        Object(point, radius, speed) {
    _border = generateAsteroid(radius);
}

Asteroid::~Asteroid() {}

float Asteroid::mass() const {
    return 4.0 / 3.0 * M_PI * pow(this->collisionRadius(), 3.0) * ::ro;
}

Points Asteroid::generateAsteroid(float radius) const {
    Points border;

    float alfa = 0.0f;

    while (alfa < (M_PI * 2)) {
        float r = common::rangeRand(radius * ::minRadiusK, radius * ::maxRadiusK);

        border.push_back(Point(this->x() + cos(alfa) * r, this->y() + sin(alfa) * r));

        int alfaInt = rand() % 30 + 35;

        alfa += alfaInt / 180.0;
    }
    return border;
}
