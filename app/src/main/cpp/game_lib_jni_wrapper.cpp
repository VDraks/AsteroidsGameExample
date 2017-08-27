#include <jni.h>

#include "game/application.h"

extern "C"
JNIEXPORT void JNICALL
Java_ru_kytin_zeptolab_asteroids_GameLibJNIWrapper_onSurfaceCreated(JNIEnv* env, jclass cls)  {

}

extern "C"
JNIEXPORT void JNICALL
Java_ru_kytin_zeptolab_asteroids_GameLibJNIWrapper_onSurfaceChanged(JNIEnv * env, jclass cls, jint width, jint height) {
    asteroids::Application::self()->init(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_kytin_zeptolab_asteroids_GameLibJNIWrapper_onDrawFrame(JNIEnv * env, jclass cls) {
    asteroids::Application::self()->update();
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_kytin_zeptolab_asteroids_GameLibJNIWrapper_onTouch(JNIEnv* env, jclass cls, jint actionMask, jfloat x, jfloat y) {
    asteroids::Application::self()->onTouch(actionMask, x, y);
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_kytin_zeptolab_asteroids_GameLibJNIWrapper_onPause(JNIEnv* env, jclass cls) {
    asteroids::Application::self()->onPause();
}

extern "C"
JNIEXPORT void JNICALL
Java_ru_kytin_zeptolab_asteroids_GameLibJNIWrapper_onResume(JNIEnv* env, jclass type) {
    asteroids::Application::self()->onResume();
}