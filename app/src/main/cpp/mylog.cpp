//
// Created by admin on 2023/3/6.
//

#include <cstdio>
#include "mylog.h"

void printLog(android_LogPriority logLevel, const char *tag, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    __android_log_vprint(logLevel, tag, fmt, args);
    va_end(args);
}