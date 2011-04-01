# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := $(LOCAL_CFLAGS) -O3
LOCAL_CPPFLAGS := $(LOCAL_CPPFLAGS) -O3

LOCAL_MODULE    := core
LOCAL_SRC_FILES := AacEncoder.cpp \
	AudioEncodingController.cpp \
	AudioEncodingJni.cpp \
	EncoderConfiguration.cpp \
	LameEncoder.cpp

LOCAL_LDLIBS := -ldl -llog -lc
LOCAL_STATIC_LIBRARIES := faac lame 

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../faac-1.28/include/ \
	$(LOCAL_PATH)/../lame-3.98.4/include/ \
	$(LOCAL_PATH)/../libogg-1.2.2/include/ogg/
	
include $(BUILD_SHARED_LIBRARY)