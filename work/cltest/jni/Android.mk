LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/include  \
	$(LOCAL_PATH)/include/CL \

LOCAL_MODULE    := test
LOCAL_SRC_FILES := ./test.cpp

LOCAL_LDLIBS := -L$(LOCAL_PATH)/externallibs  -lOpenCL

include $(BUILD_EXECUTABLE)