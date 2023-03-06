cmake_minimum_required(VERSION 3.18.1)


project("myvideodemo")


add_library(
        native-render

        SHARED

        native-render.cpp
        mylog.cpp
)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


target_link_libraries( # Specifies the target library.

        native-render

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}

        # EGL依赖库
        EGL
        # OpenGL es依赖库
        GLESv3

        android
        )