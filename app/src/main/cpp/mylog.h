//
// Created by admin on 2023/3/6.
//

#ifndef MYVIDEODEMO_MYLOG_H
#define MYVIDEODEMO_MYLOG_H

#include <android/log.h>

#define  LOG_TAG    "HSMediaLibrary"

void printLog(android_LogPriority logLevel, const char *tag, const char *fmt, ...);

#define  LOGV(...)  printLog(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__);
#define  LOGD(...)  printLog(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#define  LOGI(...)  printLog(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
#define  LOGW(...)  printLog(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);
#define  LOGE(...)  printLog(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);

#endif //MYVIDEODEMO_MYLOG_H
