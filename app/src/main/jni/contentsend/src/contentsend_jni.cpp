
#include<stdlib.h>
#include<jni.h>
#include "androidlog.h"

#include <contentsend_interface.h>


enum ContentType {
	PDF,
	IMAGE
};

extern "C" void setPageChanged(bool isChanged);
extern  ContentSendInterface pdfContentInterface;
//extern  ContentSendInterface pictureContentInterface;


extern "C" JNIEXPORT void JNICALL Java_com_polycom_mfw_contentsend_ContentSendJNI_initContentSend
    (JNIEnv* env, jclass clazz, jint width, jint height)
{
	if(g_contentSendInterface != NULL ) 
    {
		g_contentSendInterface->init(width, height, 3);
	}

}

extern "C" JNIEXPORT void JNICALL Java_com_polycom_mfw_contentsend_ContentSendJNI_unInitContentSend
    (JNIEnv* env, jclass clazz)
{
	if(g_contentSendInterface != NULL ) 
    {
		g_contentSendInterface->unInit();
	}
}

extern "C" JNIEXPORT void JNICALL Java_com_polycom_mfw_contentsend_ContentSendJNI_setPageChanged
    (JNIEnv* env, jclass clazz)
{
    setPageChanged(true);
}

extern "C" JNIEXPORT void JNICALL Java_com_polycom_mfw_contentsend_ContentSendJNI_setContentSize
    (JNIEnv* env, jclass clazz, jint width, jint height)
{
	if(g_contentSendInterface != NULL ) 
    {
		g_contentSendInterface->setContentCapsParameter(width, height, 3);
	}
}

extern "C" JNIEXPORT int JNICALL Java_com_polycom_mfw_contentsend_ContentSendJNI_getContentBuffer
    (JNIEnv* env, jclass clazz, jobject buffer, jint bufferType)
{
    int retlen = 0;
	if(g_contentSendInterface != NULL ) 
    {	
        int w,h;
        
        char *buf = (char*)env->GetDirectBufferAddress(buffer);    
        
        if(1 == bufferType)//RGBA
        {
            retlen = g_contentSendInterface->getPageRGBABuffer(buf, &w, &h);
        }
        else //YUV
        {
            retlen = g_contentSendInterface->getPageYUVBuffer(buf, &w, &h);
        }
	}
    return retlen;
}

extern "C" JNIEXPORT void JNICALL Java_com_polycom_mfw_contentsend_ContentSendJNI_setContentType(JNIEnv* env, jclass clazz, jint nType)
{
	nLogi("setContentType : type = %d   g_contentSendInterface is : %x", nType,g_contentSendInterface);

	if(g_contentSendInterface != NULL ) {

		g_contentSendInterface->unInit();
	}
	switch(nType){

		case PDF:   // pdf
			g_contentSendInterface = &pdfContentInterface;
			break;
		case IMAGE:	  //
			//g_contentSendInterface = &pictureContentInterface;
			break;
		default :
			g_contentSendInterface = NULL;
	}
	
	if(g_contentSendInterface != NULL ) {

		g_contentSendInterface->init(g_initContentWidth, g_initContentHeight, g_initContentFramerate);
	}
}

