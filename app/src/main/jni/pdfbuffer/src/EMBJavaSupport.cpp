#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <wchar.h>
#include <android/log.h>
#include <pthread.h>


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

#include "EMBJavaSupport.h"

#define FALSE 0
#define TRUE 1

#define LOG_TAG "EMBSDKJavaSupport"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

pthread_mutex_t foxitRenderLock;

void FX_OUTPUT_LOG_FUNC_V(const char* format, va_list argList)
{
	__android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG,format, argList);
}

extern "C" void FX_OUTPUT_LOG_FUNC(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	FX_OUTPUT_LOG_FUNC_V(format, argList);
	va_end(argList);
}
FS_PAUSE pPause;// = NULL;
FPDF_PAGE g_Page = NULL;
extern "C" int FX_GET_TICK_FUNC()
{
	return 0;
}

/***********************************************************************
*
*	Exception Throwing
*
***********************************************************************/

//C++ version
void throwExceptionHelper(JNIEnv *env, jclass obj, const char* exceptionName, const char* msg){
	jclass Exception = env->FindClass(exceptionName);
	if (Exception == NULL)
	/* Unable to find the exception class, give up. */
		return;
	env->ThrowNew(Exception, msg);
}

//add your own exception class here
void throwException(JNIEnv *env, jclass obj, int errorcode, const char* msg) {
	switch(errorcode) {
		case FS_ERR_MEMORY:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/memoryException", msg);
			break;
		case FS_ERR_ERROR:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/errorException", msg);
			break;
		case FS_ERR_PASSWORD:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/passwordException", msg);
			break;
		case FS_ERR_FORMAT:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/formatException", msg);
			break;
		case FS_ERR_FILE:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/fileAccessException", msg);
			break;
		case FS_ERR_PARAM:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/parameterException", msg);
			break;
		case FS_ERR_STATUS:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/passwordException", msg);
			break;
		case FS_ERR_TOBECONTINUED:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/toBeContinuedException", msg);
			break;
		case FS_ERR_NOTFOUND:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/searchNotFoundException", msg);
			break;
		case FS_ERR_UNPARSEDPAGE:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/unparsedPageException", msg);
			break;
		case FS_ERR_NOTEXTONPAGE:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/noTextOnPageException",msg);
			break;
		case FS_ERR_INVALID_LICENSE:
			throwExceptionHelper(env,obj,"com/polycom/cmad/pdf/exception/invalidLicenseException",msg);
		default:
			break;
	}
}

/***********************************************************************
*
*	global data
*
***********************************************************************/

FS_MEM_FIXEDMGR* pFixedMemmgr = NULL;
void* managedMemoryBlock = NULL;
FS_BOOL FixedMemoryInitialized = FALSE;

/***********************************************************************
*
*	user callback
*
***********************************************************************/
FS_BOOL FXMem_More(FS_LPVOID clientData, int alloc_size, void** FX_NEW_memory, int* FX_NEW_size)
{
	LOGI("FXMem_More");
	*FX_NEW_size = alloc_size;
	if (*FX_NEW_size < 1000000) *FX_NEW_size = 1000000;
	*FX_NEW_memory = malloc(*FX_NEW_size);
	return TRUE;
}
void FXMem_Free(FS_LPVOID clientData, void* memory)
{
	free(memory);
}

FS_DWORD FileGetSize(void* clientData){
	FILE* file = (FILE*)clientData;
	if (file && fseek(file, 0, SEEK_END) == 0){
		return ftell(file);
	}
	return 0;
}

FS_RESULT FileReadBlock(void* clientData, void* buffer, FS_DWORD offset, FS_DWORD size) {
	FILE* file = (FILE*)clientData;
	if (file && fseek(file, offset, SEEK_SET) == 0){
		if (fread(buffer, 1, size, file) == size){
			return FS_ERR_SUCCESS;
		}
	}
	return FS_ERR_ERROR;
}
	
void FileRelease(void* clientData) {
	return;
}	
static FS_RESULT FileFlush(FS_LPVOID clientData)
{
	LOGI("FileFlush begin :clientData = %x",clientData);
	FILE* pFile = (FILE*)clientData;
	int ret;

	if(pFile)
	{
		ret = fflush(pFile);
		if (ret == 0)
		{
			return FS_ERR_SUCCESS;
		}
	}
	else
	{
		return FS_ERR_PARAM;
	}
	LOGI("FileFlush end");
	return FS_ERR_ERROR;
}

static FS_RESULT FileWriterBlock(FS_LPVOID clientData, const void* buffer, FS_DWORD offset, FS_DWORD size)
{
	LOGI("FileWriterBlock begin:clientData = %x buffer = %x offset = %x size = %x",clientData,buffer,offset,size);
	FILE* pFile = (FILE*)clientData;
	int ret = FALSE;

	if(pFile)
	{
		//ret = fseek(pFile, offset, SEEK_SET);
		if(fwrite(buffer, sizeof(char), size, pFile) == size)
		{
			return FS_ERR_SUCCESS;
		}
	}
	else
	{
		return FS_ERR_PARAM;
	}
	LOGI("FileWriterBlock end");
	return FS_ERR_ERROR;

}  

FS_BOOL	NeedToPauseNow(void* clientData){
	return FALSE;
}

void OOMHandler(FS_LPVOID clientData,void* memory, int size){
	LOGI("Out of Memory!");
}	

class CPDF_Pause
{
public:
	static FS_BOOL	NeedToPauseNow(void* clientData)
	{
		return TRUE;	
	}

public:
	static JNIEnv*	m_pEnv;
	static jobject	m_objPauseInfo;
};

JNIEnv* CPDF_Pause::m_pEnv = NULL;
jobject CPDF_Pause::m_objPauseInfo = NULL;

class CPDF_FormFillerInfo
{
public:
	static void SetEnv(JNIEnv* pEnv, jobject object)
	{
		m_pEnv = pEnv;
		m_objFormFillerInfo = object;
	}

	static void SetNewThreadEnv(JNIEnv* pEnv)
	{
		m_pNewThreadEnv = pEnv;
	}

	static void		Release(FS_LPVOID pThis)
	{
		
	}

	static 	void 	ExecuteNamedAction(FS_LPVOID pThis, FS_LPCSTR namedAction)
	{

	}

	static	FPDF_PAGE	GetCurrentPage(FS_LPVOID pThis, FPDF_DOCUMENT document)
	{
		jclass clss = m_pEnv->GetObjectClass(m_objFormFillerInfo);
		jmethodID methodID = m_pEnv->GetMethodID(clss, "FFI_GetCurrentPage", "(I)I");
		if (methodID == NULL)
			return NULL;

		int nCurPageHandler = m_pEnv->CallIntMethod(m_objFormFillerInfo, methodID, (int)document);
		return (FPDF_PAGE)nCurPageHandler;
	}
	
	static FPDF_SYSTEMTIME GetLocalTime(FS_LPVOID pThis)
	{
		FPDF_SYSTEMTIME stime;
		return stime;
	}
	
	static FPDF_PAGE GetPage(FS_LPVOID pThis, FPDF_DOCUMENT document, FS_INT32 nPageIndex)
	{
		jclass clss = m_pEnv->GetObjectClass(m_objFormFillerInfo);
		jmethodID methodID = m_pEnv->GetMethodID(clss, "FFI_GetPage", "(II)I");
		if (methodID == NULL)
			return NULL;

		int nPageHandler = m_pEnv->CallIntMethod(m_objFormFillerInfo, methodID, (int)document, nPageIndex);
		return (FPDF_PAGE)nPageHandler;
	}

	static FS_INT32 GetRotation(FS_LPVOID pThis, FPDF_PAGE page)
	{
		return 0;
	}

	static void Invalidate(FS_LPVOID pThis,FPDF_PAGE page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom)
	{
		JNIEnv* pEnv = NULL;
		LOGI("%x", m_bNewThread);
		if (m_bNewThread != FALSE)
		{
			pEnv = m_pNewThreadEnv;
		}
		else
		{
			pEnv = m_pEnv;
		}
	
		jclass clss = pEnv->GetObjectClass(m_objFormFillerInfo);
		jmethodID methodID = pEnv->GetMethodID(clss, "FFI_Invalidate", "(IFFFF)V");
		if (methodID == NULL)
			return;
		
		pEnv->CallVoidMethod(m_objFormFillerInfo, methodID, (int)page, left, top, right, bottom);
		m_bNewThread = FALSE;
	}
	
	static void KillTimer(FS_LPVOID pThis, FS_INT32 nTimerID)
	{
		jclass clss = m_pEnv->GetObjectClass(m_objFormFillerInfo);
		jmethodID methodID = m_pEnv->GetMethodID(clss, "FFI_KillTimer", "(I)V");
		if (methodID == NULL)
			return;
		
		m_pEnv->CallVoidMethod(m_objFormFillerInfo, methodID, nTimerID);
	}

	static void OnChange(FS_LPVOID pThis)
	{

	}
	
	static void OnSetFieldInputFocus(FS_LPVOID clientData,FPDF_FORMFIELD field, FS_LPCWSTR focusText, FS_DWORD nTextLen)
	{
		jclass clss = m_pEnv->GetObjectClass(m_objFormFillerInfo);
		jmethodID methodID = m_pEnv->GetMethodID(clss, "FFI_OnSetFieldInputFocus", "(ILjava/lang/String;I)V");
		if (methodID == NULL)
			return;
		
		jchar* text = (jchar*)malloc(nTextLen+1);
		int i;
		for (i=0; i<nTextLen; i++)
			text[i] = focusText[i];
		text[nTextLen] = 0;
		
		jstring jText = m_pEnv->NewString(text, nTextLen);   
		//free((void*)text);
		
		m_pEnv->CallVoidMethod(m_objFormFillerInfo, methodID, field, jText, nTextLen);
		
	}

	static void OutputSelectedRect(FS_LPVOID pThis,FPDF_PAGE page, double left, double top, double right, double bottom)
	{

	}

	static void SetCursor(FS_LPVOID pThis, FS_INT32 nCursorType)
	{

	}

	static FS_INT32 SetTimer(FS_LPVOID pThis, FS_INT32 uElapse, FS_LPFTimerCallback lpTimerFunc)
	{
		jclass clss = m_pEnv->GetObjectClass(m_objFormFillerInfo);
		jmethodID methodID = m_pEnv->GetMethodID(clss, "FFI_SetTimer", "(II)I");
		if (methodID == NULL)
			return 2;
			//return FoxitEMBSDK_EMBJavaSupport_TIMER_EVENT_ID;

		int eventID = m_pEnv->CallIntMethod(m_objFormFillerInfo, methodID, uElapse, (int)lpTimerFunc);
		return eventID;
	}

public:
	static JNIEnv*	m_pEnv;
	static JNIEnv*  m_pNewThreadEnv;
	static FS_BOOL	m_bNewThread;
	static jobject	m_objFormFillerInfo;
};

JNIEnv* CPDF_FormFillerInfo::m_pEnv = NULL;
jobject CPDF_FormFillerInfo::m_objFormFillerInfo = NULL;
FS_BOOL CPDF_FormFillerInfo::m_bNewThread = FALSE;
JNIEnv* CPDF_FormFillerInfo::m_pNewThreadEnv = NULL;

class CPDF_JsPlatform
{
public:
	static void SetEnv(JNIEnv* pEnv, jobject object)
	{
		m_pEnv = pEnv;
		m_objJsPlatform = object;
	}
	
	static	FS_INT32 alert(FS_LPVOID clientData, FS_LPCWSTR Msg, FS_LPCWSTR Title, FS_INT32 Type, FS_INT32 Icon)
	{
		return 0;
	}

	static void beep(FS_LPVOID clientData, FS_INT32 nType)
	{

	}
	
	static	FS_INT32 response(FS_LPVOID clientData, FS_LPCWSTR Question, FS_LPCWSTR Title, FS_LPCWSTR Default, 
							 FS_LPCWSTR cLabel, FS_BOOL bPassword, FS_LPVOID response, FS_DWORD length)
	{
		return 0;
	}

	static FS_INT32 getFilePath(FS_LPVOID clientData, FS_LPVOID filePath, FS_DWORD length)
	{
		return 0;
	}

	static void mail(FS_LPVOID clientData, FS_LPVOID mailData, FS_DWORD length, FS_BOOL bUI, FS_LPCWSTR To, 
					 FS_LPCWSTR Subject, FS_LPCWSTR CC, FS_LPCWSTR BCC, FS_LPCWSTR Msg)
	{

	}

	static void print(FS_LPVOID clientData, FS_BOOL bUI, FS_INT32 nStart, FS_INT32 nEnd, FS_BOOL bSilent ,
					  FS_BOOL bShrinkToFit, FS_BOOL bPrintAsImage , FS_BOOL bReverse , FS_BOOL bAnnotations)
	{

	}

	static	void submitForm(FS_LPVOID clientData, FS_LPVOID formData, FS_DWORD length, FS_LPCWSTR URL)
	{

	}

	static FS_INT32  browse(FS_LPVOID clientData, FS_LPVOID filePath, FS_DWORD length)
	{
		return 1;
	}

public:
	static JNIEnv*		m_pEnv;
	static jobject		m_objJsPlatform;
};

JNIEnv* CPDF_JsPlatform::m_pEnv = NULL;
jobject CPDF_JsPlatform::m_objJsPlatform = NULL;


class CPDF_PSI
{
public:
	static void SetEnv(JNIEnv* pEnv)
	{
		m_pEnv = pEnv;
		LOGD("FPSI_Invalidate8 %i",m_pEnv);
	}
	
	static void SetObject(jobject object)
	{
		m_jPSIObj = object;
		LOGD("FPSI_Invalidate7 %i",m_jPSIObj);
	}
	
	static 	void	FPSI_Invalidate(void* param, FS_INT32 left, FS_INT32 top, FS_INT32 right, FS_INT32 bottom)
	{
		LOGD("FPSI_Invalidate1 PSIObj: %i, env:%i, left:%i, top:%i, right%i, bottom%i",m_jPSIObj,m_pEnv,left, top, right, bottom);
		jclass clss = m_pEnv->GetObjectClass(m_jPSIObj);
		jmethodID methodID = m_pEnv->GetMethodID(clss, "FPSI_Invalidate", "(IIII)V");
		if (methodID == NULL)
		{
			LOGD("FPSI_Invalidate6");
			return;
		}
		
		m_pEnv->CallVoidMethod(m_jPSIObj, methodID, left, top, right, bottom);
		
	}
	
	static FS_FLOAT	FPSI_GetOpacity(void* param)
	{
		
		jclass clss = m_pEnv->GetObjectClass(m_jPSIObj);
		jmethodID methodID = m_pEnv->GetMethodID(clss, "FPSI_GetOpacity", "()F");
		LOGD("FPSI_Invalidate4 m_jPSIObj:%i,param:%i",m_jPSIObj,param);
		if (methodID == NULL) return 1.0f;
		LOGD("FPSI_Invalidate5");
		return m_pEnv->CallFloatMethod(m_jPSIObj, methodID);
	}
	
public:
	static JNIEnv*	m_pEnv;
	static jobject	m_jPSIObj;
};

JNIEnv* CPDF_PSI::m_pEnv = NULL;
jobject CPDF_PSI::m_jPSIObj = NULL;

