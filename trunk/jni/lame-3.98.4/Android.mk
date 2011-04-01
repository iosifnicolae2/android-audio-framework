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

LOCAL_MODULE := lame

LOCAL_SRC_FILES := libmp3lame/VbrTag.c \
	libmp3lame/bitstream.c \
	libmp3lame/encoder.c \
	libmp3lame/fft.c \
	libmp3lame/gain_analysis.c \
    libmp3lame/id3tag.c \
    libmp3lame/lame.c \
    libmp3lame/newmdct.c \
	libmp3lame/presets.c \
	libmp3lame/psymodel.c \
	libmp3lame/quantize.c \
	libmp3lame/quantize_pvt.c \
	libmp3lame/reservoir.c \
	libmp3lame/set_get.c \
	libmp3lame/tables.c \
	libmp3lame/takehiro.c \
	libmp3lame/util.c \
	libmp3lame/vbrquantize.c \
	libmp3lame/version.c \
	libmp3lame/mpglib_interface.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/

LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := $(LOCAL_CFLAGS) -O3 -fprefetch-loop-arrays -ffast-math

LOCAL_LDLIBS := -ldl -lc -lz -lm

include $(BUILD_STATIC_LIBRARY)