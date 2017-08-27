#ifndef SHIP_H
#define SHIP_H

#include "object.h"

namespace model {

class Ship: public Object {
public:
    Ship(const Point& point);
    ~Ship() override;

    void move() override;

    Vector acceleration() const;
    void setAcceleration(const Vector& acceleration);

    Vector direction() const;

private:
    Vector _accelerationVector;
    Vector _directionVector;
    Points _baseBorder;
};

using ShipPtr = std::shared_ptr<Ship>;

} // namespace model

#endif // SHIP_H
