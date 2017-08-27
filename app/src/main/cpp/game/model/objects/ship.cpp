#include "ship.h"

using namespace model;

namespace {
float collisionRadius = 30;
} // namespace

Ship::Ship(const Point& point):
        Object(point, ::collisionRadius, Vector(0.0, 1.0)),
        _accelerationVector(0.0, 0.0),
        _directionVector(0.0, 1.0) {

    _baseBorder.push_back(Point(0.0, 0.0)); // center
    _baseBorder.push_back(Point(4.0, -4.0));
    _baseBorder.push_back(Point(8.0, 0.0));
    _baseBorder.push_back(Point(8.0, 12.0));
    _baseBorder.push_back(Point(4.0, 28.0));
    _baseBorder.push_back(Point(8.0, 28.0));
    _baseBorder.push_back(Point(20.0, 4.0));
    _baseBorder.push_back(Point(20.0, -16.0));
    _baseBorder.push_back(Point(8.0, -28.0));
    _baseBorder.push_back(Point(8.0, -20.0));
    _baseBorder.push_back(Point(0.0, -16.0)); // center

    auto it = _baseBorder.rbegin();
    ++it;
    int pointCount = _baseBorder.size() - 2;
    for (int i = 0; i < pointCount; ++i, ++it) {
        _baseBorder.push_back(Point(-(it->x), it->y));
    }

    for (const Point& eachPoint: _baseBorder) {
        _border.push_back(eachPoint + point);
    }
}

Ship::~Ship() {}

void Ship::move() {
    Vector oldSpeed = Vector(0, 1);
    _velocity = _velocity + _accelerationVector;

    _point.move(_velocity);

    if (_velocity.norma() == 0.0) return;

    _directionVector = _velocity.normaVector();

    float cosRotateAngle = (oldSpeed * _velocity) / (oldSpeed.norma() * _velocity.norma());
    float sinRotateAngle = sqrt(1.0 - cosRotateAngle * cosRotateAngle);
    if (_velocity.x > 0) sinRotateAngle = -sinRotateAngle;

    Points tmpPoints = _baseBorder;
    for (Point& point: tmpPoints) {
        Point tmp = point;
        point.x = tmp.x * cosRotateAngle + tmp.y * (-sinRotateAngle);
        point.y = tmp.x * sinRotateAngle + tmp.y * cosRotateAngle;
        point = point + this->point();
    }

    _border = tmpPoints;
}

Vector Ship::acceleration() const {
    return _accelerationVector;
}

void Ship::setAcceleration(const Vector& acceleration) {
    _accelerationVector = acceleration;
}

Vector Ship::direction() const {
    return _directionVector;
}
