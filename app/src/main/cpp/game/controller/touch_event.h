#ifndef TOUCH_EVENT_H
#define TOUCH_EVENT_H

namespace controller {

class TouchEvent {
public:
    TouchEvent(int actionMask, float x, float y);

    int actionMask() const;
    void setActionMask(int actionMask);

    float x() const;
    float y() const;

private:
    int _actionMask;
    float _x;
    float _y;
};

} // namespace controller

#endif // TOUCH_EVENT_H
