#ifndef APPLICATION_CONTEXT_H
#define APPLICATION_CONTEXT_H

#include "collision/collision_handler.h"
#include "controller/controller_handler.h"
#include "model/model_handler.h"
#include "view/view_handler.h"

namespace asteroids {

class ApplicationContext
{
public:
    ApplicationContext(int width, int height);

    void init(int width, int height);
    void update();
    void onTouch(int actionMask, float x, float y);
    void onPause();
    void onResume();

private:
    model::ModelHandler _modelHandler;
    collision::CollisionHandler _collisionHandler;
    controller::ControllerHandler _controllerHandler;
    view::ViewHandler _viewHandler;
    double _lastTime;
};

} // namespace asteroids

#endif // APPLICATION_CONTEXT_H
