#include "controller_handler.h"

#include <android/input.h>

using namespace controller;

namespace {
const double pushedTime = 140.00;
const float shipAccelerationK = 0.0005;
} // namespace

ControllerHandler::ControllerHandler(model::ModelHandler* const modelHandler,
                                     int screenWidth, int screenHeight,
                                     float worldWidth, float worldHeight):
        _modelHandler(modelHandler),
        _worldHeight(worldHeight),
        _kx(worldWidth / screenWidth),
        _ky(worldHeight / screenHeight),
        _touchTime(0),
        _isPushed(false),
        _isTouch(false),
        _x(0),
        _y(0) {
}

void ControllerHandler::update(double deltaTime) {
    if (_isTouch) {
        _touchTime += deltaTime;
    }

    if (_touchTime >= ::pushedTime) {
        _isPushed = true;
    }

    if (_modelHandler->isGameOver()) {
        while (!_touchEvents.empty()) {
            if (_touchEvents.front().actionMask() == AMOTION_EVENT_ACTION_UP) {
                _modelHandler->newGame();
                break;
            }
            _touchEvents.pop();
        }
        this->clear();
        return;
    }

    while (!_touchEvents.empty()) {
        this->processTouchEvent(_touchEvents.front());
        _touchEvents.pop();
    }

    if (_isPushed) {
        model::Vector accelerationDirection(_x - _modelHandler->ship()->x(),
                                            _y - _modelHandler->ship()->y());
        _modelHandler->ship()->setAcceleration(accelerationDirection * ::shipAccelerationK);
    }
}

void ControllerHandler::onTouch(int actionMask, float x, float y) {
    _touchEvents.push(TouchEvent(actionMask, x * _kx, _worldHeight - y * _ky));
}

void ControllerHandler::processTouchEvent(const TouchEvent& event) {
    switch (event.actionMask()) {
        case AMOTION_EVENT_ACTION_DOWN:
            _isTouch = true;
            _isPushed = false;
            _touchTime = 0;
            break;
        case AMOTION_EVENT_ACTION_UP:
            if (!_isPushed)
                _modelHandler->addProjectile();

            _isTouch = false;
            _isPushed = false;
            _touchTime = 0;

            _modelHandler->ship()->setAcceleration(model::Vector(0.0, 0.0));

            break;
        case AMOTION_EVENT_ACTION_MOVE:
            break;
        default:
            break;
    }

    _x = event.x();
    _y = event.y();
}

void ControllerHandler::clear() {
    std::queue<TouchEvent> empty;
    std::swap(_touchEvents, empty);

    _isTouch = false;
    _isPushed = false;
    _touchTime = 0;
    _x = 0;
    _y = 0;
}
