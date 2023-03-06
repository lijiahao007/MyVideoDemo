package com.example.myvideodemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyGLSurfaceView extends GLSurfaceView implements GLSurfaceView.Renderer, Runnable {

    private static final String TAG = "MyGLSurfaceView";
    MyNativeRenderer myNativeRenderer = new MyNativeRenderer();

    public MyGLSurfaceView(Context context) {
        super(context);
        initView();
    }

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        initView();
    }


    private void initView() {
        // 1. 设置自定义Renderer, renderMode
        setRenderer(this);
    }


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        new Thread(this).start();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }


    @Override
    public void run() {
        myNativeRenderer.drawTriangle(getHolder().getSurface());
    }
}
