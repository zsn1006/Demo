#ifndef _ANDROID_LOG_H_
#define _ANDROID_LOG_H_

#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define nLoge(fmt...) __android_log_print(ANDROID_LOG_ERROR, "Conversation", ##fmt);

#define nLogw(fmt...) __android_log_print(ANDROID_LOG_WARN, "Conversation", ##fmt);

#define nLogi(fmt...) __android_log_print(ANDROID_LOG_INFO, "Conversation", ##fmt);

#define nLogd(fmt...) __android_log_print(ANDROID_LOG_DEBUG, "Conversation", ##fmt);

#ifdef __cplusplus
}
#endif

#endif
