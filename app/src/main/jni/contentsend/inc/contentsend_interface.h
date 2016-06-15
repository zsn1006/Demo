
#ifndef CONTENTSEND_INTERFACE_H
#define CONTENTSEND_INTERFACE_H


typedef struct _ContentSendInterface {
	void (*init)(int width, int height,int framerate);
	void (*unInit)();
	void (*setContentCapsParameter)(int width, int height,int framerate);
	int  (*getPageYUVBuffer)(char* pOut, int* pWidth, int* pHeight);
	int  (*getPageRGBABuffer)(char* pOut, int* pWidth, int* pHeight);

} ContentSendInterface ;

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

extern volatile const ContentSendInterface* g_contentSendInterface;
extern volatile int g_initContentWidth;
extern volatile int g_initContentHeight;
extern volatile int g_initContentFramerate;


void contentSendInit();
void contentSendUninit();




#ifdef __cplusplus	
}
#endif  // __cplusplus

#endif // CONTENTSEND_INTERFACE_H
