#ifndef __LOG_H__
#define __LOG_H__

#include <jni.h>
#include <android/log.h>
#define LOG_DEBUG
#ifdef LOG_DEBUG
	#define LOG_TAG				"arcsoft"
	#define LOG(...)			__android_log_print(ANDROID_LOG_ERROR,  LOG_TAG,  __VA_ARGS__)

#else
	#define LOG
#endif

#endif