#include "application_context.h"

#include <ctime>

using namespace asteroids;

namespace {
const float worldHeight = 1000.0f;

float worldWidth(int screenWidth, int screenHeight) {
    return ::worldHeight * (static_cast<float>(screenWidth)
                            / static_cast<float>(screenHeight));
}

double nowMs(void) {
    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
}
} // namespace

asteroids::ApplicationContext::ApplicationContext(int width, int height):
        _modelHandler(::worldWidth(width, height), ::worldHeight),
        _collisionHandler(&_modelHandler),
        _controllerHandler(&_modelHandler, width, height,
                           ::worldWidth(width, height), ::worldHeight),
        _viewHandler(&_modelHandler),
        _lastTime(::nowMs()) {
}

void ApplicationContext::init(int width, int height) {
    _viewHandler.setupGraphics(width, height,
                               ::worldWidth(width, height), ::worldHeight);
}

void ApplicationContext::update() {
    double nowTime = ::nowMs();
    double deltaTime = nowTime - _lastTime;
    _lastTime = nowTime;

    _modelHandler.update(deltaTime);
    _collisionHandler.update(deltaTime);
    _controllerHandler.update(deltaTime);
    _viewHandler.update(deltaTime);
}

void ApplicationContext::onTouch(int actionMask, float x, float y) {
    _controllerHandler.onTouch(actionMask, x, y);
}

void ApplicationContext::onPause() {

}

void ApplicationContext::onResume() {
    _lastTime = ::nowMs();
}
