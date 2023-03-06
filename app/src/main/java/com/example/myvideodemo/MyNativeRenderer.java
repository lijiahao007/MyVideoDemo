package com.example.myvideodemo;

public class MyNativeRenderer {

    static {
        System.loadLibrary("native-render");
    }

    public native void drawTriangle(Object surface);

}

