#include "application.h"

#include "application_context.h"

using namespace asteroids;

Application* Application::self() {
    static Application app;
    return &app;
}

void Application::init(int width, int height) {
    if (!_context)
        _context.reset(new ApplicationContext(width, height));

    _context->init(width, height);
}

void Application::update() {
    _context->update();
}

void Application::onTouch(int actionMask, float x, float y) {
    _context->onTouch(actionMask, x, y);
}

void Application::onPause() {
    _context->onPause();
}

void Application::onResume() {
    if (_context)
        _context->onResume();
}

Application::Application() {}
