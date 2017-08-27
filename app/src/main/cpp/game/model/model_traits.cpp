#include "model_traits.h"

using namespace model;

float model::distance(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float model::distance(const Point &a, const Point &b) {
    return distance(a.x, a.y, b.x, b.y);
}