/***********************************************************************
*
*	Document module
*
***********************************************************************/

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFDocLoad
  (JNIEnv *pEnv, jclass cls, jint fileRead, jstring password)
{
	const char* lpszPassword = NULL;
	if (!password)
		lpszPassword = pEnv->GetStringUTFChars(password, NULL);
	
	FS_FILEREAD* pFileRead = NULL;
	pFileRead = (FS_FILEREAD*)fileRead;
	if (!pFileRead)
		throwException(pEnv,cls,FS_ERR_PARAM,"FPDFDocLoad: fail to read file");

	pthread_mutex_init(&foxitRenderLock, NULL);

	FPDF_DOCUMENT pPDFDoc = NULL;	
	FS_RESULT ret = FPDF_Doc_Load(pFileRead, lpszPassword, &pPDFDoc);
	LOGI("memcheck--FPDFDocLoad, doc: %x", pPDFDoc);
	LOGI("DocLoad = %x", ret);
	if(ret)
		throwException(pEnv,cls,ret,"FPDFDocLoad: Doc did not load properly");
	return (jint)pPDFDoc;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFDocClose
  (JNIEnv *pEnv, jclass cls, jint document)
{
	pthread_mutex_destroy(&foxitRenderLock);

	LOGI("memcheck--FPDFDocClose, doc: %x", document);

	FS_RESULT ret = FPDF_Doc_Close((FPDF_DOCUMENT)document);
	if(ret)
		throwException(pEnv,cls,ret,"FPDFDocClose: fail to close doc");
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFDocSaveAs
  (JNIEnv *pEnv, jclass cls, jint document, jint flag, jint pause,jint filewrite)
{
	FPDF_DOCUMENT pPDFDoc = (FPDF_DOCUMENT)document;
	FS_PAUSE* pPause = (FS_PAUSE*)pause;
	FS_FILEWRITE* pfileWrite = (FS_FILEWRITE*)filewrite;
	FS_RESULT ret = FPDF_Doc_SaveAs(pPDFDoc, pfileWrite, flag, pPause);
	LOGI("ret = %x",ret);
	if(ret!=FS_ERR_SUCCESS)
	{
		throwException(pEnv,cls,ret,"FPDFDocSaveAs: failed to save doc.");
	}
}



JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFDocGetPageCount
  (JNIEnv *pEnv, jclass cls, jint document)
{
	FS_INT32 count = 0;
	FPDF_Doc_GetPageCount((FPDF_DOCUMENT)document,&count);
	if(count==0)//going to fix this...
		throwException(pEnv,cls,FS_ERR_PARAM,"FPDFDocGetPageCount: invalid parameter. Null pointer?");
	else if(count==-1)
		throwException(pEnv,cls,FS_ERR_MEMORY,"FPDFDocGetPageCount: out of memory.");

	return (jint)count;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageLoad
  (JNIEnv *pEnv, jclass cls, jint document, jint index)
{
	FPDF_PAGE pPage = NULL;
	FS_RESULT nRet = FPDF_Page_Load((FPDF_DOCUMENT)document, index, &pPage);
	if(nRet)
		throwException(pEnv,cls,nRet,"FPDFPageLoad: Page load failed");

	LOGI("memcheck--FPDFPageLoad, page: %x", pPage);
	return (jint)pPage;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageClose
  (JNIEnv *pEnv, jclass cls, jint page)
{
	LOGI("memcheck--FPDFPageClose, page: %x", page);
	int ret=2;
	if(ret = FPDF_Page_Close((FPDF_PAGE)page))
		throwException(pEnv,cls,ret,"FPDFPageClose: Invalid parameter");
}

JNIEXPORT int JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageStartParse
  (JNIEnv *pEnv, jclass cls, jint page, jint text_only, jint pauseHandler)
{
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	if (!pPage)
		throwException(pEnv,cls,FS_ERR_PARAM,"FPDFPageStartParse: Invalid parameters");
	
	FS_PAUSE* pPause = (FS_PAUSE*)pauseHandler;
	pthread_mutex_lock(&foxitRenderLock);
	FS_RESULT ret = FPDF_Page_StartParse(pPage, text_only, pPause);
	pthread_mutex_unlock(&foxitRenderLock);

	return (jint)ret;
	LOGI("FPDFPageStartParse %i",ret);
}

JNIEXPORT int JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageContinueParse
  (JNIEnv *pEnv, jclass cls, jint page, jint pauseHandler)
{
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	if (!pPage)
		throwException(pEnv,cls,FS_ERR_PARAM,"FPDFPageContinueParse: Invalid parameters");
	
	FS_PAUSE* pPause = (FS_PAUSE*)pauseHandler;
	FS_RESULT ret = FPDF_Page_ContinueParse(pPage, pPause);
	
	return (jint)ret;
}

JNIEXPORT jfloat JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageGetSizeX
  (JNIEnv *pEnv, jclass cls, jint pageHandle)
{
  	FS_FLOAT x,y;
	FS_RESULT ret;
	ret=FPDF_Page_GetSize((FPDF_PAGE)pageHandle, &x, &y);
	if(ret)
		throwException(pEnv,cls,ret,"FPDFPageGetSizeX: GetPageSizeX error");
	return (jfloat)x;
}

JNIEXPORT jfloat JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageGetSizeY
  (JNIEnv *pEnv, jclass cls, jint pageHandle)
{
  	FS_FLOAT x,y;
	FS_RESULT ret;
	ret=FPDF_Page_GetSize((FPDF_PAGE)pageHandle, &x, &y);
	if(ret)
		throwException(pEnv,cls,ret,"FPDFPageGetSizeY: GetPageSizeY error");
	return (jfloat)y;
}

/***********************************************************************
*
*	Text module
*
***********************************************************************/
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextLoadPage
  (JNIEnv *pEnv, jclass cls, jint page)
  {
	LOGI("FoxitEMBSDK_EMBJavaSupport.cpp FPDFTextLoadPage");
	FPDF_TEXTPAGE textpage = NULL;
	FS_RESULT ret;
	if(ret = FPDF_Text_LoadPage((FPDF_PAGE)page, &textpage))
		throwException(pEnv,cls,ret,"FPDFTextLoadPage: FPDFTextLoadPage did not return success");
	return (jint)textpage;
  }

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextCloseTextPage
  (JNIEnv * pEnv, jclass cls, jint page)
  {
	FS_RESULT ret;
	if(ret = FPDF_Text_CloseTextPage((FPDF_TEXTPAGE)page))
		throwException(pEnv,cls,ret,"FPDFTextCloseTextPage: FPDFTextCloseTextPage did not return success");
  }


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextCountChars
  (JNIEnv * pEnv, jclass cls, jint pageHandle)
  {
	FS_INT32 count = 0;
	FS_RESULT ret;
	if(ret=FPDF_Text_CountChars((FPDF_TEXTPAGE)pageHandle, &count))
		throwException(pEnv,cls,ret,"FPDFTextCountChar: FPDFTextCountChars did not return success");
	return count;
  }

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextCountRects
  (JNIEnv *pEnv, jclass cls, jint textPage, jint start, jint count)
{
	FPDF_TEXTPAGE pTextPage = (FPDF_TEXTPAGE)textPage;
	FS_INT32 rectCount = 0;
	FS_RESULT ret = FPDF_Text_CountRects(pTextPage,start,count,&rectCount);
	if(ret)
		throwException(pEnv,cls,ret,"FPDFTextCountRects: FPDFTextCountRects did not return success");
	return rectCount;

}
JNIEXPORT jobject JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextGetRect
  (JNIEnv * pEnv, jclass cls, jint textpage, jint index)
  {
	FS_RECTF rect;
	FS_RESULT ret;
	if(ret = FPDF_Text_GetRect((FPDF_TEXTPAGE)textpage, index, &rect))
		throwException(pEnv,cls,ret,"FPDFTextGetRect: FPDFTextGetRect did not return success");
	
	jclass cls_r = pEnv->FindClass((const char*)"FoxitEMBSDK/EMBJavaSupport$RectangleF");
	jobject obj = pEnv->AllocObject(cls_r);
	jfieldID left = pEnv->GetFieldID( cls_r, (const char*)"left", "F");
	jfieldID right = pEnv->GetFieldID(cls_r, (const char*)"right", "F");
	jfieldID top = pEnv->GetFieldID(cls_r, (const char*)"top", "F");
	jfieldID bottom = pEnv->GetFieldID( cls_r, (const char*)"bottom", "F");

	pEnv->SetFloatField( obj, left, rect.left);
	pEnv->SetFloatField( obj, right, rect.right);
	pEnv->SetFloatField( obj, top, rect.top);
	pEnv->SetFloatField( obj, bottom, rect.bottom);
	return obj;
  }


JNIEXPORT jstring JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextGetText
  (JNIEnv * pEnv, jclass cls, jint textpage, jint nStart, jint nCount)
{
	FS_DWORD bufflen = 0;
	FS_RESULT ret;
	if(ret = FPDF_Text_GetText((FPDF_TEXTPAGE)textpage, nStart, nCount, NULL, &bufflen))
		throwException(pEnv,cls,ret,"FPDFTextGetText: FPDFTextGetText did not return success");
	FS_WCHAR* pBuff = new FS_WCHAR[bufflen+1];
	pBuff[bufflen] = 0;
	if(ret = FPDF_Text_GetText((FPDF_TEXTPAGE)textpage, nStart, nCount, pBuff, &bufflen))
		throwException(pEnv,cls,ret,"FPDFTextGetText: FPDFTextGetText did not return success");
	return pEnv->NewString( pBuff, bufflen);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextFindStart
(JNIEnv *pEnv, jclass cls, jint textpage, jstring findwhat, jlong flag, jint startindex)
{
	int   length   =   pEnv-> GetStringLength(findwhat); 
	const FS_WCHAR* wcFind   =   pEnv-> GetStringChars(findwhat,   0); 
	FPDF_TEXTPAGE pTextPage = (FPDF_TEXTPAGE)textpage;
	FPDF_SCHHANDLE searchHandle = NULL;
	LOGI("wcFind is %x %x %x %x",wcFind[0],wcFind[1],wcFind[2],wcFind[3]);
	FS_RESULT ret = FPDF_Text_FindStart(pTextPage,(FS_WCHAR*)wcFind, flag, startindex, &searchHandle);
	if(ret != 0) throwException(pEnv,cls,ret,"FPDFTextFindStart: FPDFTextFindStart did not return success");
	return (jint)searchHandle;
	
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextFindNext
(JNIEnv *pEnv, jclass cls, jint searchHandle)
{
	FPDF_SCHHANDLE pSearchHandle = (FPDF_SCHHANDLE)searchHandle;
	FS_BOOL isMatch = FALSE;
	FS_RESULT ret = FPDF_Text_FindNext(pSearchHandle, &isMatch);
	LOGI("FPDF_Text_FindNext return is %x,Match is %x",ret,isMatch);
	if(ret != 0) throwException(pEnv,cls,ret,"FPDFTextFindNext: FPDFTextFindNext did not return success");
	return isMatch;
	
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextFindPrev
  (JNIEnv *pEnv, jclass cls, jint searchHandle)
{
	FPDF_SCHHANDLE pSearchHandle = (FPDF_SCHHANDLE)searchHandle;
	FS_BOOL isMatch = FALSE;
	FS_RESULT ret = FPDF_Text_FindPrev(pSearchHandle, &isMatch);
	if(ret != 0) throwException(pEnv,cls,ret,"FPDFTextFindPrev: FPDFTextFindPrev did not return success");
	return isMatch;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextGetSchResultIndex
  (JNIEnv *pEnv, jclass cls, jint searchHandle)
{	
	FPDF_SCHHANDLE pSearchHandle = (FPDF_SCHHANDLE)searchHandle;
	FS_INT32 index = -1;
	FS_RESULT ret = FPDF_Text_GetSchResultIndex(pSearchHandle, &index);
	if(ret != 0)  throwException(pEnv,cls,ret,"FPDFTextGetSchResultIndex: FPDFTextGetSchResultIndex did not return success");
	return index;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextGetSchCount
  (JNIEnv *pEnv, jclass cls, jint searchHandle)
{
	FPDF_SCHHANDLE pSearchHandle = (FPDF_SCHHANDLE)searchHandle;
	FS_INT32 count = -1;
	FS_RESULT ret = FPDF_Text_GetSchCount(pSearchHandle ,&count);
	if(ret != 0) throwException(pEnv,cls,ret,"FPDFTextGetSchCount: FPDFTextGetSchCount did not return success");
	return count;
}	

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFTextFindClose
  (JNIEnv *pEnv, jclass cls, jint searchHandle)
{
	FPDF_SCHHANDLE pSearchHandle = (FPDF_SCHHANDLE)searchHandle;
	FS_RESULT ret = FPDF_Text_FindClose(pSearchHandle);
	if(ret != 0) throwException(pEnv,cls,ret,"FPDFTextFindClose: FPDFTextFindClose did not return success");
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPagePageToDevicePoint
  (JNIEnv *pEnv, jclass cls, jint page, jint start_x, jint start_y, jint size_x, jint size_y, jint rotate, jobject point)
{
	if(page == 0) return ;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	jclass clsPoint = pEnv->GetObjectClass(point);
	if(clsPoint == NULL) return;
	jfieldID idX = pEnv->GetFieldID(clsPoint, "x", "I");
	jfieldID idY = pEnv->GetFieldID(clsPoint, "y", "I");
	int x = pEnv->GetIntField(point,idX);
	int y = pEnv->GetIntField(point,idY);
	FS_POINT FX_Point;
	FX_Point.x = x;
	FX_Point.y = y;
//	LOGI("FX_Point.x = %x,FX_Point.y = %x ",x,y);
//	LOGI("start_x = %x, start_y = %x size_x = %x size_y = %x",start_x,start_y,size_x,size_y);
	FPDF_Page_PageToDevicePoint(pPage,start_x, start_y, size_x, size_y, rotate, &FX_Point);
//	LOGI("FX_Point.x = %x,FX_Point.y = %x ",FX_Point.x,FX_Point.y);
	idX = pEnv->GetFieldID(clsPoint, "x", "I");
	idY = pEnv->GetFieldID(clsPoint, "y", "I");
	pEnv->SetIntField(point, idX, (jint)FX_Point.x);
	pEnv->SetIntField(point, idY, (jint)FX_Point.y);
//	idX = pEnv->GetFieldID(clsPoint, "x", "I");
//	idY = pEnv->GetFieldID(clsPoint, "y", "I");
//	x = pEnv->GetIntField(point,idX);
//	y = pEnv->GetIntField(point,idY);
//	LOGI("Point.x = %x,Point.y = %x ",x,y);

}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPagePageToDevicePointF
(JNIEnv *pEnv, jclass cls, jint page, jint start_x, jint start_y, jint size_x, jint size_y, jint rotate, jobject point)
{
	if(page == 0) return ;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	jclass clsPoint = pEnv->GetObjectClass(point);
	if(clsPoint == NULL) return;
	jfieldID idX = pEnv->GetFieldID(clsPoint, "x", "F");
	jfieldID idY = pEnv->GetFieldID(clsPoint, "y", "F");
	float x = pEnv->GetFloatField(point,idX);
	float y = pEnv->GetFloatField(point,idY);
	FS_POINTF FX_Point;
	FX_Point.x = x;
	FX_Point.y = y;
	//	LOGI("FX_Point.x = %x,FX_Point.y = %x ",x,y);
	//	LOGI("start_x = %x, start_y = %x size_x = %x size_y = %x",start_x,start_y,size_x,size_y);
	FPDF_Page_PageToDevicePointF(pPage,start_x, start_y, size_x, size_y, rotate, &FX_Point);
	//	LOGI("FX_Point.x = %x,FX_Point.y = %x ",FX_Point.x,FX_Point.y);
	idX = pEnv->GetFieldID(clsPoint, "x", "F");
	idY = pEnv->GetFieldID(clsPoint, "y", "F");
	pEnv->SetFloatField(point, idX, FX_Point.x);
	pEnv->SetFloatField(point, idY, FX_Point.y);
	//	idX = pEnv->GetFieldID(clsPoint, "x", "I");
	//	idY = pEnv->GetFieldID(clsPoint, "y", "I");
	//	x = pEnv->GetIntField(point,idX);
	//	y = pEnv->GetIntField(point,idY);
	//	LOGI("Point.x = %x,Point.y = %x ",x,y);
	
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPagePageToDeviceRect
 (JNIEnv *pEnv, jclass clss, jint page, jint start_x, jint start_y, jint size_x, jint size_y, jint rotate, jobject rect)
{
	if (page == 0) 
		throwException(pEnv,clss,FS_ERR_PARAM,"FPDFPagePageToDeviceRect: Page is NULL");
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	
	if (!rect) 
		throwException(pEnv,clss,FS_ERR_PARAM,"FPDFPagePageToDeviceRect: Rect is NULL");
	FS_RECT deviceRect;
	jclass cls = pEnv->GetObjectClass(rect);
	jfieldID idA = pEnv->GetFieldID(cls, "left", "I");
	jfieldID idB = pEnv->GetFieldID(cls, "top", "I");
	jfieldID idC = pEnv->GetFieldID(cls, "right", "I");
	jfieldID idD = pEnv->GetFieldID(cls, "bottom", "I");
	
	deviceRect.left = pEnv->GetIntField(rect, idA);
	deviceRect.top = pEnv->GetIntField(rect, idB);
	deviceRect.right = pEnv->GetIntField(rect, idC);
	deviceRect.bottom = pEnv->GetIntField(rect, idD);
	FPDF_Page_PageToDeviceRect(pPage, start_x, start_y, size_x, size_y, rotate, &deviceRect);
	pEnv->SetIntField(rect, idA, deviceRect.left);
	pEnv->SetIntField(rect, idB, deviceRect.top);
	pEnv->SetIntField(rect, idC, deviceRect.right);
	pEnv->SetIntField(rect, idD, deviceRect.bottom);
	
	return (jint)FS_ERR_SUCCESS;
}

/***********************************************************************
*
*	View module
*
***********************************************************************/

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFRenderPageStart
  (JNIEnv *pEnv, jclass cls, jint dib, jint page, jint start_x, jint start_y, jint size_x, jint size_y, jint rotate, jint flags, jobject clip, jint pauseHandler)
{
	FPDF_PAGE pPDFPage = (FPDF_PAGE)page;
	if (!pPDFPage)
		throwException(pEnv,cls,FS_ERR_PARAM,"FPDFRenderPageStart: null page pointer");
	
	FS_BITMAP bitmap = (FS_BITMAP)dib;
	if (!bitmap)
		throwException(pEnv,cls,FS_ERR_PARAM,"FPDFRenderPageStart: null dib pointer");
	FS_PAUSE* pPause = (FS_PAUSE*)pauseHandler;
//	pPause.FSPause_NeedPauseNow = CPDF_Pause::NeedToPauseNow;
	//LOGCQ("pPause->clientData=%1$d",pPause->clientData);
	//LOGCQ("pPause->FSPause_NeedPauseNow=%1$x",pPause->FSPause_NeedPauseNow);
	FS_RESULT ret;
	if (clip != NULL) 
	{
		FS_RECT clipRect;
		jclass cls = pEnv->GetObjectClass(clip);
		jfieldID idA = pEnv->GetFieldID(cls, "left", "I");
		jfieldID idB = pEnv->GetFieldID(cls, "top", "I");
		jfieldID idC = pEnv->GetFieldID(cls, "right", "I");
		jfieldID idD = pEnv->GetFieldID(cls, "bottom", "I");
		
		clipRect.left = pEnv->GetIntField(clip, idA);
		clipRect.top = pEnv->GetIntField(clip, idB);
		clipRect.right = pEnv->GetIntField(clip, idC);
		clipRect.bottom = pEnv->GetIntField(clip, idD);
		pthread_mutex_lock(&foxitRenderLock);
		FPDF_RenderPage_Start(bitmap, pPDFPage, start_x, start_y, size_x, size_y, rotate, flags, &clipRect, pPause);
		pthread_mutex_unlock(&foxitRenderLock);
	}
	else 
	{
		pthread_mutex_lock(&foxitRenderLock);
		ret = FPDF_RenderPage_Start(bitmap, pPDFPage, start_x, start_y, size_x, size_y, rotate, flags, NULL, pPause);
		pthread_mutex_unlock(&foxitRenderLock);
	}
	
	return (jint)ret;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFRenderPageContinue
  (JNIEnv *pEnv, jclass cls, jint page, jint pauseHandler)
{
	FPDF_PAGE pPDFPage = (FPDF_PAGE)page;
	if (!pPDFPage)
		throwException(pEnv,cls,FS_ERR_PARAM,"FPDFRenderPageContinue: null page pointer");


	FS_PAUSE* pPause = (FS_PAUSE*)pauseHandler;
	FS_RESULT ret = FPDF_RenderPage_Continue(pPDFPage, pPause);

	return (jint)ret;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageDeviceToPageRect
  (JNIEnv *pEnv, jclass clss, jint page, jint start_x, jint start_y, jint size_x, jint size_y, jint rotate, jobject rect)
{
	if (page == 0) throwException(pEnv,clss,FS_ERR_PARAM,"FPDFPageDeviceToPageRect: null page pointer");
	FPDF_PAGE pPage = (FPDF_PAGE)page;

	if (!rect) throwException(pEnv,clss,FS_ERR_PARAM,"FPDFPageDeviceToPageRect: invalid rect object");
	FS_RECT deviceRect;
	jclass cls = pEnv->GetObjectClass(rect);
	jfieldID idA = pEnv->GetFieldID(cls, "left", "I");
	jfieldID idB = pEnv->GetFieldID(cls, "top", "I");
	jfieldID idC = pEnv->GetFieldID(cls, "right", "I");
	jfieldID idD = pEnv->GetFieldID(cls, "bottom", "I");

	deviceRect.left = pEnv->GetIntField(rect, idA);
	deviceRect.top = pEnv->GetIntField(rect, idB);
	deviceRect.right = pEnv->GetIntField(rect, idC);
	deviceRect.bottom = pEnv->GetIntField(rect, idD);

	FPDF_Page_DeviceToPageRect(pPage, start_x, start_y, size_x, size_y, rotate, &deviceRect);

	pEnv->SetIntField(rect, idA, deviceRect.left);
	pEnv->SetIntField(rect, idB, deviceRect.top);
	pEnv->SetIntField(rect, idC, deviceRect.right);
	pEnv->SetIntField(rect, idD, deviceRect.bottom);

	return (jint)FS_ERR_SUCCESS;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFPageDeviceToPageRectF
 * Signature: (IIIIIILFoxitEMBSDK/EMBJavaSupport/RectangleF;)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageDeviceToPageRectF
  (JNIEnv *pEnv, jclass cls, jint page, jint start_x, jint start_y, jint size_x, jint size_y, jint rotate, jobject rectf)
{
	if (page == 0) throwException(pEnv,cls,FS_ERR_PARAM,"FPDFPageDeviceToPageRectF: null page pointer");
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	
	if (!rectf) throwException(pEnv,cls,FS_ERR_PARAM,"FPDFPageDeviceToPageRectF: invalid rect object");
	FS_RECTF deviceRect;
	jclass clss = pEnv->GetObjectClass(rectf);
	jfieldID idA = pEnv->GetFieldID(clss, "left", "F");
	jfieldID idB = pEnv->GetFieldID(clss, "top", "F");
	jfieldID idC = pEnv->GetFieldID(clss, "right", "F");
	jfieldID idD = pEnv->GetFieldID(clss, "bottom", "F");
	
	deviceRect.left = pEnv->GetFloatField(rectf, idA);
	deviceRect.top = pEnv->GetFloatField(rectf, idB);
	deviceRect.right = pEnv->GetFloatField(rectf, idC);
	deviceRect.bottom = pEnv->GetFloatField(rectf, idD);
	
	FPDF_Page_DeviceToPageRectF(pPage, start_x, start_y, size_x, size_y, rotate, &deviceRect);
	
	pEnv->SetFloatField(rectf, idA, deviceRect.left);
	pEnv->SetFloatField(rectf, idB, deviceRect.top);
	pEnv->SetFloatField(rectf, idC, deviceRect.right);
	pEnv->SetFloatField(rectf, idD, deviceRect.bottom);
	
	return (jint)FS_ERR_SUCCESS;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageDeviceToPagePointF
  (JNIEnv *pEnv, jclass cls, jint page, jint start_x, jint start_y, jint size_x, jint size_y, jint rotate, jobject point)
{
	if (page == 0 || point == NULL)
		return;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	jclass clss = pEnv->GetObjectClass(point);
	FS_POINTF devicePoint;
	
	jfieldID idx = pEnv->GetFieldID(clss, "x", "F");
	jfieldID idy = pEnv->GetFieldID(clss, "y", "F");

	devicePoint.x = pEnv->GetFloatField(point, idx);
	devicePoint.y = pEnv->GetFloatField(point, idy);

	FPDF_Page_DeviceToPagePointF(pPage, start_x, start_y, size_x, size_y, rotate, &devicePoint);

	pEnv->SetFloatField(point, idx, devicePoint.x);
	pEnv->SetFloatField(point, idy, devicePoint.y);
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPagePageToDeviceRectF
  (JNIEnv *pEnv, jclass clss, jint page, jint startx, jint starty, jint sizex, jint sizey, jint rotate, jobject rect)
{
	if (page == 0) return;
	FPDF_PAGE pPage = (FPDF_PAGE)page;

	if (!rect) return;
	FS_RECTF pdfRect;
	jclass cls = pEnv->GetObjectClass(rect);
	jfieldID ddA = pEnv->GetFieldID(cls, "left", "F");
	jfieldID ddB = pEnv->GetFieldID(cls, "top", "F");
	jfieldID ddC = pEnv->GetFieldID(cls, "right", "F");
	jfieldID ddD = pEnv->GetFieldID(cls, "bottom", "F");

	pdfRect.left = pEnv->GetFloatField(rect, ddA);
	pdfRect.top = pEnv->GetFloatField(rect, ddB);
	pdfRect.right = pEnv->GetFloatField(rect, ddC);
	pdfRect.bottom = pEnv->GetFloatField(rect, ddD);

	FPDF_Page_PageToDeviceRectF(pPage, startx, starty, sizex, sizey, rotate, &pdfRect);
	
	pEnv->SetFloatField(rect, ddA, pdfRect.left);
	pEnv->SetFloatField(rect, ddB, pdfRect.top);
	pEnv->SetFloatField(rect, ddC, pdfRect.right);
	pEnv->SetFloatField(rect, ddD, pdfRect.bottom);
}
/***********************************************************************
*
*	Base module
*
***********************************************************************/


JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSInitLibrary
  (JNIEnv *pEnv, jclass cls, jint hInstance)
{
	FS_Library_Init((void*)hInstance);
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSDestroyLibrary
  (JNIEnv *pEnv, jclass cls)
{
	FS_Library_Destroy();
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSMemInitFixedMemory
  (JNIEnv *pEnv, jclass cls, jint size)
{
	if(FixedMemoryInitialized) return;
	
	if(size<0)
		throwException(pEnv,cls,FS_ERR_MEMORY,"FSMemInitFixedMemory: cannot allocate memory less than 0");
	
	//allocat memory block
	managedMemoryBlock = malloc(size);
	
	//instantiate FS_MEM_FIXEDMGR*
	pFixedMemmgr = (FS_MEM_FIXEDMGR*)malloc(sizeof(FS_MEM_FIXEDMGR));
	pFixedMemmgr->More = FXMem_More;
	pFixedMemmgr->Free = FXMem_Free;
	pFixedMemmgr->OOMHandler = OOMHandler;
	//define FPDF_FIXED_OOM_HANDLER
	//FPDF_FIXED_OOM_HANDLER handler = OOMHandler;

	//FS_RESULT ret = FS_Mem_InitFixedMemory(managedMemoryBlock, size, pFixedMemmgr, handler);
	FS_RESULT ret = FS_Memory_InitFixed(managedMemoryBlock, size, pFixedMemmgr);
	if(ret)
		throwException(pEnv,cls,ret,"FSMemInitFixedMemory: FS_Mem_InitFixedMemory did not return success");
	
	FixedMemoryInitialized = TRUE;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSMemDestroyMemory
  (JNIEnv *pEnv, jclass cls)
{
	FS_Memory_Destroy();
	free(pFixedMemmgr);
	free(managedMemoryBlock);
	FixedMemoryInitialized = FALSE;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSBitmapCreate
  (JNIEnv *pEnv, jclass cls, jint width, jint height, jint format, jbyteArray buffer, jint stride)
{
	FS_BITMAP bitmap = NULL;
	void* pBuffer = NULL;
	int nRet;
	if (buffer != NULL)
	{
		unsigned char* pBuf = (unsigned char*)(pEnv->GetByteArrayElements(buffer, NULL));
		pBuffer = (void*)pBuf;
	}

	nRet = FS_Bitmap_Create(width, height, format, pBuffer, stride, &bitmap);
	if(nRet)
		throwException(pEnv,cls,nRet,"FSBitmapCreate: FS_Bitmap_Create did not return success");

	LOGI("memcheck--FSBitmapCreate, bmp: %x", bitmap);
	return (jint)bitmap;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSBitmapDestroy
  (JNIEnv *pEnv, jclass cls, jint dib)
{
	FS_RESULT ret;

	LOGI("memcheck--FSBitmapDestroy, bmp: %x", dib);

	if(ret = FS_Bitmap_Destroy((FS_BITMAP)dib))
		throwException(pEnv,cls,ret,"FSBitmapDestroy: FS_Bitmap_Destroy did not return success");
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSBitmapGetBuffer
  (JNIEnv *pEnv, jclass cls, jint dib, jbyteArray data)
{
	FS_BITMAP bitmap = (FS_BITMAP)dib;
	int stride = FS_Bitmap_GetStride(bitmap);
	int height = FS_Bitmap_GetHeight(bitmap);
	void* buffer = FS_Bitmap_GetBuffer(bitmap);
	LOGI("FSBitmapGetBuffer, stride(%d), height(%d), buffer(%p)", stride, height, buffer);
	if(!bitmap || stride < 0 || height < 0 || buffer == NULL)
		throwException(pEnv, cls, FS_ERR_PARAM, "FSBitmapGetBuffer: null or invalid dib pointer");
	
	int size = stride*height;
	pEnv->SetByteArrayRegion(data, 0, size, (jbyte*)buffer);

	return size;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSBitmapFillColor
  (JNIEnv *pEnv, jclass cls, jint dib, jint argb)
{
	FS_BITMAP bitmap = (FS_BITMAP)dib;
	void* buffer = FS_Bitmap_GetBuffer(bitmap);
	int stride = FS_Bitmap_GetStride(bitmap);
	int height = FS_Bitmap_GetHeight(bitmap);
	
	if(!dib||buffer==NULL||stride<0||height<0)
		throwException(pEnv,cls,FS_ERR_PARAM,"FSBitmapFillColor: null or invalid dib pointer");
	memset(FS_Bitmap_GetBuffer(bitmap), argb, FS_Bitmap_GetStride(bitmap) * FS_Bitmap_GetHeight(bitmap));
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFontLoadGBCMap
  (JNIEnv *pEnv, jclass cls)
{
	FS_FontCMap_LoadGB();
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFontLoadGBExtCMap
  (JNIEnv *pEnv, jclass cls)
{
	FS_FontCMap_LoadGBExt();
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFontLoadCNSCMap
  (JNIEnv *pEnv, jclass cls)
{
	FS_FontCMap_LoadCNS();
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFontLoadKoreaCMap
  (JNIEnv *pEnv, jclass cls)
{
	FS_FontCMap_LoadKorea();
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFontLoadJapanCMap
  (JNIEnv *pEnv, jclass cls)
{
	FS_FontCMap_LoadJapan();
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFontLoadJapanExtCMap
  (JNIEnv *pEnv, jclass cls)
{
	FS_FontCMap_LoadJapanExt();
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSLoadJbig2Decoder
  (JNIEnv *pEnv, jclass cls)
{
	FS_LoadJbig2Decoder();
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSLoadJpeg2000Decoder
  (JNIEnv *pEnv, jclass cls)
{
	FS_LoadJpeg2000Decoder();
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFileReadAlloc
  (JNIEnv *pEnv, jclass cls, jstring filePath)
{
	if (!FixedMemoryInitialized) return NULL;
	
	FS_FILEREAD* pFileRead = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FS_FILEREAD),(FS_LPVOID*)&pFileRead);
	if(nRet)
		throwException(pEnv,cls,nRet,"FSFileReadAlloc: out of memory");
	
	pFileRead->GetSize = FileGetSize;
	pFileRead->ReadBlock = FileReadBlock;

	const char* file_path;
	file_path = pEnv->GetStringUTFChars(filePath, NULL);
	void* file = fopen(file_path, "rb");
	pFileRead->clientData = file;

	return (jint)pFileRead;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFileReadRelease
  (JNIEnv *pEnv, jclass cls, jint fileRead)
{
	FS_FILEREAD* pFileRead = (FS_FILEREAD*)fileRead;
	fclose((FILE*)pFileRead->clientData);
	if (!FixedMemoryInitialized)
	{
		free(pFileRead);
		return;
	}
	FS_Memory_Free(pFileRead);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSPauseHandlerAlloc
  (JNIEnv *pEnv, jclass cls)
{
	if (!FixedMemoryInitialized) return NULL;
	FS_PAUSE* pPause = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FS_PAUSE),(FS_LPVOID*)&pPause);
	if(nRet)
		throwException(pEnv,cls,nRet,"FSPauseHandlerAlloc: out of memory");

	pPause->NeedPauseNow = NeedToPauseNow;
	pPause->clientData = NULL;

	return (jint)pPause;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSPauseHandlerRelease
  (JNIEnv *pEnv, jclass cls, jint pauseHandler)
{
	FS_PAUSE* pPause = (FS_PAUSE*)pauseHandler;
	if (!pPause)
		return;

	if (pPause->clientData){
		//Clean up clientData. Since clientData in this case is null
		//we do nothing. If you are using clientData, make sure you
		//call the appropriate cleanup functions.
	}
	
	pPause->clientData = NULL;
	
	if (!FixedMemoryInitialized)
	{
		free(pPause);
		return;
	}
 	FS_Memory_Free(pPause);
}
/***********************************************************************
*
*	Form module
*
***********************************************************************/

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillerInfoAlloc
  (JNIEnv *pEnv, jclass cls, jobject jobJFormFillerInfo)
{
	if (jobJFormFillerInfo == 0)
		return (jint)NULL;
	
	jobject object = pEnv->NewGlobalRef(jobJFormFillerInfo);
	CPDF_FormFillerInfo::SetEnv(pEnv, object);
	
	//if (CPDFGlobalData::m_pSGlobalData->m_bInitedFixedMemory == FALSE)
	//	return (jint)NULL;

	FPDF_FORMFILL_INFO* pFormFillerInfo = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPDF_FORMFILL_INFO),(FS_LPVOID*)&pFormFillerInfo);
	if (nRet)
		throwException(pEnv,cls,nRet,"FPDFFormFillerInfoAlloc: out of memory");

	pFormFillerInfo->clientData = NULL;
	//pFormFillerInfo->version = 1;
	pFormFillerInfo->Release = CPDF_FormFillerInfo::Release;
	pFormFillerInfo->ExecuteNamedAction = CPDF_FormFillerInfo::ExecuteNamedAction;
	pFormFillerInfo->GetCurrentPage = CPDF_FormFillerInfo::GetCurrentPage;
	pFormFillerInfo->GetLocalTime = CPDF_FormFillerInfo::GetLocalTime;
	pFormFillerInfo->GetPage = CPDF_FormFillerInfo::GetPage;
	pFormFillerInfo->GetRotation = CPDF_FormFillerInfo::GetRotation;
	pFormFillerInfo->Invalidate = CPDF_FormFillerInfo::Invalidate;
	pFormFillerInfo->KillTimer = CPDF_FormFillerInfo::KillTimer;
	pFormFillerInfo->OnChange = CPDF_FormFillerInfo::OnChange;
	pFormFillerInfo->OnSetFieldInputFocus = CPDF_FormFillerInfo::OnSetFieldInputFocus;
	pFormFillerInfo->OutputSelectedRect = NULL;
	pFormFillerInfo->SetCursor = CPDF_FormFillerInfo::SetCursor;
	pFormFillerInfo->SetTimer = CPDF_FormFillerInfo::SetTimer;
	pFormFillerInfo->jsPlatform = NULL;
	return (jint)pFormFillerInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillerInfoRelease
  (JNIEnv *pEnv, jclass cls, jint nFormFillerInfo)
{
	if (nFormFillerInfo == 0)
		return;

	pEnv->DeleteGlobalRef(CPDF_FormFillerInfo::m_objFormFillerInfo);
	FS_Memory_Free((void*)nFormFillerInfo);
} 

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFJsPlatformAlloc
  (JNIEnv *pEnv, jclass cls, jobject jobJJsPlatform)
{
	if (jobJJsPlatform == 0) 
		return (jint)NULL;
	jobject object = pEnv->NewGlobalRef(jobJJsPlatform);
	CPDF_JsPlatform::SetEnv(pEnv, object);

	//if (CPDFGlobalData::m_pSGlobalData->m_bInitedFixedMemory == FALSE)
	//	return (jint)NULL;
	
	FPDF_FORMFILL_JSPLATFORM* pJsPlatform = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPDF_FORMFILL_JSPLATFORM),(FS_LPVOID*)&pJsPlatform);
	if (nRet)
		throwException(pEnv,cls,nRet,"FPDFJsPlatformAlloc: out of memory");

	//pJsPlatform->version = 1;
	pJsPlatform->clientData = NULL;
	pJsPlatform->Alert = CPDF_JsPlatform::alert;
	pJsPlatform->Beep = CPDF_JsPlatform::beep;
	pJsPlatform->Response = CPDF_JsPlatform::response;
	pJsPlatform->GetFilePath = CPDF_JsPlatform::getFilePath;
	pJsPlatform->Mail = CPDF_JsPlatform::mail;
	pJsPlatform->Print = CPDF_JsPlatform::print;
	pJsPlatform->SubmitForm = CPDF_JsPlatform::submitForm;
	pJsPlatform->Browse = CPDF_JsPlatform::browse;
	pJsPlatform->formfillinfo = NULL;

	return (jint)pJsPlatform;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFJsPlatformRelease
  (JNIEnv *pEnv, jclass cls, jint nJsPlatform)
{
	if (nJsPlatform == 0)
		return;

	pEnv->DeleteGlobalRef(CPDF_JsPlatform::m_objJsPlatform);
	FS_Memory_Free((void*)nJsPlatform);
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillerInfoSetJsPlatform
  (JNIEnv *pEnv, jclass cls, jint nFormFillerInfo, jint nJsPlatform)
{
	if (nFormFillerInfo == 0)
		return;
	FPDF_FORMFILL_INFO* pFormFillerInfo = (FPDF_FORMFILL_INFO*)nFormFillerInfo;
	FPDF_FORMFILL_JSPLATFORM* pJsPlatform = (FPDF_FORMFILL_JSPLATFORM*)nJsPlatform;

	pFormFillerInfo->jsPlatform = pJsPlatform;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFJsPlatformSetFormFillerInfo
  (JNIEnv *pEnv, jclass cls, jint nJsPlatform, jint nFormFillerInfo)
{
	if (nJsPlatform == 0)
		return;
	FPDF_FORMFILL_INFO* pFormFillerInfo = (FPDF_FORMFILL_INFO*)nFormFillerInfo;
	FPDF_FORMFILL_JSPLATFORM* pJsPlatform = (FPDF_FORMFILL_JSPLATFORM*)nJsPlatform;
	
	pJsPlatform->formfillinfo = pFormFillerInfo;
}	

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFDocInitFormFillEnviroument
  (JNIEnv *pEnv, jclass cls, jint document, jint nFormFillerInfo)
{
	if (document == 0 || nFormFillerInfo == 0)
		return (jint)NULL;
	
	FPDF_DOCUMENT pPDFDoc = (FPDF_DOCUMENT)document;
	FPDF_FORMFILL_INFO* pFormFillerInfo = (FPDF_FORMFILL_INFO*)nFormFillerInfo;

	FPDF_FORMENV formHandler = NULL;
	FS_RESULT ret1 = FPDF_FormFill_InitEnvironment(pPDFDoc, pFormFillerInfo,&formHandler);
	LOGI("InitFormFillEnviroument = %x", ret1);

	return (jint)formHandler;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFDocExitFormFillEnviroument
  (JNIEnv *pEnv, jclass cls, jint nFormHandler)
{
	if (nFormHandler == 0)
		return;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;
	FPDF_FormFill_ExitEnvironment(formHandler);
	
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillOnAfterLoadPage
  (JNIEnv *pEnv, jclass cls, jint nFormHandler,jint page )
{
	if (page == 0 || nFormHandler == 0)
		return;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;
	
	int ret = FPDF_FormFill_OnAfterLoadPage(formHandler,pPage);
	LOGI("Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDF_1FormFill_1OnAfterLoadPage RET IS  %x",ret);
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillDraw
  (JNIEnv *pEnv, jclass cls, jint nFormHandler, jint dib, jint page, jint startx, jint starty, jint sizex, jint sizey, jint rotate, jint flags)
{
	if (page == 0 || nFormHandler == 0 || dib == 0)
		return;

	FPDF_PAGE pPage = (FPDF_PAGE)page;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;
	FS_BITMAP bitmap = (FS_BITMAP)dib;

	FPDF_FormFill_Draw(formHandler, bitmap, pPage, startx, starty, sizex, sizey, rotate, flags);
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillOnKillFocus
  (JNIEnv *pEnv, jclass cls, jint nFormHandler)
{
	if (nFormHandler == 0)
		return;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;

	FPDF_FormFill_OnKillFocus(formHandler);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormCreateInterForm
  (JNIEnv *pEnv, jclass cls, jint document, jboolean updateAP)
{
	if (document == 0)
		return (jint)NULL;

	FPDF_DOCUMENT pPDFDoc = (FPDF_DOCUMENT)document;

	FPDF_FORM pInterForm = NULL;
	FS_RESULT ret1 = FPDF_Form_Create(pPDFDoc, updateAP,&pInterForm);

	return (jint)pInterForm;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormReleaseInterForm
  (JNIEnv *pEnv, jclass cls, jint nInterForm)
{
	if (nInterForm == 0)
		return;

	FPDF_Form_Release((FPDF_FORM)nInterForm);
}

JNIEXPORT jboolean JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormExportToXML
  (JNIEnv *pEnv, jclass cls, jint nInterForm, jint nFileWriter)
{

	if (nInterForm == 0)
		return (jboolean)FALSE;
	FPDF_FORM pInterForm = (FPDF_FORM)nInterForm;
	void *file = fopen("/data/data/com.foxitsample.formfiled/FormXml.xml", "wb");
	FS_FILEWRITE fileWriter;
	fileWriter.GetSize = FileGetSize;
	fileWriter.WriteBlock = FileWriterBlock;
	fileWriter.Flush = FileFlush;
	fileWriter.clientData = file;
	FS_BOOL bRet = FPDF_Form_ExportToXML(pInterForm, fileWriter);
	fclose((FILE*)file);

	return (jboolean)bRet;
}

JNIEXPORT jboolean JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormImportFromXML
  (JNIEnv *pEnv, jclass cls, jint nInterForm, jint nFileReader)
{

	if (nInterForm == 0)
		return (jboolean)FALSE;

	FPDF_FORM pInterForm = (FPDF_FORM)nInterForm;
	void* file = fopen("/data/data/com.foxitsample.formfiled/FormXml.xml", "rb");
	if(file == NULL) return FALSE;
	FS_FILEREAD fileReader;
	fileReader.GetSize = FileGetSize;
	fileReader.ReadBlock = FileReadBlock;
	fileReader.clientData = file;

	FS_BOOL bRet = FPDF_Form_ImportFromXML(pInterForm, fileReader);
	fclose((FILE*)file);
	return (jboolean)bRet;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillUpdatForm
  (JNIEnv *pEnv, jclass cls, jint nFormHandler)
{
	if (nFormHandler == 0)
		return;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;
	FPDF_FormFill_Update(formHandler);
	
}

JNIEXPORT jboolean JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillOnMouseMove
  (JNIEnv *pEnv, jclass cls, jint nFormHandler, jint page, jint evenflag, jdouble page_x, jdouble page_y)
{
	if (nFormHandler == 0 || page == 0)
		return (jboolean)FALSE;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	CPDF_FormFillerInfo::m_bNewThread = FALSE;

	FS_BOOL bRet = FPDF_FormFill_OnMouseMove(formHandler, pPage, evenflag, page_x, page_y);
	return (jboolean)bRet;
}

JNIEXPORT jboolean JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillOnLButtonUp
  (JNIEnv *pEnv, jclass cls, jint nFormHandler, jint page, jint evenflag, jdouble page_x, jdouble page_y)
{
	if (nFormHandler == 0 || page == 0)
		return (jboolean)FALSE;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	CPDF_FormFillerInfo::m_bNewThread = FALSE;

	FS_BOOL bRet = FPDF_FormFill_OnLButtonUp(formHandler, pPage, evenflag, page_x, page_y);
	return (jboolean)bRet;
}

JNIEXPORT jboolean JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillOnLButtonDown
  (JNIEnv *pEnv, jclass cls, jint nFormHandler, jint page, jint evenflag, jdouble page_x, jdouble page_y)
{
	if (nFormHandler == 0 || page == 0)
		return (jboolean)FALSE;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	CPDF_FormFillerInfo::m_bNewThread = FALSE;

	FS_BOOL bRet = FPDF_FormFill_OnLButtonDown(formHandler, pPage, evenflag, page_x, page_y);
	return (jboolean)bRet;
}

JNIEXPORT jboolean JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFormFillOnSetText
  (JNIEnv *pEnv, jclass cls, jint nFormHandler, jint page, jstring text, jint evenflag)
{
	if (nFormHandler == 0 || page == 0)
		return (jboolean)FALSE;
	FPDF_FORMENV formHandler = (FPDF_FORMENV)nFormHandler;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	CPDF_FormFillerInfo::m_bNewThread = FALSE;

	JNIEnv* pTempEnv = NULL;
	pTempEnv = CPDF_FormFillerInfo::m_pEnv;	
	
	FS_LPCWSTR pText = NULL;
	int nLen = pTempEnv->GetStringLength(text);   
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(const unsigned short)*nLen,(FS_LPVOID*)&pText);
	if (nRet)
		throwException(pEnv,cls,nRet,"FPDFFormFillOnSetText: out of memory");

	memset((void*)pText, '\0', sizeof(const unsigned short)*nLen);
	memcpy((void*)pText, pTempEnv->GetStringChars(text, NULL), nLen*sizeof(const unsigned short));
	pTempEnv->ReleaseStringChars(text, pText);
	FS_BOOL bRet = FPDF_FormFill_OnSetText(formHandler, pPage, pText, nLen, evenflag);
	return (jboolean)bRet;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFExecCallBack
  (JNIEnv *pEnv, jclass cls, jint callbackaddr, jboolean bNewThread)
{
	if (callbackaddr == 0)
		return;
	
	CPDF_FormFillerInfo::m_bNewThread = bNewThread;
	CPDF_FormFillerInfo::SetNewThreadEnv(pEnv);

	FS_LPFTimerCallback pCallBackFunc = (FS_LPFTimerCallback)callbackaddr;
	(*pCallBackFunc)(10);

}


/***********************************************************************
*
*	Annotation module
*
***********************************************************************/

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFNoteInfoAlloc
(JNIEnv *pEnv, jclass clss, jstring author, jlong color, jint opacity, jobject rect, jstring content,jint page)
{
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	FPDF_ANNOT_NOTEINFO* pNoteInfo = NULL;
	FS_RESULT ret;

	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPDF_ANNOT_NOTEINFO),(FS_LPVOID*)&pNoteInfo);
	if (nRet)
		throwException(pEnv,clss,nRet,"FPDFNoteInfoAlloc: note info out of memory");

	pNoteInfo->size = sizeof(FPDF_ANNOT_NOTEINFO);

	int nLen = pEnv->GetStringLength(author);
	if (nLen > 63) return (jint)NULL;
	memset(pNoteInfo->author, '\0', 64);
	memcpy(pNoteInfo->author, pEnv->GetStringChars(author, NULL), nLen*sizeof(FS_WCHAR));
	pEnv->ReleaseStringChars(author, pNoteInfo->author);

	pNoteInfo->color = color;
	pNoteInfo->opacity =opacity;

	nLen = pEnv->GetStringLength(content);
	pNoteInfo->contents = NULL;

	nRet = FS_Memory_Alloc(sizeof(FS_WCHAR)*(nLen+1),(FS_LPVOID*)(&(pNoteInfo->contents)));
	if (nRet)
		throwException(pEnv,clss,nRet,"FPDFNoteInfoAlloc: note info content out of memory");

	if (!pNoteInfo->contents)return (jint)NULL;
	memset((void*)pNoteInfo->contents, '\0', (nLen+1)*sizeof(FS_WCHAR));

	memcpy((void*)pNoteInfo->contents, pEnv->GetStringChars(content, NULL), nLen*sizeof(FS_WCHAR));
	

	pEnv->ReleaseStringChars(content, pNoteInfo->contents);

	if (!rect) return (jint)NULL;
	FS_RECTF pdfRect;

	jclass cls = pEnv->GetObjectClass(rect);
	jfieldID idA = pEnv->GetFieldID(cls, "left", "F");
	jfieldID idB = pEnv->GetFieldID(cls, "top", "F");
	jfieldID idC = pEnv->GetFieldID(cls, "right", "F");
	jfieldID idD = pEnv->GetFieldID(cls, "bottom", "F");

	pdfRect.left = pEnv->GetFloatField(rect, idA);
	pdfRect.top = pEnv->GetFloatField(rect, idB);
	pdfRect.right = pEnv->GetFloatField(rect, idC);
	pdfRect.bottom = pEnv->GetFloatField(rect, idD);

	pNoteInfo->rect = pdfRect;

	return (jint)pNoteInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFNoteInfoRelease
  (JNIEnv *pEnv, jclass cls, jint noteInfo)
{
 	if (noteInfo == 0)return;

 	FPDF_ANNOT_NOTEINFO* pNoteInfo = (FPDF_ANNOT_NOTEINFO*)noteInfo;
 	FS_Memory_Free((void*)pNoteInfo->contents);

 	FS_Memory_Free((void*)pNoteInfo);
}
 
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPencilInfoAlloc
  (JNIEnv *pEnv, jclass cls, jstring author, jlong color, jint opacity, jboolean busebezier, jboolean boptimize, jint line_width, jint line_count)
{
	FPDF_ANNOT_PENCILINFO* pPencilInfo = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPDF_ANNOT_PENCILINFO),(FS_LPVOID*)&pPencilInfo);
	if (nRet)
		throwException(pEnv,cls,nRet,"FPDFPencilInfoAlloc: out of memory");

	pPencilInfo->size = sizeof(FPDF_ANNOT_PENCILINFO);
	int nLen = pEnv->GetStringLength(author);
	if (nLen > 63) return (jint)NULL;
	memset(pPencilInfo->author, '\0', 64);
	memcpy(pPencilInfo->author, pEnv->GetStringChars(author, NULL), nLen*sizeof(FS_WCHAR));
	pEnv->ReleaseStringChars(author, pPencilInfo->author);

	pPencilInfo->color = color;
	pPencilInfo->opacity = opacity;
	pPencilInfo->busebezier = busebezier;
	pPencilInfo->boptimize = boptimize;
	pPencilInfo->line_width = line_width;
	pPencilInfo->line_count = line_count;

	return (jint)pPencilInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPencilInfoSetLineInfo
  (JNIEnv *pEnv, jclass cls, jint nPencilInfo, jint nLineInfo)
{
	FPDF_ANNOT_PENCILINFO* pPencilInfo = (FPDF_ANNOT_PENCILINFO*)nPencilInfo;
	if (!pPencilInfo) return;

	FPDF_ANNOT_LINE* pLineInfo = (FPDF_ANNOT_LINE*)nLineInfo;
	if (!pLineInfo) return;

	pPencilInfo->lines = pLineInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPencilInfoRelease
  (JNIEnv *pEnv, jclass cls, jint pencilInfo)
{
	if (pencilInfo == 0)return;

	FPDF_ANNOT_PENCILINFO* pPencilInfo = (FPDF_ANNOT_PENCILINFO*)pencilInfo;
 	FS_Memory_Free((void*)pPencilInfo);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFLineInfoAlloc
  (JNIEnv *pEnv, jclass cls, jint line_count)
{
	FPDF_ANNOT_LINE* pLineInfoArray = (FPDF_ANNOT_LINE*)malloc(sizeof(FPDF_ANNOT_LINE) * line_count);
	return (jint)pLineInfoArray;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFLineInfoSetPointInfo
  (JNIEnv *pEnv, jclass cls, jint nLineInfo, jint line_index, jint nPointCounts, jfloatArray points)
{
	FPDF_ANNOT_LINE* pLineInfoArray = (FPDF_ANNOT_LINE*)nLineInfo;
	if (!pLineInfoArray) return;

	float *pPoints = (float*)pEnv->GetFloatArrayElements(points, FALSE);
	FS_POINTF* pPointArr = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FS_POINTF)*nPointCounts,(FS_LPVOID*)&pPointArr);
	if (nRet)
		throwException(pEnv,cls,nRet,"FPDFLineInfoSetPointInfo: out of memory");

	int i;
	for (i=0; i<nPointCounts; i++)
	{
		pPointArr[i].x = pPoints[i*2];
		pPointArr[i].y = pPoints[i*2+1];
	}

	pLineInfoArray[line_index].point_count = nPointCounts;
	pLineInfoArray[line_index].points = pPointArr;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFLineInfoRelease
  (JNIEnv *pEnv, jclass cls, jint nLineInfo)
{
	if (nLineInfo == 0)return;
	FPDF_ANNOT_LINE* pLineInfo = (FPDF_ANNOT_LINE*)nLineInfo;

	FS_Memory_Free(pLineInfo->points);
	FS_Memory_Free(pLineInfo);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFHighlightInfoAlloc
  (JNIEnv *pEnv, jclass cls, jstring author, jlong color, jint opacity, jint quad_cout)
{
	FPDF_ANNOT_HIGHLIGHTINFO* pHighlightInfo = NULL;
	 
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPDF_ANNOT_HIGHLIGHTINFO),(FS_LPVOID*)&pHighlightInfo);
	if (nRet)
		throwException(pEnv,cls,nRet,"FPDFHighlightInfoAlloc: out of memory");

	pHighlightInfo->size = sizeof(FPDF_ANNOT_HIGHLIGHTINFO);
	pHighlightInfo->color = color;
	pHighlightInfo->opacity = opacity;
	pHighlightInfo->quad_count  = quad_cout;

	int nLen = pEnv->GetStringLength(author);
	if (nLen > 63) return (jint)NULL;
	memset(pHighlightInfo->author, '\0', 64);
	memcpy(pHighlightInfo->author, pEnv->GetStringChars(author, NULL), nLen*sizeof(FS_WCHAR));
	pEnv->ReleaseStringChars(author, pHighlightInfo->author);

	return (jint)pHighlightInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFHighlightSetQuads
  (JNIEnv *pEnv, jclass cls, jint nHighlighInfo, jint nQuadsInfo)
{
	if (nHighlighInfo == 0)
		return;
	if (nQuadsInfo == 0)
		return;

	FPDF_ANNOT_HIGHLIGHTINFO* pHighlightInfo = (FPDF_ANNOT_HIGHLIGHTINFO*)nHighlighInfo;
	FPDF_ANNOT_QUAD* pQuadsInfo = (FPDF_ANNOT_QUAD*)nQuadsInfo;

	pHighlightInfo->quads = pQuadsInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFHighlightInfoRelease
  (JNIEnv *pEnv, jclass cls, jint nHighlightInfo)
{
	if (nHighlightInfo == 0)
		return;
	FPDF_ANNOT_HIGHLIGHTINFO* pHighlightInfo = (FPDF_ANNOT_HIGHLIGHTINFO*)nHighlightInfo;
	FS_Memory_Free(pHighlightInfo);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFQuadsInfoAlloc
  (JNIEnv *pEnv, jclass cls, jint quad_count, jfloatArray quads)
{
	FPDF_ANNOT_QUAD* pQuadsInfo = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPDF_ANNOT_QUAD)*quad_count,(FS_LPVOID*)&pQuadsInfo);
	if (nRet)
		throwException(pEnv,cls,nRet,"FPDFQuadsInfoAlloc: out of memory");

	float *pQuads = (float*)pEnv->GetFloatArrayElements(quads, FALSE);
	int i;
	for (i=0; i<quad_count; i++)
	{
		pQuadsInfo[i].x1 = pQuads[8*i];
		pQuadsInfo[i].y1 = pQuads[8*i+1];

		pQuadsInfo[i].x2 = pQuads[8*i+2];
		pQuadsInfo[i].y2 = pQuads[8*i+3];

		pQuadsInfo[i].x3 = pQuads[8*i+4];
		pQuadsInfo[i].y3 = pQuads[8*i+5];

		pQuadsInfo[i].x4 = pQuads[8*i+6];
		pQuadsInfo[i].y4 = pQuads[8*i+7];
	}

	return (jint)pQuadsInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFQuadsInfoRelease
  (JNIEnv *pEnv, jclass cls, jint nQuadInfo)
{
	if (nQuadInfo == 0)
		return;
	FPDF_ANNOT_QUAD* pQuadsInfo = (FPDF_ANNOT_QUAD*)nQuadInfo;
	FS_Memory_Free(pQuadsInfo);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFStampInfoAlloc
  (JNIEnv *pEnv, jclass clss, jstring author, jlong color, jint opacity, jobject rect, jstring subject, jstring image_path)
{
	FPDF_ANNOT_STAMPINFO* pStampInfo = NULL;
	FS_RESULT nRet= FS_Memory_Alloc(sizeof(FPDF_ANNOT_STAMPINFO),(FS_LPVOID*)&pStampInfo);
	if (nRet)
		throwException(pEnv,clss,nRet,"FPDFStampInfoAlloc: stamp info out of memory");

	pStampInfo->size = sizeof(FPDF_ANNOT_STAMPINFO);
	pStampInfo->color = color;
	pStampInfo->opacity = opacity;

	int nLen = pEnv->GetStringLength(author);
	if (nLen > 63) return (jint)NULL;
	memset(pStampInfo->author, '\0', 64);
	memcpy(pStampInfo->author, pEnv->GetStringChars(author, NULL), nLen*sizeof(FS_WCHAR));
	pEnv->ReleaseStringChars(author, pStampInfo->author);

	if (!rect) return (jint)NULL;
	FS_RECTF pdfRect;
	jclass cls = pEnv->GetObjectClass(rect);
	jfieldID idA = pEnv->GetFieldID(cls, "left", "F");
	jfieldID idB = pEnv->GetFieldID(cls, "top", "F");
	jfieldID idC = pEnv->GetFieldID(cls, "right", "F");
	jfieldID idD = pEnv->GetFieldID(cls, "bottom", "F");
	pdfRect.left = pEnv->GetFloatField(rect, idA);
	pdfRect.top = pEnv->GetFloatField(rect, idB);
	pdfRect.right = pEnv->GetFloatField(rect, idC);
	pdfRect.bottom = pEnv->GetFloatField(rect, idD);
	pStampInfo->rect = pdfRect;

	nLen = pEnv->GetStringLength(subject);
	if (nLen > 31) return (jint)NULL;
	memset(pStampInfo->name, '\0', 32);
	memcpy(pStampInfo->name, pEnv->GetStringChars(subject, NULL), nLen*sizeof(FS_WCHAR));
	pEnv->ReleaseStringChars(subject, pStampInfo->name);

	pStampInfo->imgtype = FPDF_IMAGETYPE_JPG;
	
	const char* pImagePath = pEnv->GetStringUTFChars(image_path, NULL);
	unsigned char* buffer =NULL;
	FILE* file = fopen(pImagePath, "rb");
	fseek(file, 0,SEEK_END);
	int len = ftell(file);
	fseek(file,0,SEEK_SET);
	nRet = FS_Memory_Alloc(len,(FS_LPVOID*)&buffer);
	if (nRet)
		throwException(pEnv,clss,nRet,"FPDFStampInfoAlloc: file buffer out of memory");

	fread(buffer, 1, len, file);
	fclose(file);
	pStampInfo->imgdatasize = len;
	pStampInfo->imgdata = (unsigned char*)buffer;

	return (jint)pStampInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFStampInfoRelease
  (JNIEnv *pEnv, jclass cls, jint nStampInfo)
{
	if (nStampInfo == 0)
		return;

	FPDF_ANNOT_STAMPINFO* pStampInfo = (FPDF_ANNOT_STAMPINFO*)nStampInfo;
	FS_Memory_Free(pStampInfo);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFileAttachmentInfoAlloc
  (JNIEnv *pEnv, jclass clss, jstring author, jlong color, jint opacity, jobject rect, jstring filepath)
{
	FPDF_ANNOT_FILEATTACHMENTINFO* pFileAttachmentInfo = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPDF_ANNOT_FILEATTACHMENTINFO),(FS_LPVOID*)&pFileAttachmentInfo);
	if (nRet)
		throwException(pEnv,clss,nRet,"FPDFFileAttachmentInfoAlloc: file attachment info out of memory");

	pFileAttachmentInfo->size = sizeof(FPDF_ANNOT_FILEATTACHMENTINFO);
	pFileAttachmentInfo->color = color;
	pFileAttachmentInfo->icon = 0;
	pFileAttachmentInfo->opacity = opacity;

	int nLen = pEnv->GetStringLength(author);
	if (nLen > 63) return (jint)NULL;
	memset(pFileAttachmentInfo->author, '\0', 64);
	memcpy(pFileAttachmentInfo->author, pEnv->GetStringChars(author, NULL), nLen*sizeof(FS_WCHAR));
	pEnv->ReleaseStringChars(author, pFileAttachmentInfo->author);

	if (!rect) return (jint)NULL;
	FS_RECTF pdfRect;
	jclass cls = pEnv->GetObjectClass(rect);
	jfieldID idA = pEnv->GetFieldID(cls, "left", "F");
	jfieldID idB = pEnv->GetFieldID(cls, "top", "F");
	jfieldID idC = pEnv->GetFieldID(cls, "right", "F");
	jfieldID idD = pEnv->GetFieldID(cls, "bottom", "F");
	pdfRect.left = pEnv->GetFloatField(rect, idA);
	pdfRect.top = pEnv->GetFloatField(rect, idB);
	pdfRect.right = pEnv->GetFloatField(rect, idC);
	pdfRect.bottom = pEnv->GetFloatField(rect, idD);
	pFileAttachmentInfo->rect = pdfRect;

	const char* pFilePath = pEnv->GetStringUTFChars(filepath, NULL);
	unsigned char* buffer =NULL;
	FILE* file = fopen(pFilePath, "rb");
	fseek(file, 0,SEEK_END);
	int len = ftell(file);
	fseek(file,0,SEEK_SET);
	nRet = FS_Memory_Alloc(len,(FS_LPVOID*)&buffer);
	if (nRet)
		throwException(pEnv,clss,nRet,"FPDFFileAttachmentInfoAlloc: file attachment buffer out of memory");

	fread(buffer, 1, len, file);
	fclose(file);

	pFileAttachmentInfo->file_size = len;
	pFileAttachmentInfo->file_data = buffer;

	return (jint)pFileAttachmentInfo;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFFileAttachmentInfoRelease
  (JNIEnv *pEnv, jclass cls, jint nFileAttachmentInfo)
{
	if (nFileAttachmentInfo == 0)
		return;

	FPDF_ANNOT_FILEATTACHMENTINFO* pFileAttachmentInfo = (FPDF_ANNOT_FILEATTACHMENTINFO*)nFileAttachmentInfo;
	FS_Memory_Free(pFileAttachmentInfo);
}
 
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFAnnotAdd
  (JNIEnv *pEnv, jclass cls, jint page, jint annot_type, jint data)
{
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	if (!pPage) return -1;
	if (data == 0) return -1;

	FS_RESULT ret;
	int nAnnotCount = 0;
	ret = FPDF_Annot_GetCount(pPage, &nAnnotCount);

	LOGI("Annot Count = %x", nAnnotCount);

	int size = 0;
	switch (annot_type)
	{
	case FPDF_ANNOTTYPE_NOTE:
		{
			size = sizeof(FPDF_ANNOT_NOTEINFO);
			break;
		}
	case FPDF_ANNOTTYPE_PENCIL:
		{
			size = sizeof(FPDF_ANNOT_PENCILINFO);
			break;
		}
	case FPDF_ANNOTTYPE_STAMP:
		{
			size = sizeof(FPDF_ANNOT_STAMPINFO);
			break;
		}
	case FPDF_ANNOTTYPE_HIGHLIGHT:
		{
			size = sizeof(FPDF_ANNOT_HIGHLIGHTINFO);
			break;
		}
	case FPDF_ANNOTTYPE_FILEATTACHMENT:
		{
			size = sizeof(FPDF_ANNOT_FILEATTACHMENTINFO);
			break;
		}
	default:
		return -1;
	}
	int nIndex =0;
	LOGI("FPDF_Annot_Add JNI");
	ret = FPDF_Annot_Add(pPage, annot_type, (void*)data, size, (FPDF_ANNOT*)&nIndex);
	LOGI("FPDF_Annot_Add nIndex = %x",nIndex);
	LOGI("ret=%x", ret);
	if (ret != FS_ERR_SUCCESS)
		return -1;

	ret = FPDF_Annot_GetCount(pPage, &nAnnotCount);
	LOGI("nCount =  %x", nAnnotCount);

	return nIndex;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFAnnotDelete
  (JNIEnv *pEnv, jclass cls, jint page, jint index)
{
	if (page == 0) return FS_ERR_PARAM;
	FPDF_PAGE pPage = (FPDF_PAGE)page;

	FS_RESULT ret = FPDF_Annot_Delete(pPage, (FPDF_ANNOT)index);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFAnnotGetCount
  (JNIEnv *pEnv, jclass cls, jint page)
{
	if (page == 0) return -1;
	FPDF_PAGE pPage = (FPDF_PAGE)page;

	int count = -1;
	FS_RESULT ret = FPDF_Annot_GetCount(pPage, &count);
	return count;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFAnnotGetIndexAtPos
  (JNIEnv *pEnv, jclass cls, jint page, jint x, jint y)
{
	if (page == 0) return -1;
	FPDF_PAGE pPage = (FPDF_PAGE)page;

	int index = -1;
	FS_RESULT ret = FPDF_Annot_GetAtPos(pPage, x, y, (FPDF_ANNOT*)&index);

	return index;
}	

/***********************************************************************
*
*	Reflow module
*
***********************************************************************/
/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFReflowAllocPage
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFReflowAllocPage
  (JNIEnv *pEnv, jclass cls)
{
	FPDF_REFLOWPAGE reflowPage = NULL;
	FPDF_Reflow_AllocPage(&reflowPage);
	return (jint)reflowPage ;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFReflowStartParse
 * Signature: (IIIIII)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFReflowStartParse
  (JNIEnv *pEnv, jclass cls, jint reflowPage, jint page, jint width, jint fitPageHight, jint pauseHandler, jint nFlag)
{
	FS_PAUSE* pPause = (FS_PAUSE*)pauseHandler;
	FS_RESULT ret = FPDF_Reflow_StartParse((FPDF_REFLOWPAGE)reflowPage,(FPDF_PAGE)page,width,fitPageHight,pPause,nFlag);
	return ret;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFReflowGetPageHight
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFReflowGetPageHight
  (JNIEnv *pEnv, jclass cls, jint reflowPage)
{
	FS_FLOAT width= 0.0f;
	FS_FLOAT height=0.0f;
	FPDF_Reflow_GetPageSize((FPDF_REFLOWPAGE)reflowPage,&width,&height);
	return (jint)height;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFReflowGetPageWidth
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFReflowGetPageWidth
  (JNIEnv *pEnv, jclass cls, jint reflowPage)
{

	FS_FLOAT width= 0.0f;
	FS_FLOAT height=0.0f;
	FPDF_Reflow_GetPageSize((FPDF_REFLOWPAGE)reflowPage,&width,&height);
	return (jint)width;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFReflowStartRender
 * Signature: (IIIIIIII)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFReflowStartRender
  (JNIEnv *pEnv, jclass cls, jint dib, jint reflowPage, jint start_x, jint start_y, jint size_x, jint size_y, jint rotate, jint pauseHandler)
{
	FS_BITMAP bitmap = (FS_BITMAP)dib;
	FS_PAUSE* pPause = (FS_PAUSE*)pauseHandler;
	FS_RESULT ret= FPDF_Reflow_StartRender(bitmap,(FPDF_REFLOWPAGE)reflowPage,start_x,start_y,size_x,size_y,rotate,pPause);
	return ret;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFReflowSetDitherBits
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFReflowSetDitherBits
  (JNIEnv *pEnv, jclass cls, jint reflowPage, jint DitherBits)
{
	FS_RESULT ret = FPDF_Reflow_SetDitherBits((FPDF_REFLOWPAGE)reflowPage,DitherBits);
	return ret ;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDF_Page_GetPageHeight
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageGetPageHeight
  (JNIEnv *pEnv, jclass cls, jint page)
  {
  	FS_FLOAT height = 0;
	FS_FLOAT width = 0;
	FPDF_PAGE pPage =(FPDF_PAGE)page;
	FPDF_Page_GetSize(pPage, &width, & height);
	return height;
  }

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDF_Page_GetPageWidth
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFPageGetPageWidth
  (JNIEnv *pEnv, jclass cls, jint page)
  {
		FS_FLOAT height = 0;
		FS_FLOAT width = 0;
	   FPDF_PAGE pPage =(FPDF_PAGE)page;
	   FPDF_Page_GetSize(pPage, &width, & height);
	   return width;
  }


/*?
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDF_RenderPage_GetRenderProgress
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFRenderPageGetRenderProgress
  (JNIEnv *pEnv, jclass cls, jint page)
  {
	  FPDF_PAGE pPage = (FPDF_PAGE)page;
	  int nProgress = FPDF_RenderPage_GetProgress(pPage);
	  //LOGCQ("Jni nProgress =%1$d",nProgress);
	//  LOGI("Jni nProgress =%1$d",nProgress);
	  return nProgress ;
	  
  }


/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FS_EMBPauseHandler_Alloc
 * Signature: (LFoxitEMBSDK/EMBJavaSupport/CEMBPause;)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSEMBPauseHandlerAlloc
  (JNIEnv *pEnv, jclass cls, jobject CpauseInfo)
  {
	if (CpauseInfo == 0)
		return (jint)NULL;

	FS_PAUSE* pPause = NULL;

	FS_RESULT nRet=FS_Memory_Alloc(sizeof(FS_PAUSE),(FS_LPVOID*)&pPause);
	if (nRet)
		throwException(pEnv,cls,nRet,"FSEMBPauseHandlerAlloc: out of memory");

	pPause->clientData = NULL;
	pPause->NeedPauseNow =CPDF_Pause::NeedToPauseNow;

	  return (jint)pPause;
} 
 JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSUnlock
  (JNIEnv *pEnv, jclass cls, jstring sn, jstring code)
  {
  	const char* pstr_sn =NULL;
  	const char* pstr_code = NULL;
  	pstr_sn = pEnv->GetStringUTFChars(sn, NULL);
  	pstr_code = pEnv->GetStringUTFChars(code, NULL);
  	LOGI("sn = %s,code = %s",pstr_sn,pstr_code);
  	FS_Library_Unlock((char*)pstr_sn,(char*)pstr_code);
  }

class CPDF_FontMap :public FS_FONT_FILE_MAPPER
{
public :
	 CPDF_FontMap(){}
	 FS_BOOL MyMapFont(const char* name, int charset,
                       unsigned int flags,	int weight,
                       char* path, int* face_index)
	{
	    if(0 == charset)
	        return TRUE;
	    if(m_filepath != NULL)
	    {
	    	LOGI("m_filepath is %s",m_filepath);
	        strcpy(path, m_filepath);
	        *face_index = 0;        
	    }
		return TRUE;
	}
	const char* m_filepath;
};

FS_BOOL MyMapFont(FS_LPVOID mapper, FS_LPCSTR name, FS_INT32 charset,FS_DWORD flags,FS_INT32 weight,FS_CHAR* path, FS_INT32* face_index)
{
	LOGI("mapper is  %x",mapper);
	CPDF_FontMap* pFontMap = (CPDF_FontMap*)mapper;	
	return pFontMap->MyMapFont(name,charset,flags,weight,path,face_index);
}

CPDF_FontMap pFontMap;

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSSetFileFontmap
  (JNIEnv *pEnv, jclass cls, jstring filepath)
  {
  		const char* strfile_path;
	    strfile_path = pEnv->GetStringUTFChars(filepath, NULL);
	    pFontMap.m_filepath = strfile_path;
	    //FILE* pFile = fopen(strfile_path,"rb");
	    //LOGI("pFile is %x",pFile);
	    pFontMap.clientData = &pFontMap;
  	    pFontMap.MapFont = MyMapFont;
    	int nRet = FS_Font_SetFontFileMapper(&pFontMap);
  }	  


/***********************************************************************
*
*	PSI module
*
***********************************************************************/
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSIInitAppCallback
  (JNIEnv *pEnv, jclass cls, jobject jpsiobj)
{
	if (jpsiobj == NULL)
		return (jint)NULL;
	
	jobject object = pEnv->NewGlobalRef(jpsiobj);
	CPDF_PSI::SetEnv(pEnv);
	CPDF_PSI::SetObject(object);
	
	FPSI_APPCALLBACK* pCallback = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPSI_APPCALLBACK),(FS_LPVOID*)&pCallback);
	if(nRet) throwException(pEnv,cls,nRet,"FPSIInitAppCallback: out of memory");
	LOGD("FPSIInitAppCallback 4 %i",nRet);
	return (jint)pCallback;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSIReleaseAppCallback
  (JNIEnv *pEnv, jclass cls, jint nPSIobj)
{
	if (nPSIobj == 0)
		return;

	FPSI_APPCALLBACK* pCallback = (FPSI_APPCALLBACK*)nPSIobj;
	FS_Memory_Free((void*)pCallback);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSIInitEnvironment
  (JNIEnv *pEnv, jclass cls, jint nPSIobj, jboolean bSimulation)
{
	if (nPSIobj == 0)
		return (jint)NULL;

	FPSI_APPCALLBACK* pCallback = (FPSI_APPCALLBACK*)nPSIobj;
	if (!pCallback)
		return (jint)NULL;

	//pCallback->m_Param = NULL;

	pCallback->Invalidate = CPDF_PSI::FPSI_Invalidate;
	pCallback->GetOpacity = CPDF_PSI::FPSI_GetOpacity;
	pCallback->clientData = pCallback;
	pCallback->lStructSize= sizeof(FPSI_APPCALLBACK);

	//FPSI_HANDLE hHandle = FPSI_InitEnvironment(pCallback, bSimulation);
	FPSI_HANDLE hHandle = NULL;
	FS_RESULT nRet = FPSI_InitEnvironment(pCallback, bSimulation, &hHandle);
	LOGI("InitPSIEnviroment ret = %x", nRet);
	return (jint)hHandle;
}

JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSIDestroyEnvironment
  (JNIEnv *pEnv, jclass cls, jint nPSIHandle)
{
	if (nPSIHandle == 0)
		return;

	pEnv->DeleteGlobalRef(CPDF_PSI::m_jPSIObj);

	FPSI_HANDLE hHandle = (FPSI_HANDLE)nPSIHandle;
	FPSI_DestroyEnvironment(hHandle);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSIInitCanvas
  (JNIEnv *pEnv, jclass cls, jint nPSIHandle, jint nWidth, jint nHeight)
{
	if (nPSIHandle == 0)
		return (jint)FS_ERR_PARAM;

	FPSI_HANDLE hHandle = (FPSI_HANDLE)nPSIHandle;	
	return (jint)FPSI_InitCanvas(hHandle, nWidth, nHeight);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSISetInkColor
  (JNIEnv *pEnv, jclass cls, jint nPSIHandle, jlong color)
{
	if (nPSIHandle == 0)
		return (jint)FS_ERR_PARAM;

	FPSI_HANDLE hHandle = (FPSI_HANDLE)nPSIHandle;
	return (jint)FPSI_SetInkColor(hHandle, color);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSISetInkDiameter
  (JNIEnv *pEnv, jclass cls, jint nPSIHandle, jint nDiameter)
{
	if (nPSIHandle == 0)
		return (jint)FS_ERR_PARAM;
	
	FPSI_HANDLE hHandle = (FPSI_HANDLE)nPSIHandle;

	return (jint)FPSI_SetInkDiameter(hHandle, nDiameter);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSIAddPoint
  (JNIEnv *pEnv, jclass cls, jint nPSIHandle, jfloat x, jfloat y, jfloat pressure, jint flag)
{
	CPDF_PSI::SetEnv(pEnv);
	if (nPSIHandle == 0)
		return (jint)FS_ERR_PARAM;

	FPSI_HANDLE hHandle = (FPSI_HANDLE)nPSIHandle;
	FS_RESULT nRet = FPSI_AddPoint(hHandle, x, y, pressure, flag);
	return (jint)nRet;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPSIRender
  (JNIEnv *pEnv, jclass cls, jint nPSIHandle, jint dib, jint left, jint top, jint right, jint bottom, jint src_left, jint src_top)
{
	if (nPSIHandle == 0)
		return (jint)FS_ERR_PARAM;
	
	FPSI_HANDLE hHandle = (FPSI_HANDLE)nPSIHandle;
	FS_BITMAP bitmap = (FS_BITMAP)dib;
	return (jint)FPSI_Render(hHandle, bitmap, left, top, right, bottom, src_left, src_top);
}

  
/***********************************************************************
*
*	Security module
*
***********************************************************************/

FS_LPVOID CreateHandler()
{
	

 	return NULL;
	
}

FS_DWORD GetPermissions(FS_LPVOID handler,FS_DWORD permission)
{
	return permission;
}

FS_LPVOID CreateFilter(FS_LPVOID handler, FS_INT32 objnum, FS_INT32 version)
{	

	return NULL;
}

FS_BOOL	FilterInput(FS_LPVOID filter, FPDF_DECRYPT_OUTPUT output, FS_LPCBYTE src_data, FS_DWORD src_len)
{
	
 	
	//decrypt data
	char* decryptBuf = new char[src_len];
	memcpy(decryptBuf, src_data, src_len);
	char* p = decryptBuf;
	for(int i=0; i<src_len; i++)
	{
		*p = *p -1;
		p++;
	}

	int bret = FPDF_Security_OutputDecrypted(output, decryptBuf, src_len);
	if(bret == FALSE)
	{
		delete []decryptBuf;
		return 0;
	}
	delete []decryptBuf;
	return 1;
}

FS_INT32 FinishFilter(FS_LPVOID filter, FPDF_DECRYPT_OUTPUT output)
{

	return 1;
}

FS_BOOL GetCryptInfo(FS_LPVOID handler,FS_INT32* cipher,FS_LPBYTE buffer,FS_LPDWORD  keylen)
{
	return false;
}

void FinishHandler(FS_LPVOID handler)
{

}

FS_DWORD EncryptGetSize(FS_LPVOID handler,FS_INT32 objnum, FS_INT32 version, FS_LPCBYTE src_buf, FS_DWORD src_size)
{
	
	return src_size;
}

FS_BOOL	EncryptContent(void* handler,int objnum, int version, FS_LPCBYTE src_buf, FS_DWORD src_size, FS_LPBYTE dest_buf, FS_DWORD* dest_size)
{
	//encrypt data
	memcpy(dest_buf, src_buf, src_size);
	for(unsigned int i=0; i<src_size; i++)
	{
		*dest_buf = *dest_buf + 1;
		dest_buf++;
	}
	*dest_size = src_size;
	return 1;
}


 JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityCreateSecurityHandler
  (JNIEnv *pEnv, jclass cls)
  {
  		FPDF_SECURITY_HANDLER* pSecurityHandler = new FPDF_SECURITY_HANDLER;
		pSecurityHandler->CreateFilter = CreateFilter;
		pSecurityHandler->CreateHandler = CreateHandler;
		pSecurityHandler->EncryptContent = EncryptContent;
		pSecurityHandler->EncryptGetSize = EncryptGetSize;
		pSecurityHandler->FilterInput = FilterInput;
		pSecurityHandler->FinishFilter = FinishFilter;
		pSecurityHandler->FinishHandler = FinishHandler;
		pSecurityHandler->GetCryptInfo = GetCryptInfo;
		pSecurityHandler->GetPermissions = GetPermissions;
		pSecurityHandler->lStructSize = sizeof(FPDF_SECURITY_HANDLER);
		return (jint)pSecurityHandler;
  }

class CPDFPKISecurityHandle:public FPDF_PKI_SECURITY_HANDLER
{
public:
		
		FS_BOOL    MyGetPubKeyVerify(const char* envelop,FS_INT32 len, unsigned char* seed_buf, FS_DWORD* nlen);
};



FS_BOOL    CPDFPKISecurityHandle::MyGetPubKeyVerify(const char* envelop,FS_INT32 len, unsigned char* seed_buf, FS_DWORD* nlen)
{
	//this ,customer must use their way to get the publick key.
	return TRUE;
}

FS_BOOL    MyGetPubKeyVerify(FS_LPVOID pThis,FS_LPCSTR envelop,FS_DWORD len,  FS_LPBYTE seed_buf, FS_LPDWORD nlen)
{
	CPDFPKISecurityHandle* pPKSecurityHandler = (CPDFPKISecurityHandle*)pThis;
	return pPKSecurityHandler->MyGetPubKeyVerify(envelop,len,seed_buf,nlen);
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityCreatePKISecurityHandler
  (JNIEnv *pEnv, jclass cls)
 {
  	FPDF_PKI_SECURITY_HANDLER* pPKIHandler = new FPDF_PKI_SECURITY_HANDLER;
  	pPKIHandler->GetPubKeyVerify = MyGetPubKeyVerify;
  	pPKIHandler->clientData = pPKIHandler;
  	pPKIHandler->lStructSize = sizeof(FPDF_PKI_SECURITY_HANDLER);
  	return (jint)pPKIHandler;
 }	


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityRegisterHandler
  (JNIEnv *pEnv, jclass cls, jstring handler_name, jint pHandler)
{
  	const char* handlername;
	handlername = pEnv->GetStringUTFChars(handler_name, NULL);
	return FPDF_Security_RegisterHandler(handlername,(void*)pHandler);	
		
}


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityUnRegisterHandler
  (JNIEnv *pEnv, jclass cls, jstring handler_name)
{
  	const char* handlername;
	handlername = pEnv->GetStringUTFChars(handler_name, NULL);
	return FPDF_Security_UnRegisterHandler(handlername);	
}


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityCustomEncrypt
  (JNIEnv *pEnv, jclass cls, jint pDoc, jstring handler_name, jint pHandler,jstring filepath)
{
  		FPDF_DOCUMENT pPDFDoc = (FPDF_DOCUMENT)pDoc;
  		const char* handlername;
		handlername = pEnv->GetStringUTFChars(handler_name, NULL);
		FPDF_SECURITY_HANDLER* pSecurityHandler = (FPDF_SECURITY_HANDLER*)pHandler;
		const char* file_path;
		file_path = pEnv->GetStringUTFChars(filepath, NULL);
		FILE* file = fopen(file_path, "wb+");
		LOGI("CustomEncrypt FILE* is %x",(int)file);
		FS_FILEWRITE fileWrite;
		fileWrite.GetSize = FileGetSize;
		fileWrite.WriteBlock = FileWriterBlock;
		fileWrite.Flush = FileFlush;
		fileWrite.clientData = file;
		
		FS_RESULT nRet  = FPDF_Security_CustomEncrypt(pPDFDoc, handlername,pSecurityHandler,&fileWrite);
		fclose(file);
		return nRet;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityRemove
  (JNIEnv *pEnv, jclass cls, jint pDoc, jstring filepath)
{
  	FPDF_DOCUMENT pPDFDoc = (FPDF_DOCUMENT)pDoc;
  	const char* file_path;
	file_path = pEnv->GetStringUTFChars(filepath, NULL);
	FILE* file = fopen(file_path, "wb+");
	FS_FILEWRITE fileWrite;
	fileWrite.GetSize = FileGetSize;
	fileWrite.WriteBlock = FileWriterBlock;
	fileWrite.Flush = FileFlush;
	fileWrite.clientData = file;
	
	FS_RESULT nRet =  FPDF_Security_Remove(pPDFDoc,&fileWrite);
	fclose(file);
	return nRet ;
}


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityGetPasswordLevel
  (JNIEnv *pEnv, jclass cls, jint pDoc)
{
  	FPDF_DOCUMENT pPDFDoc = (FPDF_DOCUMENT)pDoc;
  	FS_INT32 level = 0;
  	FS_RESULT nRet = FPDF_Security_GetPasswordLevel(pPDFDoc, &level);
  	return level;
}


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityCreateEnvelopes
  (JNIEnv *pEnv, jclass cls)
{
  	FPDF_ENVELOPES pEnvelopesH = NULL;
  	FS_RESULT nRet = FPDF_Security_CreateEnvelopes(&pEnvelopesH);
  	return (jint)pEnvelopesH;
}


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityAddEnvelope
  (JNIEnv *pEnv, jclass cls, jint envelopes, jbyteArray envelopeData, jint dataLen)
{
	FPDF_ENVELOPES pEnvelopes = (FPDF_ENVELOPES)envelopes;
	FS_LPVOID lpenvelopeData =	(FS_LPVOID)pEnv->GetByteArrayElements(envelopeData, NULL);
	return FPDF_Security_AddEnvelope(pEnvelopes,lpenvelopeData,dataLen);
}	


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityCertEncrypt
  (JNIEnv *pEnv, jclass cls, jint pDoc, jint nCipher, jboolean bEncryptMetaData, jint envelops, 
  										jbyteArray key, jint nKeyBytes, jstring filepath)
{
  	FPDF_DOCUMENT pPDFDoc = (FPDF_DOCUMENT)pDoc;
  	FS_LPCBYTE lpKey =	(FS_LPCBYTE)pEnv->GetByteArrayElements(key, NULL);
  	FPDF_ENVELOPES pEnvelops = (FPDF_ENVELOPES)envelops;
  	
  	const char* file_path;
	file_path = pEnv->GetStringUTFChars(filepath, NULL);
	FILE* file = fopen(file_path, "wb+");
	FS_FILEWRITE fileWrite;
	fileWrite.GetSize = FileGetSize;
	fileWrite.WriteBlock = FileWriterBlock;
	fileWrite.Flush = FileFlush;
	fileWrite.clientData = file;
	
	FS_RESULT nRet =  FPDF_Security_CertEncrypt(pPDFDoc,nCipher,bEncryptMetaData,pEnvelops,lpKey,nKeyBytes,&fileWrite);
	fclose(file);
	return nRet;
}


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityDestroyEnvelopes
  (JNIEnv *pEnv, jclass cls, jint envelops)
 {
  	FPDF_ENVELOPES pEnvelops = (FPDF_ENVELOPES)envelops;
  	return FPDF_Security_DestroyEnvelopes(pEnvelops);
 }


JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityDestroySecurityHandler
 (JNIEnv *pEnv, jclass cls, jint pHandler)
{
  	FPDF_SECURITY_HANDLER* pSecurityHandler = (FPDF_SECURITY_HANDLER*)pHandler;
	if(pSecurityHandler != NULL)
		delete pSecurityHandler;
}


JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSecurityDestroyPKISecurityHandler
  (JNIEnv *pEnv, jclass cls, jint pPKIHandler)
{
	FPDF_PKI_SECURITY_HANDLER* pPKISecurityHandler = (FPDF_PKI_SECURITY_HANDLER*)pPKIHandler;
	if(pPKISecurityHandler != NULL)
		delete pPKISecurityHandler;
}

JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFileWriteAlloc
  (JNIEnv *pEnv, jclass cls, jstring filepath)
{
	
	FS_FILEWRITE* fileWrite = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FS_FILEWRITE),(FS_LPVOID*)&fileWrite);
	if(nRet != FS_ERR_SUCCESS) 
		throwException(pEnv,cls,FS_ERR_MEMORY,"FSFileWriteAlloc: out of memory.");
	fileWrite->GetSize = FileGetSize;
	fileWrite->WriteBlock = FileWriterBlock;
	fileWrite->Flush = FileFlush;
	const char* file_path;
	file_path = pEnv->GetStringUTFChars(filepath, NULL);
	LOGI("Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFileWriteAlloc: file_path = %s",file_path);
	void* file = fopen(file_path, "wb");
	LOGI("Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFileWriteAlloc: file = %x",file);
	
	void* fileTest = fopen("/data/data/com.foxitsample.annotations/FoxitTest.txt","wb+");
	LOGI("Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFileWriteAlloc: fileTest = %x",fileTest);
	
	fileWrite->clientData = file;
	fileWrite->lStructSize = sizeof(FS_FILEWRITE);
	return (jint)fileWrite;
}


JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FSFileWriteRelease
  (JNIEnv *pEnv, jclass cls, jint filewrite)
{
	FS_FILEWRITE* pFileWrite = (FS_FILEWRITE*)filewrite;
	FILE* pFile = (FILE*)pFileWrite->clientData;
	fclose(pFile);
	FS_Memory_Free(pFileWrite);
}


JNIEXPORT int JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureAdd
  (JNIEnv *pEnv, jclass cls, jint doc, jint page, jobject rect)
{
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	FPDF_PAGE pPage = (FPDF_PAGE)page;
	FS_RECTF* pRect = NULL;
	LOGI("begin Alloc");
	FS_Memory_Alloc(sizeof(FS_RECT),(FS_LPVOID*)&pRect);
	LOGI("end Alloc,pRect is %x",pRect);
	if(pRect == NULL) 
		throwException(pEnv,cls,FS_ERR_MEMORY,"FPDFSignatureAdd: out of memory.");
	
	jclass cls_r = pEnv->GetObjectClass(rect);
	LOGI("cls_r IS %x",cls_r);
	jfieldID left = pEnv->GetFieldID( cls_r, (const char*)"left", "F");
	jfieldID right = pEnv->GetFieldID(cls_r, (const char*)"right", "F");
	jfieldID top = pEnv->GetFieldID(cls_r, (const char*)"top", "F");
	jfieldID bottom = pEnv->GetFieldID( cls_r, (const char*)"bottom", "F");	
	float nLeft = pEnv->GetFloatField( rect, left);
	float nRight = pEnv->GetFloatField( rect, right);
	float nTop = pEnv->GetFloatField( rect, top);
	float nBottom = pEnv->GetFloatField( rect, bottom);
   
    pRect->left = nLeft;
	pRect->right = nRight;
	pRect->top = nTop;
	pRect->bottom = nBottom;
	LOGI("begin add");
	FPDF_SIGNATURE_FIELD signHandler= NULL;
	FS_RESULT nRet = FPDF_Signature_Add(pDoc,pPage,*pRect,&signHandler);
	LOGI("end add");
	if(nRet != FS_ERR_SUCCESS )
		throwException(pEnv,cls,nRet,"FPDFSignatureAdd: signature did not add properly");
	return (jint)signHandler;
}

FS_LPWSTR js2w(JNIEnv* env, jstring str) 
{  
	int len = env->GetStringLength(str);   
	FS_WCHAR *w_buffer = new FS_WCHAR[len+1];  
	if (w_buffer == NULL)
	{	
		return NULL;	
	}
	memset(w_buffer, 0x00, (len+1)*sizeof(unsigned short));
	const jchar *cstr = env->GetStringChars(str, NULL);	
	if (cstr == NULL)	
	{	
		delete []w_buffer;
		return NULL;
		
	}	
	//Attention: sizeof wchar_t == 4
	for (int i=0; i < len; i++)	
	{	
		w_buffer[i] = cstr[i];	
	}	
	env->ReleaseStringChars(str, cstr);	
	return w_buffer;      
	
}
/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetSigner
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetSigner
  (JNIEnv *pEnv, jclass cls, jint sigField, jstring signer)
{
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_LPWSTR pwstrSigner = js2w(pEnv,signer);
	FS_RESULT nRet = FPDF_Signature_SetSigner(pSigField,pwstrSigner);
	if(nRet != FS_ERR_SUCCESS )
	{
		delete pwstrSigner;
		throwException(pEnv,cls,nRet,"FPDFSignatureSetSigner: signature did not set signer properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetDN
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetDN
  (JNIEnv *pEnv, jclass cls, jint sigField, jstring dn)
{
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_LPWSTR pwstrDN = js2w(pEnv,dn);
	FS_RESULT nRet = FPDF_Signature_SetDN(pSigField,pwstrDN);
	if(nRet != FS_ERR_SUCCESS )
	{
		delete pwstrDN;
		throwException(pEnv,cls,nRet,"FPDFSignatureSetDN:  signature did not set dn properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetDate
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetDate
  (JNIEnv *pEnv, jclass cls, jint sigField, jstring date)
{
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_LPWSTR pwstrData = js2w(pEnv,date);
	FS_RESULT nRet = FPDF_Signature_SetDate(pSigField,pwstrData);
	if(nRet != FS_ERR_SUCCESS )
	{
		delete pwstrData;
		throwException(pEnv,cls,nRet,"FPDFSignatureSetDate:  signature did not set date properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetText
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetText
  (JNIEnv *pEnv, jclass cls, jint sigField, jstring text)
{
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_LPWSTR pwstrText = js2w(pEnv,text);
	FS_RESULT nRet = FPDF_Signature_SetText(pSigField,pwstrText);
	if(nRet != FS_ERR_SUCCESS )
	{
		delete pwstrText;
		throwException(pEnv,cls,nRet,"FPDFSignatureSetText:  signature did not set text properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetReason
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetReason
  (JNIEnv *pEnv, jclass cls, jint sigField, jstring reason)
{
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_LPWSTR pwstrReason = js2w(pEnv,reason);
	FS_RESULT nRet = FPDF_Signature_SetReason(pSigField,pwstrReason);
	if(nRet != FS_ERR_SUCCESS )
	{
		delete pwstrReason;
		throwException(pEnv,cls,nRet,"FPDFSignatureSetReason:  signature did not set reason properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetLocation
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetLocation
  (JNIEnv *pEnv, jclass cls, jint sigField, jstring location)
{
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_LPWSTR pwstrLocation = js2w(pEnv,location);
	FS_RESULT nRet = FPDF_Signature_SetLocation(pSigField,pwstrLocation);
	if(nRet != FS_ERR_SUCCESS )
	{
		delete pwstrLocation;
		throwException(pEnv,cls,nRet,"FPDFSignatureSetLocation:  signature did not set location properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetOption
 * Signature: (II)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetOption
  (JNIEnv *pEnv, jclass cls, jint sigField, jint option)
{
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_RESULT nRet = FPDF_Signature_SetOption(pSigField,option);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureSetOption:  signature did not set option properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetImageType
 * Signature: (II)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetImageType
  (JNIEnv *pEnv, jclass cls, jint sigField, jint imagetype)
{
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_RESULT nRet = FPDF_Signature_SetImageType(pSigField,imagetype);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureSetImageType:  signature did not set option properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSetImageData
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSetImageData
  (JNIEnv *pEnv, jclass cls, jint sigField, jstring imagepath) 
{
	const char* lpimagepath ;
	lpimagepath = pEnv->GetStringUTFChars(imagepath, NULL);
	LOGI("lpimagepath is %s",lpimagepath);
	FILE* pFile = fopen(lpimagepath,"rb");
	LOGI("pFile is %x",pFile);
	if(pFile == NULL) 
		throwException(pEnv,cls,FS_ERR_PARAM,"FPDFSignatureSetImageData:  signature did not set iamgedata properly");
	fseek(pFile,0,SEEK_END);
	FS_DWORD nFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	FS_LPBYTE pbImageData = new FS_BYTE[nFileSize];
	memset(pbImageData,0,nFileSize);
	fread(pbImageData,sizeof(unsigned char),nFileSize,pFile);
	fclose(pFile);
	LOGI("begin FPDF_Signature_SetImageData");
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_RESULT nRet = FPDF_Signature_SetImageData(pSigField,nFileSize,pbImageData);
	LOGI("end FPDF_Signature_SetImageData");
	if(nRet != FS_ERR_SUCCESS )
	{
		delete pbImageData;
		throwException(pEnv,cls,nRet,"FPDFSignatureSetImageData:  signature did not set iamgedata properly");
	}
	delete pbImageData;
}


FS_RESULT gSignDocument(FS_LPVOID clientData, FPDF_SIGNATURE_FIELD* sigField, FS_LPBYTE srcbuffer, 
									FS_DWORD dwSize, FS_LPBYTE* signedbuffer, FS_DWORD* dwSignedSize)
{
	const char* buffer = "Foxit Test Data";
    int size = strlen(buffer);
    char* csp = (char*)malloc(size);
    int i;
    
    //encrypt the buffer
    for(i=0;i<size;i++)
        csp[i] = buffer[i]+i;
    
    *signedbuffer = (unsigned char*)csp;
    *dwSignedSize = size;
	return FS_ERR_SUCCESS;
}

FS_RESULT gVerifySignature(FS_LPVOID clientData, FPDF_SIGNATURE_FIELD* sigField, FS_LPBYTE srcbuffer,
                           FS_DWORD dwSize, FS_LPBYTE signedbuffer, FS_DWORD dwSignedSize, int* state)
{
	LOGI("gVerifySignature BEGIN");
    FPDF_SIGNATURE_HANDLER* pSignedHandler = (FPDF_SIGNATURE_HANDLER* )clientData;
    FS_WCHAR* filter = pSignedHandler->filter;
    FS_LPCWSTR str = (FS_LPCWSTR)L"Foxit.Example";
    int bEqu = memcmp(filter, str, strlen("Foxit.Example") * 2);
    if(bEqu)
        return FS_ERR_ERROR;
    
    filter = pSignedHandler->subfilter;
    str = (FS_LPCWSTR)L"Foxit.Example.Subfilter";
    bEqu = memcmp(filter, str, strlen("Foxit.Example.Subfilter") * 2);
    if(bEqu)
        return FS_ERR_ERROR;
    
    FS_LPBYTE buffer = (FS_LPBYTE)malloc(dwSignedSize);
    
    int i;
    for(i = 0; i < dwSignedSize; i++)
        buffer[i] = signedbuffer[i] - i;
	
    bEqu = memcmp(buffer, "Foxit Test Data", dwSignedSize);
    LOGI("gVerifySignature end");
    if(bEqu)
        return FS_ERR_ERROR;
    
    return FS_ERR_SUCCESS;
}


/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureAllocHandler
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureAllocHandler
  (JNIEnv *pEnv, jclass cls)
{
	FPDF_SIGNATURE_HANDLER* pSignatureHandler = NULL;
	FS_RESULT nRet = FS_Memory_Alloc(sizeof(FPDF_SIGNATURE_HANDLER),(FS_LPVOID*)&pSignatureHandler);
	if(!pSignatureHandler)
		throwException(pEnv,cls,FS_ERR_MEMORY,"FPDFSignatureAllocHandler: out of memory");
	pSignatureHandler->SignDocument = gSignDocument;
	pSignatureHandler->VerifySignature = gVerifySignature;
	pSignatureHandler->clientData = pSignatureHandler;
	memset(pSignatureHandler->filter, 0, 128); 
	memset(pSignatureHandler->subfilter, 0, 128); 
	memcpy(pSignatureHandler->filter, L"Foxit.Example", 26); 
	memcpy(pSignatureHandler->subfilter, L"Foxit.Example.Subfilter", 46); 
	return (jint)pSignatureHandler;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureReleaseHandler
 * Signature: (I)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureReleaseHandler
  (JNIEnv *pEnv, jclass cls, jint signaturehandler)
{
	FPDF_SIGNATURE_HANDLER* pSignatureHandler = (FPDF_SIGNATURE_HANDLER*)signaturehandler;
	FS_Memory_Free(pSignatureHandler);
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureSign
 * Signature: (IIII)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureSign
  (JNIEnv *pEnv, jclass cls, int doc,int sigField,int sigHandler,int filewrite)
{
	FS_FILEWRITE* pFileWrite = (FS_FILEWRITE*)filewrite;
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	FPDF_SIGNATURE_HANDLER* pSignatureHandler = (FPDF_SIGNATURE_HANDLER*)sigHandler;
	FPDF_SIGNATURE_FIELD pSignField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_RESULT nRet = FPDF_Signature_Sign(pDoc,pSignField,pSignatureHandler,pFileWrite);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureSign:  signature did not sign properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureDelete
 * Signature: (II)I
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureDelete
  (JNIEnv *pEnv, jclass cls, jint doc, jint sigField)
{
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	FPDF_SIGNATURE_FIELD pSigField = (FPDF_SIGNATURE_FIELD)sigField;
	FS_RESULT nRet = FPDF_Signature_Release(pDoc, pSigField);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureDelete:  signature did not delete properly");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureCount
  (JNIEnv *pEnv, jclass cls , jint doc)
{
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	int nCount = 0;
	FS_RESULT nRet = FPDF_Signature_GetCount(pDoc,&nCount);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureCount:  signature did not get count properly");
	}
	return nCount;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureGetByIndex
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureGetByIndex
  (JNIEnv *pEnv, jclass cls, jint doc, jint index)
{
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	FPDF_SIGNATURE_FIELD pSignatureField = NULL;
	FS_RESULT nRet = FPDF_Signature_Get(pDoc,index,&pSignatureField);
	LOGI("nRet = %x",nRet);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureGetByIndex:  signature did not get Signature properly");
	}
	return (jint)pSignatureField;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureVerify
 * Signature: (III)I
 */
JNIEXPORT jboolean JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureVerify
  (JNIEnv *pEnv, jclass cls, jint doc, jint sigField, jint sigHandler)
{
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	FPDF_SIGNATURE_FIELD pSignField = (FPDF_SIGNATURE_FIELD)sigField;
	FPDF_SIGNATURE_HANDLER* pSignHandler = (FPDF_SIGNATURE_HANDLER*)sigHandler;
	FS_RESULT nRet = FPDF_Signature_Verify(pDoc, pSignField, pSignHandler);
	LOGI("nRet = %x",nRet);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureVerify:  signature did not verify properly");
		return FALSE;
	}

	return TRUE;
}


JNIEXPORT jint JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureIsSigned
  (JNIEnv *pEnv, jclass cls, jint doc, jint sigfield)
{
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	FS_BOOL bSigned = FALSE;
	FPDF_SIGNATURE_FIELD pSignField = (FPDF_SIGNATURE_FIELD)sigfield;
	FS_RESULT nRet = FPDF_Signature_IsSigned(pDoc, pSignField,&bSigned);
	//LOGI("bSigned = %x",bSigned);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureIsSigned:  have some errors");
	}
	return (jint)bSigned;
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureClear
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureClear
  (JNIEnv *pEnv, jclass cls, jint doc, jint sigfield)
{
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	FPDF_SIGNATURE_FIELD pSignField = (FPDF_SIGNATURE_FIELD)sigfield;
	FS_RESULT nRet = FPDF_Signature_Clear(pDoc, pSignField);
	LOGI("nRet = %x",nRet);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureClear:  have some errors");
	}
}

/*
 * Class:     FoxitEMBSDK_EMBJavaSupport
 * Method:    FPDFSignatureRemove
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_FPDFSignatureRemove
  (JNIEnv *pEnv, jclass cls, jint doc, jint sigfield)
{
	FPDF_DOCUMENT pDoc = (FPDF_DOCUMENT)doc;
	FPDF_SIGNATURE_FIELD pSignField = (FPDF_SIGNATURE_FIELD)sigfield;
	FS_RESULT nRet = FPDF_Signature_Remove(pDoc, pSignField);
	LOGI("nRet = %x",nRet);
	if(nRet != FS_ERR_SUCCESS )
	{
		throwException(pEnv,cls,nRet,"FPDFSignatureClear:  have some errors");
	}
}

extern "C" void setPage(int newPageHandle, int newPageStartX, int newPageEndX, int newPageStartY, int newPageEndY);
extern "C" void closeDoc();

/*
 * Class:     com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport
 * Method:    setPage
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_setPage
  (JNIEnv *, jclass, jint newPageHandle, jint newPageStartX, jint newPageEndX, jint newPageStartY, jint newPageEndY)
{
	setPage(newPageHandle, newPageStartX, newPageEndX, newPageStartY, newPageEndY);
}

/*
 * Class:     com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport
 * Method:    closeDoc
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_polycom_mfw_pdf_FoxitEMBSDK_EMBJavaSupport_closeDoc
  (JNIEnv *, jclass)
{
	closeDoc();
}
