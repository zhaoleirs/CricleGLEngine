LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= sewisdomGL
LOCAL_CFLAGS += -D GL_GLEXT_PROTOTYPES
LOCAL_SRC_FILES :=\
	zip_support/ioapi.cpp \
	zip_support/unzip.cpp \
	Common.cpp \
	PngUtil.cpp \
	Texture.cpp \
	GLInterface.cpp	\
	GLBuffer.cpp \
	Ani.cpp \
	BytesReader.cpp \
	FileUnit.cpp
LOCAL_LDLIBS := -llog -lGLESv1_CM -lz

LOCAL_STATIC_LIBRARIES := libpng

include $(BUILD_SHARED_LIBRARY)
