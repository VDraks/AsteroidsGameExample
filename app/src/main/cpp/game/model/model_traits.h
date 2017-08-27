#ifndef MODEL_TRAITS_H
#define MODEL_TRAITS_H

#include <vector>
#include <list>
#include <cmath>
#include <cstdlib>

#include "../common/common.h"

namespace model {

class Vector {
public:
    Vector(): x(0), y(0) {}
    Vector(float x, float y): x(x), y(y) {}

    const Vector operator+(const Vector& vector) const {
        return Vector(x + vector.x, y + vector.y);
    }

    const Vector operator-(const Vector& vector) const {
        return Vector(x - vector.x, y - vector.y);
    }

    const float operator*(const Vector& vector) const {
        return x * vector.x + y * vector.y;
    }

    const Vector operator*(float a) const {
        return Vector(x * a, y * a);
    }

    const Vector operator/(float a) const {
        return Vector(x / a, y / a);
    }

    float norma() const {
        return sqrt(x * x + y * y);
    }

    const Vector normaVector() const {
        if (this->norma() == 0.0) return Vector();
        return (*this / this->norma());
    }

    float x;
    float y;
};

class Point {
public:
    Point(): x(0), y(0) {}
    Point(float x, float y): x(x), y(y) {}

    void move(const Vector& vec) {
        x += vec.x;
        y += vec.y;
    }

    const Point operator+(const Point& point) const {
        return Point(x + point.x, y + point.y);
    }

    const Point operator-(const Point& point) const {
        return Point(x - point.x, y - point.y);
    }

    float x;
    float y;
};

using Points = std::vector<Point>;

float distance(float x1, float y1, float x2, float y2);
float distance(const Point& a, const Point& b);

} // namespace model

#endif // MODEL_TRAITS_H
