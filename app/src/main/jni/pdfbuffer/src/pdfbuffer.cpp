#include "androidlog.h"
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

#include "fs_base.h"
#include "fpdf_base.h"
#include "fpdf_document.h"
#include "fpdf_view.h"
#include "fpdf_annot.h"
#include "fpdf_text.h"
#include "fpdf_reflow.h"
#include "fpdf_formfield.h"
#include "fpdf_security.h"
#include "fpdf_signature.h"
#include "fpsi.h"

#include "contentsend_interface.h"

extern pthread_mutex_t foxitRenderLock;

extern "C"
{

#define USE_NEON_RGBA2YUV 0
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// Page info set by java ui
FPDF_PAGE pageHandle = 0;
int pageStartX = 0;
int pageEndX = 0;
int pageStartY = 0;
int pageEndY = 0;
int pageChanged = 0;

// This buffer will match the requested resolution, and be sent to mp channel
FS_BITMAP bmpCache = NULL;

// All operations on upper parameters should be acted when bFoxitActive is true
int bFoxitActive = 0;

int requestedWidth = 1280;
int requestedHeight = 720;
int requestedFrameRate = 3;

// system status flag, controlled in mpc channel thread
int bChanActive = 0;
pthread_mutex_t envLock;


void initFoxitEnv(int width, int height, int framerate);
void uninitFoxitEnv();
void setContentCapsParameter(int width, int height, int framerate);
void getContentCapsParameter(int *pWidth, int *pHeight, int *pFrameRate);
void setPage(int newPageHandle, int newPageStartX, int newPageEndX, int newPageStartY, int newPageEndY);
void closeDoc();
int getPageYUVBuffer(char *pOut, int *pWidth, int *pHeight);
int getPageRGBABuffer(char *pOut, int *pWidth, int *pHeight);
int renderCurrentPage(char *pOut);
int renderCurrentPageAsRGBA(char *pOut);
int addBlackEdgeToOriginRGB(int dib, int leftValid, int rightValid, int topValid, int bottomValid);
unsigned int RealTimeMilliSeconds(void);

#if USE_NEON_RGBA2YUV
int rgba2yuv_16x16_aligned_neon(int width, int height, unsigned char *rgba, unsigned char *yuv);
#else
void init_rgba2yuv_table();
int rgba2yuv(int width, int height, void *rgb, void *yuv);
#endif

// content send interface
ContentSendInterface pdfContentInterface = {
	initFoxitEnv,
	uninitFoxitEnv,
	setContentCapsParameter,
	getPageYUVBuffer,
	getPageRGBABuffer
};


void initFoxitEnv(int width, int height, int framerate)
{
	FS_RESULT nRet = 0;
	bmpCache = 0;
	pageHandle = 0;
	pageChanged = 0;
	requestedWidth = width;
	requestedHeight = height;
	requestedFrameRate = framerate;

	if(bChanActive)
		return;

	nLogi("initFoxitEnv, width(%d), height(%d), framerate(%d)", width, height, framerate);	

#if !USE_NEON_RGBA2YUV
	init_rgba2yuv_table();
#endif

	pthread_mutex_init(&envLock, NULL);

	bChanActive = 1;
}

void uninitFoxitEnv()
{
	FS_RESULT nRet = 0;

	if(!bChanActive)
		return;

	bChanActive = 0;

	nLogi("uninitFoxitEnv");

	pthread_mutex_lock(&envLock);

	if(bmpCache && bFoxitActive)
	{
		nLogi("uninitFoxitEnv destroy bmpCache = 0x%X", bmpCache);
		nLogi("memcheck--pdfbuffer-docdestroy, doc: %x", bmpCache);
		nRet = FS_Bitmap_Destroy(bmpCache);
		nLogi("uninitFoxitEnv, FS_Bitmap_Destroy ret %d", nRet);
		bmpCache = NULL;
	}

	pthread_mutex_unlock(&envLock);

	pthread_mutex_destroy(&envLock);
}

void setContentCapsParameter(int width, int height, int framerate)
{
	FS_RESULT nRet = 0;
	
	if(!bChanActive)
		return;

	nLogi("setContentCapsParameter, width(%d), height(%d), framerate(%d)", width, height, framerate);

	pthread_mutex_lock(&envLock);
	if(bFoxitActive && bmpCache && ((width !=  requestedWidth) || (height != requestedHeight)))
	{
		nLogi("setContentCapsParameter destroy bmpCache = 0x%X", bmpCache);
		nLogi("memcheck--pdfbuffer-docdestroy, doc: %x", bmpCache);
		nRet = FS_Bitmap_Destroy(bmpCache);
		nLogi("setContentCapsParameter, FS_Bitmap_Destroy ret %d", nRet);
	
		nRet = FS_Bitmap_Create(width, height, FS_DIBFORMAT_RGBA, NULL, 0, &bmpCache);
		nLogi("memcheck--pdfbuffer-doccreate, doc: %x", bmpCache);
		if(nRet)
		{
			nLoge("setContentCapsParameter, realloc FS_Bitmap_Create error, ret %d", nRet);
		}
		else
		{
			nLogi("setContentCapsParameter, realloc FS_Bitmap_Create success, bmpCache = 0x%X", bmpCache);
			memset(FS_Bitmap_GetBuffer(bmpCache), 0xff, width * height * 4);
		}	
	}
	requestedWidth = width;
	requestedHeight = height;
	requestedFrameRate = framerate;
	pthread_mutex_unlock(&envLock);
}

void getContentCapsParameter(int *pWidth, int *pHeight, int *pFrameRate)
{
	if(!bChanActive)
		return;
	
	pthread_mutex_lock(&envLock);
	if(pWidth) *pWidth = requestedWidth;
	if(pHeight) *pHeight = requestedHeight;
	if(pFrameRate) *pFrameRate = requestedFrameRate;
	pthread_mutex_unlock(&envLock);
}
void setPageChanged(bool isChanged)
{   
	pthread_mutex_lock(&envLock);
    pageChanged = isChanged;
	pthread_mutex_unlock(&envLock);
}
void setPage(int newPageHandle, int newPageStartX, int newPageEndX, int newPageStartY, int newPageEndY)
{
	bFoxitActive = 1;

	if(!bChanActive)
		return;

	nLogi("setPage, pageHandle(0x%X), pageStart(%.2f, %.2f), pageEnd(%.2f, %.2f)",
		newPageHandle, 
		((float)newPageStartX) / 1000.0f ,
		((float)newPageStartY) / 1000.0f,
		((float)newPageEndX) / 1000.0f,
		((float)newPageEndY) / 1000.0f);

	pthread_mutex_lock(&envLock);
	if(((int)pageHandle) != newPageHandle ||
		pageStartX != newPageStartX ||
		pageEndX != newPageEndX ||
		pageStartY != newPageStartY ||
		pageEndY !=  newPageEndY)
	{
		pageHandle = (FPDF_PAGE)newPageHandle;
		pageStartX = newPageStartX;
		pageEndX = newPageEndX;
		pageStartY = newPageStartY;
		pageEndY =  newPageEndY;
		pageChanged = 1;
	}
	pthread_mutex_unlock(&envLock);
}

void closeDoc()
{
	FS_RESULT nRet = 0;
	bFoxitActive = 0;

	if(!bChanActive)
		return;
		
	nLogi("pdfbuffer, close doc");
	pthread_mutex_lock(&envLock);
	pageHandle = 0;
	if(bmpCache)
	{
		nLogi("closeDoc destroy bmpCache = 0x%X", bmpCache);
		nRet = FS_Bitmap_Destroy(bmpCache);
		nLogi("closeDoc, FS_Bitmap_Destroy ret %d", nRet);
		bmpCache = NULL;
	}
	pthread_mutex_unlock(&envLock);
}
int bmp_write (unsigned char * image, int xsize, int ysize, char * filename) 
{ 
    unsigned char header [54] = { 
    0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0, 
    54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 32, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0 
    }; 

    long file_size = (long) xsize * (long) ysize * 4 + 54; 
    header [2] = (unsigned char) (file_size & 0x000000ff); 
    header [3] = (file_size >> 8) & 0x000000ff; 
    header [4] = (file_size >> 16) & 0x000000ff; 
    header [5] = (file_size >> 24) & 0x000000ff; 

    long width = xsize; 
    header [18] = width & 0x000000ff; 
    header [19] = (width & 0x0000ff00) >> 8; 
    header [20] = (width & 0x00ff0000) >> 16; 
    header [21] = (width & 0xff000000) >> 24; 

    int height = ysize; 
    header [22] = height &0x000000ff; 
    header [23] = (height >> 8) &0x000000ff; 
    header [24] = (height >> 16) &0x000000ff; 
    header [25] = (height >> 24) &0x000000ff; 

    char fname_bmp [128]; 
    sprintf (fname_bmp, "%s.bmp", filename); 

    FILE * fp; 
    if (! (fp = fopen (fname_bmp, "ab"))) 
    return -1; 

    fwrite (header, sizeof (unsigned char), 54, fp); 
    fwrite (image, sizeof (unsigned char), (size_t) (long) xsize * ysize * 4, fp); 

    fclose (fp); 
    return 0; 
}

int getPageRGBABuffer(char *pOut, int *pWidth, int *pHeight)
{
    FS_RESULT nRet = 0;
    int retLen = 0;

    if(!bChanActive || !bFoxitActive)
    {
        nLogw("getPageRGBABuffer, bChanActive %s or bFoxitActive %s is false", bChanActive?"true":"false", 
            bFoxitActive?"true":"false");
        return 0;
    }

    if(!pOut)
    {
        nLoge("getPageRGBABuffer, pOut is NULL");
        return 0;
    }

    pthread_mutex_lock(&envLock);

    if(pageHandle == NULL)
    {
        nLogw("getPageRGBABuffer, pageHandel is NULL");
        pthread_mutex_unlock(&envLock);
        return 0;
    }
    
    if(bmpCache == NULL)
    {
        nRet = FS_Bitmap_Create(requestedWidth, requestedHeight, FS_DIBFORMAT_RGBA, NULL, 0, &bmpCache);
        nLogi("memcheck--pdfbuffer-doccreate, doc: %x", bmpCache);
        if(nRet)
        {
            nLoge("getPageRGBABuffer, FS_Bitmap_Create error, ret %d", nRet);
        }
        else
        {
            nLogi("getPageRGBABuffer, FS_Bitmap_Create success, bmpCache = 0x%X", bmpCache);
            memset(FS_Bitmap_GetBuffer(bmpCache), 0xff, requestedWidth * requestedHeight * 4);
        }   
    }   
    
    if(pageHandle && bmpCache)
    {
        retLen = requestedWidth * requestedHeight * 4;

        if(pageChanged)
        {
            nLogi("getPageRGBABuffer, pageChanged need to render");
            renderCurrentPageAsRGBA(pOut);
            pageChanged = 0;
            memcpy(FS_Bitmap_GetBuffer(bmpCache), pOut, retLen); // cache to bmp buffer
        }

        memcpy(pOut, FS_Bitmap_GetBuffer(bmpCache), retLen);
    	//FILE *fp = fopen("/sdcard/myrgb.rgb", "ab");
    	//fwrite(pOut, 1, retLen, fp);
    	//fclose(fp);
        //nLogi("getPageRGBABuffer, bmp_write:%d,%d", requestedWidth,requestedHeight);
    	//bmp_write((unsigned char*)pOut,requestedWidth, requestedHeight, (char*)"/sdcard/content");
    }
    else
    {
        nLoge("getPageRGBABuffer, should not be here, pageHandle or bmpCache is 0");
    }

    pthread_mutex_unlock(&envLock);

    return retLen;
}

// Match the dib to requested buffer
int getPageYUVBuffer(char *pOut, int *pWidth, int *pHeight)
{
	FS_RESULT nRet = 0;
	int retLen = 0;

	if(!bChanActive || !bFoxitActive)
	{
        nLogw("getPageRGBABuffer, bChanActive %s or bFoxitActive %s is false", bChanActive?"true":"false", 
            bFoxitActive?"true":"false");
		return 0;
	}

	if(!pOut)
	{
		nLoge("getPageYUVBuffer, pOut is NULL");
		return 0;
	}

	pthread_mutex_lock(&envLock);

	if(pageHandle == NULL)
	{
		nLogw("getPageYUVBuffer, pageHandel is NULL");
		pthread_mutex_unlock(&envLock);
		return 0;
	}
	
	if(bmpCache == NULL)
	{
		nRet = FS_Bitmap_Create(requestedWidth, requestedHeight, FS_DIBFORMAT_RGBA, NULL, 0, &bmpCache);
		nLogi("memcheck--pdfbuffer-doccreate, doc: %x", bmpCache);
		if(nRet)
		{
			nLoge("getPageYUVBuffer, FS_Bitmap_Create error, ret %d", nRet);
		}
		else
		{
			nLogi("getPageYUVBuffer, FS_Bitmap_Create success, bmpCache = 0x%X", bmpCache);
			memset(FS_Bitmap_GetBuffer(bmpCache), 0xff, requestedWidth * requestedHeight * 4);
		}	
	}	
	
	if(pageHandle && bmpCache)
	{
		retLen = requestedWidth * requestedHeight * 3 / 2;

		if(pageChanged)
		{
			nLogi("getPageYUVBuffer, pageChanged need to render");
			renderCurrentPage(pOut);
			pageChanged = 0;
			memcpy(FS_Bitmap_GetBuffer(bmpCache), pOut, retLen); // cache to bmp buffer
		}

		memcpy(pOut, FS_Bitmap_GetBuffer(bmpCache), retLen);
	}
	else
	{
		nLoge("getPageYUVBuffer, should not be here, pageHandle or bmpCache is 0");
	}

	pthread_mutex_unlock(&envLock);

	return retLen;
}
int renderCurrentPageAsRGBA(char *pOut)
{
	FS_RESULT nRet = 0;
	
	if(!bChanActive)
		return -1;

	if(pageHandle == NULL || bmpCache == NULL)
	{
		nLoge("renderCurrentPage pageHandle or bmpCache is NULL");
		return -1;
	}

	if(!pOut)
	{
		nLoge("renderCurrentPage pOut is NULL");
		return -1;
	}

	memset(FS_Bitmap_GetBuffer(bmpCache), 0xff, requestedWidth * requestedHeight * 4);

	FS_FLOAT pageWidth = 0;
	FS_FLOAT pageHeight = 0;

	nRet = FPDF_Page_GetSize(pageHandle, &pageWidth, &pageHeight);
	if(nRet)
	{
		nLoge("renderCurrentPage, FPDF_Page_GetSize return error = %d", nRet);
		return -1;
	}

	nLogi("renderCurrentPage, pageHandle(0x%X), pageStart(%.2f, %.2f), pageEnd(%.2f, %.2f)",
		pageHandle,
		((float)pageStartX) / 1000.0f,
		((float)pageStartY) / 1000.0f,
		((float)pageEndX) / 1000.0f,
		((float)pageEndY) / 1000.0f);

//	nLogi("renderCurrentPage, requestW-H(%d, %d)", requestedWidth, requestedHeight);
//	nLogi("renderCurrentPage, pageW-H(%.0f, %.0f)", pageWidth, pageHeight);

	float visibleWidth = pageWidth * (pageEndX - pageStartX) / 1000.0f;
	float visibleHeight = pageHeight * (pageEndY - pageStartY) / 1000.0f;
	float pageFactor = MIN((requestedWidth / visibleWidth), (requestedHeight / visibleHeight));

//	nLogi("renderCurrentPage, visibleW-H(%.0f, %.0f), factor(%.2f)", visibleWidth, visibleHeight, pageFactor);

	int size_x = pageWidth * pageFactor;
	int size_y = pageHeight * pageFactor;

	int visible_size_x = visibleWidth * pageFactor;
	int visible_size_y = visibleHeight * pageFactor;

	int start_x = 0, start_y = 0, visible_start_x = 0, visible_end_x = 0, visible_start_y = 0, visible_end_y = 0;

	if(visible_size_x > requestedWidth) {
		visible_start_x = 0;
		visible_end_x = requestedWidth - 1;
	} else {
		visible_start_x = (requestedWidth - visible_size_x) / 2;
		visible_end_x = (requestedWidth + visible_size_x) / 2;
	}
	start_x = visible_start_x - (size_x * pageStartX / 1000.0f);

	if(visible_size_y > requestedHeight) {
		visible_start_y = 0;
		visible_end_y = requestedHeight - 1;
	} else {
		visible_start_y = (requestedHeight - visible_size_y) / 2;
		visible_end_y = (requestedHeight + visible_size_y) / 2;
	}
	start_y = visible_start_y - (size_y * pageStartY / 1000.0f);

//	nLogi("renderCurrentPage, start x-y (%d, %d)", start_x, start_y);

	pthread_mutex_lock(&foxitRenderLock);
	nRet = FPDF_RenderPage_Start(bmpCache, pageHandle, 
			start_x,
			start_y,
			size_x,
			size_y,
			0, 0, NULL, NULL);
	pthread_mutex_unlock(&foxitRenderLock);

	if(nRet)
	{
		nLoge("renderCurrentPage, FPDF_RenderPage_Start return error = %d", nRet);
		return -1;
	}

	nRet = addBlackEdgeToOriginRGB((int)bmpCache, visible_start_x, visible_end_x, visible_start_y, visible_end_y);

#if 0
	FILE *fp = fopen("/sdcard/myrgb.rgb", "ab");
	fwrite(FS_Bitmap_GetBuffer(bmpCache), 1, requestedWidth * requestedHeight * 4, fp);
	fclose(fp);
#endif

	unsigned int startTime = RealTimeMilliSeconds();
    char* buf = (char*)FS_Bitmap_GetBuffer(bmpCache);
    int size = requestedWidth * requestedHeight * 4;
    if(NULL != buf)
    {
        memcpy(pOut, buf, size);
    }
	unsigned int endTime = RealTimeMilliSeconds();

	nLogi("rgba2yuv convert Time: %d, width(%d), height(%d)", endTime - startTime, requestedWidth, requestedHeight);

#if 0
	FILE *fp = fopen("/sdcard/myyuv.yuv", "ab");
	fwrite(pOut, 1, requestedWidth * requestedHeight * 3 / 2, fp);
	fclose(fp);
#endif

	return 0;
}

int renderCurrentPage(char *pOut)
{
	FS_RESULT nRet = 0;
	
	if(!bChanActive)
		return -1;

	if(pageHandle == NULL || bmpCache == NULL)
	{
		nLoge("renderCurrentPage pageHandle or bmpCache is NULL");
		return -1;
	}

	if(!pOut)
	{
		nLoge("renderCurrentPage pOut is NULL");
		return -1;
	}

	memset(FS_Bitmap_GetBuffer(bmpCache), 0xff, requestedWidth * requestedHeight * 4);

	FS_FLOAT pageWidth = 0;
	FS_FLOAT pageHeight = 0;

	nRet = FPDF_Page_GetSize(pageHandle, &pageWidth, &pageHeight);
	if(nRet)
	{
		nLoge("renderCurrentPage, FPDF_Page_GetSize return error = %d", nRet);
		return -1;
	}

	nLogi("renderCurrentPage, pageHandle(0x%X), pageStart(%.2f, %.2f), pageEnd(%.2f, %.2f)",
		pageHandle,
		((float)pageStartX) / 1000.0f,
		((float)pageStartY) / 1000.0f,
		((float)pageEndX) / 1000.0f,
		((float)pageEndY) / 1000.0f);

//	nLogi("renderCurrentPage, requestW-H(%d, %d)", requestedWidth, requestedHeight);
//	nLogi("renderCurrentPage, pageW-H(%.0f, %.0f)", pageWidth, pageHeight);

	float visibleWidth = pageWidth * (pageEndX - pageStartX) / 1000.0f;
	float visibleHeight = pageHeight * (pageEndY - pageStartY) / 1000.0f;
	float pageFactor = MIN((requestedWidth / visibleWidth), (requestedHeight / visibleHeight));

//	nLogi("renderCurrentPage, visibleW-H(%.0f, %.0f), factor(%.2f)", visibleWidth, visibleHeight, pageFactor);

	int size_x = pageWidth * pageFactor;
	int size_y = pageHeight * pageFactor;

	int visible_size_x = visibleWidth * pageFactor;
	int visible_size_y = visibleHeight * pageFactor;

	int start_x = 0, start_y = 0, visible_start_x = 0, visible_end_x = 0, visible_start_y = 0, visible_end_y = 0;

	if(visible_size_x > requestedWidth) {
		visible_start_x = 0;
		visible_end_x = requestedWidth - 1;
	} else {
		visible_start_x = (requestedWidth - visible_size_x) / 2;
		visible_end_x = (requestedWidth + visible_size_x) / 2;
	}
	start_x = visible_start_x - (size_x * pageStartX / 1000.0f);

	if(visible_size_y > requestedHeight) {
		visible_start_y = 0;
		visible_end_y = requestedHeight - 1;
	} else {
		visible_start_y = (requestedHeight - visible_size_y) / 2;
		visible_end_y = (requestedHeight + visible_size_y) / 2;
	}
	start_y = visible_start_y - (size_y * pageStartY / 1000.0f);

//	nLogi("renderCurrentPage, start x-y (%d, %d)", start_x, start_y);

	pthread_mutex_lock(&foxitRenderLock);
	nRet = FPDF_RenderPage_Start(bmpCache, pageHandle, 
			start_x,
			start_y,
			size_x,
			size_y,
			0, 0, NULL, NULL);
	pthread_mutex_unlock(&foxitRenderLock);

	if(nRet)
	{
		nLoge("renderCurrentPage, FPDF_RenderPage_Start return error = %d", nRet);
		return -1;
	}

	nRet = addBlackEdgeToOriginRGB((int)bmpCache, visible_start_x, visible_end_x, visible_start_y, visible_end_y);

#if 0
	FILE *fp = fopen("/sdcard/myrgb.rgb", "ab");
	fwrite(FS_Bitmap_GetBuffer(bmpCache), 1, requestedWidth * requestedHeight * 4, fp);
	fclose(fp);
#endif

	unsigned int startTime = RealTimeMilliSeconds();
#if USE_NEON_RGBA2YUV
	nRet = rgba2yuv_16x16_aligned_neon(requestedWidth, requestedHeight,
			(unsigned char*)FS_Bitmap_GetBuffer(bmpCache), (unsigned char*)pOut);
#else
	nRet = rgba2yuv(requestedWidth, requestedHeight, FS_Bitmap_GetBuffer(bmpCache), pOut);
#endif
	unsigned int endTime = RealTimeMilliSeconds();

	nLogi("rgba2yuv convert Time: %d, width(%d), height(%d)", endTime - startTime, requestedWidth, requestedHeight);

#if 0
	FILE *fp = fopen("/sdcard/myyuv.yuv", "ab");
	fwrite(pOut, 1, requestedWidth * requestedHeight * 3 / 2, fp);
	fclose(fp);
#endif

	return 0;
}

int addBlackEdgeToOriginRGB(int dib, int leftValid, int rightValid, int topValid, int bottomValid)
{
	if(!dib)
	{
		nLoge("addBlackEdgeToOriginRGB, dib is null");
		return -1;
	}

	FS_BITMAP bitmap = (FS_BITMAP)dib;
	int stride = FS_Bitmap_GetStride(bitmap);

	if(stride == 0)
	{
		nLoge("addBlackEdgeToOriginRGB, stride is 0");
		return -2;
	}

	int width = FS_Bitmap_GetWidth(bitmap);
	int height = FS_Bitmap_GetHeight(bitmap);
	char* buffer = (char *)FS_Bitmap_GetBuffer(bitmap);

	nLogi("addBlackEdgeToOriginRGB, width(%d), height(%d), inBuffer(%p)",
			width, height, buffer);

	nLogi("addBlackEdgeToOriginRGB, left(%d), right(%d), top(%d), bottom(%d)",
			leftValid, rightValid, topValid, bottomValid);

	leftValid = leftValid < 0 ? 0 : leftValid;
	rightValid = rightValid > (width - 1) ? (width - 1) : rightValid;
	topValid = topValid < 0 ? 0 : topValid;
	bottomValid = bottomValid > (height - 1) ? (height - 1) : bottomValid;

	// add top
	if(topValid != 0)
	{
		memset(buffer, 0, topValid * stride);
	}

	// add bottom
	if(bottomValid != height - 1)
	{
		memset(buffer + (bottomValid + 1) * stride, 0, (height - 1 - bottomValid) * stride);
	}

	// add left and right
	int leftAddBlack = (leftValid != 0);
	int rightAddBlack = (rightValid != width - 1);
	buffer += topValid * stride;
	for(int i = topValid; i <= bottomValid; i++, buffer += stride)
	{
		if(leftAddBlack)
		{
			memset(buffer, 0, leftValid * 4);
		}

		if(rightAddBlack)
		{
			memset(buffer + (rightValid + 1) * 4, 0, (width - 1 - rightValid) * 4);
		}
	}

	return 0;
}

unsigned int RealTimeMilliSeconds(void)
{
	struct timeval tm;

	gettimeofday(&tm, NULL);

	return (tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

#if USE_NEON_RGBA2YUV
int rgba2yuv_16x16_aligned_neon(int width, int height, unsigned char *rgba, unsigned char *yuv)
{
	static long long coeff[] = { 0x000001D3096404C8LL*8, 0x00000800054E02B1LL*8, 0x0000014D06B20800LL*8, 0x000000000C080400LL, 0x0080008000800080LL};
	int size = (width * height)>>3;

	__asm("sub sp, #320");
	__asm("str r5, [sp, #0]"); //rgba
	__asm("str r6, [sp, #4]"); //y
	__asm("str r7, [sp, #8]"); //u
	__asm("str r8, [sp, #12]"); //v
	__asm("str r9, [sp, #16]"); //size
	__asm("str r10, [sp, #20]"); //coeff
	__asm("str r11, [sp, #24]"); //temp

	__asm("add r11, sp, #32");
	__asm("vst1.8 {  d0,  d1}, [r11]!");
	__asm("vst1.8 {  d2,  d3}, [r11]!");
	__asm("vst1.8 {  d4,  d5}, [r11]!");
	__asm("vst1.8 {  d6,  d7}, [r11]!");
	__asm("vst1.8 {  d8,  d9}, [r11]!");
	__asm("vst1.8 { d10, d11}, [r11]!");
	__asm("vst1.8 { d12, d13}, [r11]!");
	__asm("vst1.8 { d14, d15}, [r11]!");
	__asm("vst1.8 { d16, d17}, [r11]!");
	__asm("vst1.8 { d18, d19}, [r11]!");
	__asm("vst1.8 { d20, d21}, [r11]!");
	__asm("vst1.8 { d22, d23}, [r11]!");
	__asm("vst1.8 { d24, d25}, [r11]!");
	__asm("vst1.8 { d26, d27}, [r11]!");
	__asm("vst1.8 { d28, d29}, [r11]!");
	__asm("vst1.8 { d30, d31}, [r11]!");
	__asm("add sp, #320");

	__asm("mov r5, %0" : :"r"(rgba) ) ;
	__asm("mov r6, %0" : :"r"(yuv) ) ;
	__asm("mov r9, %0" : :"r"(size) ) ;
	__asm("mov r10, %0" : :"r"(coeff) ) ;

	__asm("lsl r9, #3");
	__asm("add r7, r6, r9");
	__asm("lsr r9, #2");
	__asm("add r8, r7, r9");

	__asm("vld1.64 d4, [r10]!");
	__asm("vld1.64 d5, [r10]!");
	__asm("vld1.64 d6, [r10]!");
	__asm("vld1.64 d7, [r10]!");
	__asm("vld1.64 d8, [r10]");
	__asm("vld1.64 d9, [r10]");

	while(height > 0)
	{
		height -= 2;
		size = width;
		while(size > 0) {
			size -= 8;
			//first line
			__asm("vld4.8 {d0,d1,d2,d3}, [r5]!");

			__asm("vmovl.u8 q6, d0");
			__asm("vmovl.u8 q7, d1");
			__asm("vmovl.u8 q8, d2");

			__asm("vqdmulh.s16  q9, q6, d4[0]");
			__asm("vqdmulh.s16 q12, q6, d5[0]");
			__asm("vqdmulh.s16 q15, q6, d6[0]");
			__asm("vqdmulh.s16 q10, q7, d4[1]");
			__asm("vqdmulh.s16 q13, q7, d5[1]");
			__asm("vqdmulh.s16  q0, q7, d6[1]");
			__asm("vqdmulh.s16 q11, q8, d4[2]");
			__asm("vqdmulh.s16 q14, q8, d5[2]");
			__asm("vqdmulh.s16  q1, q8, d6[2]");

			__asm("vadd.s16 q9, q10");
			__asm("vadd.s16 q9, q11");
			__asm("vmovn.s16 d12, q9");
			__asm("vst1.32 {d12}, [r6]!");

			//uv
			__asm("vadd.s16 q14, q4");
			__asm("vsub.s16 q14, q12");
			__asm("vsub.s16 q14, q13");
			__asm("vtbx.8 d10, {d28,d29}, d7");
			__asm("vst1.32 {d10[0]}, [r7]!");

			__asm("vadd.s16 q15, q4");
			__asm("vsub.s16 q15, q0");
			__asm("vsub.s16 q15, q1");
			__asm("vtbx.8 d11, {d30,d31}, d7");
			__asm("vst1.32 {d11[0]}, [r8]!");
		}

		size = width;
		while(size > 0) {
			size -= 8;
			//next line
			__asm("vld4.8 {d0,d1,d2,d3}, [r5]!");

			__asm("vmovl.u8 q6, d0");
			__asm("vmovl.u8 q7, d1");
			__asm("vmovl.u8 q8, d2");

			__asm("vqdmulh.s16  q9, q6, d4[0]");
			__asm("vqdmulh.s16 q10, q7, d4[1]");
			__asm("vqdmulh.s16 q11, q8, d4[2]");

			__asm("vadd.s16 q9, q10");
			__asm("vadd.s16 q9, q11");
			__asm("vmovn.s16 d12, q9");
			__asm("vst1.32 {d12}, [r6]!");
		}
	}

	__asm("sub sp, #320");
	__asm("add r11, sp, #32");
	__asm("vld1.8 {  d0,  d1}, [r11]!");
	__asm("vld1.8 {  d2,  d3}, [r11]!");
	__asm("vld1.8 {  d4,  d5}, [r11]!");
	__asm("vld1.8 {  d6,  d7}, [r11]!");
	__asm("vld1.8 {  d8,  d9}, [r11]!");
	__asm("vld1.8 { d10, d11}, [r11]!");
	__asm("vld1.8 { d12, d13}, [r11]!");
	__asm("vld1.8 { d14, d15}, [r11]!");
	__asm("vld1.8 { d16, d17}, [r11]!");
	__asm("vld1.8 { d18, d19}, [r11]!");
	__asm("vld1.8 { d20, d21}, [r11]!");
	__asm("vld1.8 { d22, d23}, [r11]!");
	__asm("vld1.8 { d24, d25}, [r11]!");
	__asm("vld1.8 { d26, d27}, [r11]!");
	__asm("vld1.8 { d28, d29}, [r11]!");
	__asm("vld1.8 { d30, d31}, [r11]!");

	__asm("ldr r5, [sp, #0]");
	__asm("ldr r6, [sp, #4]");
	__asm("ldr r7, [sp, #8]");
	__asm("ldr r8, [sp, #12]");
	__asm("ldr r9, [sp, #16]");
	__asm("ldr r10, [sp, #20]");
	__asm("ldr r11, [sp, #24]");
	__asm("add sp, #320");
	return 0;
}

#else
static int32_t RGBYUV02990[256], RGBYUV05870[256], RGBYUV01140[256];
static int32_t RGBYUV01684[256], RGBYUV03316[256];
static int32_t RGBYUV04187[256], RGBYUV00813[256];

int rgba2yuv(int width, int height, void *rgb, void *yuv)
{
    int i, j, size;
    unsigned char *p_rgba;
    unsigned char r, g, b;
    unsigned char *y, *u, *v;

    size = width * height;

    // memory
    y = (unsigned char*)yuv;
    u = (unsigned char*)(y + size);
    v = (unsigned char*)(u + (size >> 2));

    p_rgba = (unsigned char *)rgb;

    // convert RGB to YUV
    for (i = 0; i < height; i++)
    {
    	for (j = 0; j < width; j++)
    	{
    		r = p_rgba[0];
    		g = p_rgba[1];
    		b = p_rgba[2];

            *y = (unsigned char)(( RGBYUV02990[r] + RGBYUV05870[g] + RGBYUV01140[b])>>16);
            y ++;

            if(i % 2 && j % 2)
            {
                *u = (unsigned char)(( RGBYUV01684[r] + RGBYUV03316[g] + ((int32_t)(b)<<15) + 0x800000)>>16);
                *v = (unsigned char)(( ((int32_t)(r)<<15) + RGBYUV04187[g] + RGBYUV00813[b] + 0x800000)>>16);
                u ++;
                v ++;
            }

            p_rgba += 4;
    	}
    }

    return 0;
}

void init_rgba2yuv_table()
{
    int i;

    for (i = 0; i < 256; i++) RGBYUV02990[i] = (int32_t)(0.2990 * i * 65536);
    for (i = 0; i < 256; i++) RGBYUV05870[i] = (int32_t)(0.5870 * i * 65536);
    for (i = 0; i < 256; i++) RGBYUV01140[i] = (int32_t)(0.1140 * i * 65536);
    for (i = 0; i < 256; i++) RGBYUV01684[i] = -(int32_t)(0.1684 * i * 65536);
    for (i = 0; i < 256; i++) RGBYUV03316[i] = -(int32_t)(0.3316 * i * 65536);
    for (i = 0; i < 256; i++) RGBYUV04187[i] = -(int32_t)(0.4187 * i * 65536);
    for (i = 0; i < 256; i++) RGBYUV00813[i] = -(int32_t)(0.0813 * i * 65536);
}
#endif

}
