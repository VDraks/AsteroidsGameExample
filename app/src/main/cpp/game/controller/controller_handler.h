#ifndef CONTROLLER_HANDLER_H
#define CONTROLLER_HANDLER_H

#include <queue>
#include "../model/model_handler.h"
#include "touch_event.h"

namespace controller {

class ControllerHandler
{
public:
    ControllerHandler(model::ModelHandler* const modelHandler,
                      int screenWidth, int screenHeight,
                      float worldWidth, float worldHeight);

    void update(double deltaTime);
    void onTouch(int actionMask, float x, float y);

private:
    ControllerHandler(const ControllerHandler&);
    ControllerHandler& operator=(const ControllerHandler&);

    void processTouchEvent(const TouchEvent& event);
    void clear();

    model::ModelHandler* const _modelHandler;
    const float _worldHeight;
    const float _kx;
    const float _ky;

    std::queue<TouchEvent> _touchEvents;

    double _touchTime;
    bool _isPushed;
    bool _isTouch;

    float _x;
    float _y;
};

} // namespace controller

#endif // CONTROLLER_HANDLER_H
