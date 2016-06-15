LOCAL_PATH:= $(call my-dir)


PDFBUFFER_PATH := $(LOCAL_PATH)/pdfbuffer
CONTENTSEND_PATH := $(LOCAL_PATH)/contentsend

include ../../../../../buildscript/AndroidSettings.mk
include $(PDFBUFFER_PATH)/Android.mk
include $(CONTENTSEND_PATH)/Android.mk


