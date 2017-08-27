#ifndef COMMON_H
#define COMMON_H

#include <android/log.h>
#define  LOG_TAG    "GAME_ASTEROIDS"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace common {

float rangeRand(float min, float max);

} // namespace common

#endif // COMMON_H
