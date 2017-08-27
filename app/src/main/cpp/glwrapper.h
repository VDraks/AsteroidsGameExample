#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define ESCALLBACK

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

typedef struct
{
    GLfloat   m[4][4];
} ESMatrix;

typedef struct _escontext
{
   /// Put your user data here...
   void*       userData;

   /// Window width
   GLint       width;

   /// Window height
   GLint       height;

   /// Window handle
   EGLNativeWindowType  hWnd;

   /// EGL display
   EGLDisplay  eglDisplay;

   /// EGL context
   EGLContext  eglContext;

   /// EGL surface
   EGLSurface  eglSurface;

   /// Callbacks
   void (ESCALLBACK* drawFunc) ( struct _escontext * );
   void (ESCALLBACK* keyFunc) ( struct _escontext *, unsigned char, int, int );
   void (ESCALLBACK* updateFunc) ( struct _escontext *, float deltaTime );
} ESContext;

typedef struct
{
 // Handle to a program object
 GLuint programObject;
} UserData;