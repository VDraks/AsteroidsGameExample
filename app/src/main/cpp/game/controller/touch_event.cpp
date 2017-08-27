#include "touch_event.h"

using namespace controller;

TouchEvent::TouchEvent(int actionMask, float x, float y):
        _actionMask(actionMask),
        _x(x),
        _y(y) {

}
int TouchEvent::actionMask() const {
    return _actionMask;
}

float TouchEvent::x() const {
    return _x;
}

float TouchEvent::y() const {
    return _y;
}
