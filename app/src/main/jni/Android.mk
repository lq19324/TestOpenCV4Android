LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


OpenCV_INSTALL_MODULES := on
OpenCV_CAMERA_MODULES := off

OPENCV_LIB_TYPE :=STATIC

ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
include ../../../../native/jni/OpenCV.mk
else
include $(OPENCV_MK_PATH)
endif

LOCAL_MODULE := test_OpenCV

LOCAL_SRC_FILES := native_test_opencv.cpp

LOCAL_LDLIBS +=  -lm -llog -ldl -ljnigraphics
#LOCAL_LDLIBS    := -lm -llog -ldl -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_EXECUTABLE)