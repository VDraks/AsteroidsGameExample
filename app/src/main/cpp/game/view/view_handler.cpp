#include "view_handler.h"

#include <cstdlib>
#include <string>
#include <sstream>

#include "../common/common.h"
#include "../model/objects/ship.h"

using namespace view;

namespace {
    void printGLString(const char* name, GLenum s) {
        const char *v = (const char *) glGetString(s);
        LOGI("GL %s = %s\n", name, v);
    }

    void checkGlError(const char* op) {
        for (GLint error = glGetError(); error; error
                = glGetError()) {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

std::string vertexShader(float width, float height) {
    std::stringstream stream;
    stream.precision(2);
    stream << "attribute vec4 a_position;   \n"
            "attribute vec3 a_normal;     \n"
            "attribute vec2 a_texCoord;   \n"
            "attribute vec4 a_color;      \n"
            "varying vec2 v_texCoord;     \n"
            "varying vec3 v_normal;       \n"
            "varying vec4 v_color;        \n"
            "void main()                  \n"
            "{                            \n"
            "   gl_Position =               "
            "        vec4(a_position.x / (" << width << " / 2.0) - 1.0,"
            "             a_position.y / (" << height << " / 2.0) - 1.0, 0.0, 1.0); \n"
            "   v_normal = a_normal;      \n"
            "   v_texCoord = a_texCoord;  \n"
            "   v_color=a_color;          \n"
            "}                            \n";
    return stream.str();
}

const char gFragmentShader[] =
                "precision mediump float;                            \n"
                "varying vec2 v_texCoord;                            \n"
                "varying vec3 v_normal;                              \n"
                "varying vec4 v_color;                               \n"
                "uniform sampler2D s_texture;                        \n"
                "void main()                                         \n"
                "{                                                   \n"
                "    gl_FragColor = v_color;                         \n"
                "}                                                   \n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) return 0;

    glShaderSource(shader, 1, &pSource, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled) return shader;

    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (!infoLen) return shader;

    char* buf = (char*)malloc(infoLen);
    if (buf) {
        glGetShaderInfoLog(shader, infoLen, NULL, buf);
        LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
        free(buf);
    }
    glDeleteShader(shader);
    shader = 0;

    return shader;
}

const float gameColor[] = {1.0f, 1.0f, 1.0f};
const float gameOverColor[] = {0.0f, 0.0f, 0.0f};

} // namespace

ViewHandler::ViewHandler(model::ModelHandler* const modelHandler):
        _modelHandler(modelHandler) {
}

bool ViewHandler::setupGraphics(int screenWidth, int screenHeight, float worldWidth, float worldHeight) {
    ::printGLString("Version", GL_VERSION);
    ::printGLString("Vendor", GL_VENDOR);
    ::printGLString("Renderer", GL_RENDERER);
    ::printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", screenWidth, screenHeight);

    _program = createProgram(::vertexShader(worldWidth, worldHeight).c_str(), ::gFragmentShader);
    if (!_program) {
        LOGE("Could not create program.");
        return false;
    }

    _vPositionHandle = glGetAttribLocation(_program, "a_position");
    _vColorHandle = glGetAttribLocation(_program, "a_color");
    _vNormalHandle = glGetAttribLocation(_program,"a_normal");
    _vTexCoordHandle = glGetAttribLocation(_program, "a_texCoord");
    _vSamplerHandle = glGetAttribLocation(_program, "s_texture");

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glViewport(0, 0, screenWidth, screenHeight);
    glGenBuffers(1, &_objBuffer);
    glGenBuffers(1, &_colorBuffer);
    glUseProgram(_program);
    return true;
}

void ViewHandler::update(double /*deltaTime*/) {
    if (_modelHandler->isGameOver()) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        for (auto& obj: _modelHandler->allObjects()) {
            this->drawObject(obj, ::gameOverColor);
        }
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    for (auto& obj: _modelHandler->allObjects()) {
        this->drawObject(obj, ::gameColor);
    }
}

void ViewHandler::drawObject(const model::ObjectPtr& obj, const float color[3]) {

    GLfloat objBufferData[obj->border().size() * 2];
    GLfloat objBufferColor[obj->border().size() * 4];

    for (size_t i = 0; i < obj->border().size(); ++i) {
        objBufferData[i * 2] = obj->border().at(i).x;
        objBufferData[i * 2 + 1] = obj->border().at(i).y;

        objBufferColor[i * 4] = color[0];
        objBufferColor[i * 4 + 1] = color[1];
        objBufferColor[i * 4 + 2] = color[2];
        objBufferColor[i * 4 + 3] = 1.0f;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _objBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objBufferData), objBufferData, GL_STATIC_DRAW);
    glVertexAttribPointer(_vPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(_vPositionHandle);

    glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(objBufferColor), objBufferColor, GL_STATIC_DRAW);
    glVertexAttribPointer(_vColorHandle, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(_vColorHandle);

    glLineWidth(2.0);
    glDrawArrays(GL_LINE_LOOP, 0, obj->border().size());
}

GLuint ViewHandler::createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) return 0;

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) return 0;

    GLuint program = glCreateProgram();
    if (!program) return 0;

    glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);
    glLinkProgram(program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLint bufLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
        if (bufLength) {
            char* buf = (char*) malloc(bufLength);
            if (buf) {
                glGetProgramInfoLog(program, bufLength, NULL, buf);
                LOGE("Could not link program:\n%s\n", buf);
                free(buf);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    return program;
}
