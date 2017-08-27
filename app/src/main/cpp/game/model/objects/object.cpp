#include "object.h"

using namespace model;

Object::Object(const Point& point, float collisionRadius, const Vector& speed):
        _point(point),
        _velocity(speed),
        _collisionRadius(collisionRadius)
{}

Object::~Object()
{}

Point Object::point() const {
    return _point;
}

float Object::x() const {
    return _point.x;
}

float Object::y() const {
    return _point.y;
}

void Object::setPoint(const Point& point) {
    _point = point;
}

void Object::setPoint(float x, float y) {
    _point.x = x;
    _point.y = y;
}

Points Object::border() const {
    return _border;
}

Vector Object::velocity() const {
    return _velocity;
}

void Object::setVelocity(const Vector& speed) {
    _velocity = speed;
}

float Object::mass() const {
    return 1.0;
}

void Object::move(const Vector& vector) {
    _point.move(vector);

    for (Point& point: _border) {
        point.move(vector);
    }
}

void Object::move() {
    this->move(_velocity);
}

float Object::collisionRadius() const {
    return _collisionRadius;
}
