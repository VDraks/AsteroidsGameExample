#include "projectile.h"

using namespace model;

namespace {
const float collisionRadius = 1;
const float projectileMass = 1000;
} // namespace

Projectile::Projectile(const Point& point, const Vector& speed):
        Object(point, ::collisionRadius, speed) {

    _border.push_back(Point(-1.0, -1.0) + point);
    _border.push_back(Point(-1.0, 1.0) + point);
    _border.push_back(Point(1.0, 1.0) + point);
    _border.push_back(Point(1.0, -1.0) + point);
}

Projectile::~Projectile() {}

float Projectile::mass() const {
    return ::projectileMass;
}
