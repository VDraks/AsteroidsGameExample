package ru.kytin.zeptolab.asteroids;

class GameLibJNIWrapper {

    static {
        System.loadLibrary("game");
    }

    public static native void onSurfaceCreated();

    public static native void onSurfaceChanged(int width, int height);

    public static native void onDrawFrame();

    public static native void onTouch(int actionMask, float x, float y);

    public static native void onPause();

    public static native void onResume();
}
