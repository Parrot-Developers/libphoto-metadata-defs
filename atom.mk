
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libphoto-metadata-defs
LOCAL_CATEGORY_PATH := libs
LOCAL_DESCRIPTION := Photo metadata common definitions
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
# Public API headers - top level headers first
# This header list is currently used to generate a python binding
LOCAL_EXPORT_CUSTOM_VARIABLES := LIBPMETA_DEFS_HEADERS=$\
	$(LOCAL_PATH)/include/pmeta_defs.h;
LOCAL_CFLAGS := -DPMETA_DEFS_API_EXPORTS -fvisibility=hidden -std=gnu11 -D_GNU_SOURCE

LOCAL_SRC_FILES := \
	src/pmeta_defs.c

LOCAL_LIBRARIES := \
	libfutils \
	libulog

include $(BUILD_LIBRARY)


ifdef TARGET_TEST

include $(CLEAR_VARS)

LOCAL_MODULE := tst-libphoto-metadata-defs
LOCAL_CFLAGS += -DTARGET_TEST -D_GNU_SOURCE
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_SRC_FILES := \
	tests/pmeta_defs_test_main.c \
	tests/pmeta_defs_test.c

LOCAL_LIBRARIES := \
	libcunit \
	libfutils \
	libphoto-metadata-defs

include $(BUILD_EXECUTABLE)

endif
