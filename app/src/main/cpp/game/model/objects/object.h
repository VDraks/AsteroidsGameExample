#ifndef OBJECT_H
#define OBJECT_H

#include "../model_traits.h"
#include <memory>

namespace model {

class Object {
public:
    Object(const Point& point, float collisionRadius, const Vector& velocity = Vector());
    virtual ~Object();

    Point point() const;
    float x() const;
    float y() const;
    void setPoint(const Point& point);
    void setPoint(float x, float y);

    Points border() const;
    Vector velocity() const;
    void setVelocity(const Vector& velocity);
    virtual float mass() const;

    void move(const Vector& vector);
    virtual void move();

    float collisionRadius() const;

protected:
    Point _point;
    Points _border;
    Vector _velocity;

private:
    float _collisionRadius;
};

using ObjectPtr = std::shared_ptr<Object>;

} // namespace model

#endif // OBJECT_H
