#ifndef VIEW_HANDLER_H
#define VIEW_HANDLER_H

#include "glwrapper.h"

#include "../model/model_handler.h"

namespace view {

class ViewHandler {
public:
    ViewHandler(model::ModelHandler* const modelHandler);

    bool setupGraphics(int screenWidth, int screenHeight, float worldWidth, float worldHeight);
    void update(double deltaTime);

private:
    ViewHandler(const ViewHandler&);
    ViewHandler& operator=(const ViewHandler&);

    model::ModelHandler* const _modelHandler;

    GLuint _program;
    GLuint _vPositionHandle;
    GLuint _vColorHandle;
    GLuint _vNormalHandle;
    GLuint _vTexCoordHandle;
    GLuint _vSamplerHandle;

    GLuint _objBuffer;
    GLuint _colorBuffer;

    void drawObject(const model::ObjectPtr& obj, const float color[3]);
    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
};

} // namespace view

#endif // VIEW_HANDLER_H
