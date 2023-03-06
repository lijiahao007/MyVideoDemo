//
// Created by admin on 2023/1/10.
//

#include <jni.h>
#include <string>
#include "mylog.h"
#include "FragmentShader.h"

#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

GLint initShader(const char *source, int type);


GLint initShader(const char *source, GLint type) {
    //创建shader
    GLint sh = glCreateShader(type);
    if (sh == 0) {
        LOGD("glCreateShader %d failed", type);
        return 0;
    }
    //加载shader
    glShaderSource(sh,
                   1,//shader数量
                   &source,
                   0);//代码长度，传0则读到字符串结尾

    //编译shader
    glCompileShader(sh);

    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        LOGD("glCompileShader %d failed", type);
        LOGD("source %s", source);
        auto *infoLog = new GLchar[1024];
        GLsizei length;
        glGetShaderInfoLog(sh, 1024, &length, infoLog);
//        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        LOGD("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s", infoLog);
        return 0;
    }

    LOGD("glCompileShader %d success", type);
    return sh;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_myvideodemo_MyNativeRenderer_drawTriangle(JNIEnv *env, jobject thiz,
                                                           jobject surface) {

    /**        此处开始EGL的配置              **/
    // 1.获取原始窗口，ANativeWindow 就是Surface在Native的对应物。这里的surface参数即从Java层穿过来的Surface对象
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);

    // 2. 获取OpenGL es渲染目标Display，EGL_DEFAULT_DISPLAY表示默认的Display
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LOGD("egl display failed");
        return;
    }

    // 3. 初始化egl Display的连接，后两个参数是指针，是分别用来返回EGL主次版本号
    if (EGL_TRUE != eglInitialize(display, 0, 0)) {
        LOGD("eglInitialize failed");
        return;
    }


    // 返回的EGL帧缓存配置
    EGLConfig eglConfig;
    // 配置数量
    EGLint configNum;
    //期望的EGL帧缓存配置列表,配置为一个key一个value的形式，
    //      以下的EGL_RED_SIZE、EGL_GREEN_SIZE、EGL_BLUE_SIZE分别表示EGL帧缓冲中的颜色缓冲一个颜色通道用多少位表示。

    // 指定EGL surface类型
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };

    // 返回一个和 期望的EGL帧缓存配置列表configSpec 匹配的EGL帧缓存配置列表，存储在eglConfig中
    if (EGL_TRUE != eglChooseConfig(display, configSpec, &eglConfig, 1, &configNum)) {
        LOGD("eglChooseConfig failed");
        return;
    }

    // 通过 egl 和 NativeWindow 以及 EGL帧缓冲配置 创建EGLSurface。最后一个参数为属性信息，0表示不需要属性)
    EGLSurface winSurface = eglCreateWindowSurface(display, eglConfig, nwin, 0);
    if (winSurface == EGL_NO_SURFACE) {
        LOGD("eglCreateWindowSurface failed");
        return;
    }

    // 渲染上下文 EGLContext 关联的帧缓冲配置列表，EGL_CONTEXT_CLIENT_VERSION 表示这里是配置EGLContext的版本，
    const EGLint ctxAttr[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };

    // 通过 Display 和上面获取到的的 EGL帧缓存配置列表 创建一个EGLContext， EGL_NO_CONTEXT表示不需要多个设备共享上下文
    EGLContext context = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        //EGL_NO_CONTEXT表示创建上下文失败
        LOGD("eglCreateContext failed");
        return;
    }

    // 将 EGLContext 和 当前线程 以及 draw和read的EGLSurface 关联。关联之后，当前线程就成为了OpenGL es的渲染线程
    if (EGL_TRUE != eglMakeCurrent(display, winSurface, winSurface, context)) {
        LOGD("eglMakeCurrent failed");
        return;
    }
    /**        此处结束EGL的配置              **/



    GLint vsh = initShader(vertexSimpleShape, GL_VERTEX_SHADER);
    GLint fsh = initShader(fragSimpleShape, GL_FRAGMENT_SHADER);


    GLint program = glCreateProgram();
    if (program == 0) {
        LOGD("glCreateProgram failed");
        return;
    }


    glAttachShader(program, vsh);
    glAttachShader(program, fsh);


    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0) {
        LOGD("glLinkProgram failed");
        return;
    }
    LOGD("glLinkProgram success");

    glUseProgram(program);

    static float triangleVer[] = {
            0.8f, -0.8f, 0.0f,
            -0.8f, -0.8f, 0.0f,
            0.0f, 0.8f, 0.0f,
    };

    GLuint apos = static_cast<GLuint>(glGetAttribLocation(program, "aPosition"));
    glEnableVertexAttribArray(apos);
    glVertexAttribPointer(apos, 3, GL_FLOAT, GL_FALSE, 0, triangleVer);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

    eglSwapBuffers(display, winSurface);
}
