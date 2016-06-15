/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPDFFORMFIELD" module to be enabled in your SDK license.
  * @file	fpdf_formfield.h
  * @author	Foxit Corporation
  * @brief	Header file for the Form Field module.<br>
  *			The form field module offers methods which implement the following functions:<br>
  *			1. Support to manually fill a PDF form.<br>
  *			2. Support to execute javascript by forms.<br>
  *			3. Allow setting or retrieving field values of a form from applications.<br>
  *			4. Support to export the content of a form to a XML file and import the XML file into the form as well.<br>
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Form Field module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPDFFORMFIELD Form Field
 * @brief Definitions and Methods in this module are included in fpdf_formfield.h and fpdf_fwlevent.h .
 */
/**@{*/

#ifndef _FPDF_FORMFIELD_H_
#define _FPDF_FORMFIELD_H_

#ifndef _FS_BASE_H_
#include "fs_base.h"
#endif

#ifndef _FPDF_BASE_H_
#include "fpdf_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _FS_DEF_FORMFILLJSPLATFORM_
#define _FS_DEF_FORMFILLJSPLATFORM_
/** @brief Structure for formfill JS platform. */
typedef struct _FPDF_FORMFILL_JSPLATFORM_
{

	/** @brief The size of the data structure. It must be set to <b>sizeof</b>(::FPDF_FORMFILL_JSPLATFORM). */
	FS_DWORD	lStructSize; 

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/** 
	 * @brief Pop up a dialog to show warnings or hints.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] Msg			A string containing the message to be displayed.
	 * @param[in] Title			The title of the dialog.
	 * @param[in] Type			The type of button group. Its value can be:
	 *							<ul>
	 *							<li>0: OK;(default value.)</li>
	 *							<li>1: OK, Cancel;</li>
	 *							<li>2: Yes, NO; </li>
	 *							<li>3: Yes, NO, Cancel.</li>
	 *							</ul>
	 * @param[in] Icon			The Icon type. Its value can be: 
	 *							<ul>
	 *							<li>0: Error;(default value.)</li>
	 *							<li>1: Warning;</li>
	 *							<li>2: Question;</li>
	 *							<li>3: Status.</li>
	 *							</ul>
	 *
	 * @return	The return value could be the following type:
	 *			<ul>
	 *			<li>1: OK;</li>
	 *			<li>2: Cancel; </li>
	 *			<li>3: NO;</li>
	 *			<li>4: Yes;</li>
	 *			</ul>
	 */
	FS_INT32	(*Alert)(FS_LPVOID clientData, FS_LPCWSTR Msg, FS_LPCWSTR Title, FS_INT32 Type, FS_INT32 Icon);
	
	/** 
	 * @brief	Cause the system to play a sound. 
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] nType			The sound type. Its value can be:
	 *							<ul>
	 *							<li>0: Error</li>
	 *							<li>1: Warning</li>
	 *							<li>2: Question</li>
	 *							<li>3: Status</li>
	 *							<li>4: Default (default value)</li>
	 *							</ul>
	 *
	 * @return	None.
	 */
	void		(*Beep)(FS_LPVOID clientData, FS_INT32 nType);
	
	
	/** 
	 * @brief	Display a dialog box containing a question and an entry field for the user to reply to the question.  
	 *
	 * @details	No matter on what platform, the response should be always input in UTF-16LE encoding.
	 *			The return value always indicates count of bytes required for the buffer, even when there is
	 *			no buffer specified, or the buffer size is less then required. In this case, the buffer will not
	 *			be modified.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] Question		The question to be posed to the user.
	 * @param[in] Title			The title of the dialog box.
	 * @param[in] Default		A default value for the answer to the question. 
	 *							If the answer is not specified, no default value is presented.
	 * @param[in] cLabel		A short string to appear in front of the edit text field. 
	 * @param[in] bPassword		<b>TRUE</b> means that the user's response should show as asterisks (*) or bullets (?) 
	 *							to mask the response, which might be sensitive information. <br>
	 *							The default is <b>FALSE</b>.		
	 * @param[out] response		A string buffer used to receive the user's response. It can be <b>NULL</b>.
	 * @param[in,out] length	The length of the buffer, count of bytes. It can be 0.
	 *
	 * @return	Count of bytes required for the buffer <i>response</i>, including trailing zeros.
	 */
	FS_INT32	(*Response)(FS_LPVOID clientData, FS_LPCWSTR Question, FS_LPCWSTR Title, FS_LPCWSTR Default, 
							FS_LPCWSTR cLabel, FS_BOOL bPassword, FS_LPVOID response, FS_DWORD length);
	
	/*
	 * @brief	Get the file path of the current document. 
	 *
	 * @details	The parameter <i>filePath</i> should be always input in local encoding.
	 *			The return value always indicates count of bytes required for the buffer, even when there is
	 *			no buffer specified, or the buffer size is less then required. In this case, the buffer will not
	 *			be modified.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[out] filePath		The string buffer used to receive the file path. It can be <b>NULL</b>.
	 * @param[in,out] length	The length of the buffer, count of bytes. It can be 0.
	 *
	 * @return	Number of bytes the filePath consumes, including trailing zeros.
	 */
	FS_INT32	(*GetFilePath)(FS_LPVOID clientData, FS_LPVOID filePath, FS_DWORD length);
	
	
	/*
	 * @brief	Mail the data buffer as an attachment to all recipients, with or without user interaction. 
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] mailData		A pointer to the data buffer to be sent. It can be <b>NULL</b>.
	 * @param[in] length		The size(in bytes) of the buffer <i>mailData</i>. It can be 0.
	 * @param[in] bUI			<b>TRUE</b> means that the rest of the parameters are used in a compose-new-message window 
	 *							which is displayed to the user.<br>
	 *							<b>FALSE</b> means that the parameter <i>To</i> is required and all others are optional.
	 * @param[in] To			A semicolon-delimited list of recipients for the message.
	 * @param[in] Subject		The subject of the message. The limit of the length is 64 KB.
	 * @param[in] CC			A semicolon-delimited list of CC recipients for the message. 
	 * @param[in] BCC			A semicolon-delimited list of BCC recipients for the message. 
	 * @param[in] Msg			The content of the message. The limit of the length is 64 KB.
	 *
	 * @return	None.
	 *
	 * @note If the parameter <i>mailData</i> is <b>NULL</b> or <i>length</i> is 0, 
	 *		the current document will be mailed as an attachment to all recipients.
	 */
	void		(*Mail)(FS_LPVOID clientData, FS_LPVOID mailData, FS_DWORD length, FS_BOOL bUI, FS_LPCWSTR To, 
					    FS_LPCWSTR Subject, FS_LPCWSTR CC, FS_LPCWSTR BCC, FS_LPCWSTR Msg); 

	/*
	 * @brief	Print all or a specific number of pages of the document.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] bUI			<b>TRUE</b> means that a UI will be presented to the user to obtain printing information and confirm the action.
	 * @param[in] nStart		A zero-based index of the start of an inclusive range of pages.
	 * @param[in] nEnd			A zero-based index of the end of an inclusive page range.
	 * @param[in] bSilent		<b>TRUE</b> means that the cancel dialog box will be suppressed while the document is printing. 
	 *							The default is <b>FALSE</b>.
	 * @param[in] bShrinkToFit	<b>TRUE</b> means that the page is shrunk (if necessary) to fit within the imageable area of the printed page.
	 * @param[in] bPrintAsImage	<b>TRUE</b> means that it will print pages as an image.
	 * @param[in] bReverse		<b>TRUE</b> means that it will print from <i>nEnd</i> to <i>nStart</i>.
	 * @param[in] bAnnotations	<b>TRUE</b> means that annotations will be printed.The default is <b>TRUE</b>.
	 *
	 * @return	None.
	 */
	void		(*Print)(FS_LPVOID clientData, FS_BOOL bUI, FS_INT32 nStart, FS_INT32 nEnd, FS_BOOL bSilent ,
					     FS_BOOL bShrinkToFit, FS_BOOL bPrintAsImage , FS_BOOL bReverse , FS_BOOL bAnnotations);

	/*
	 * @brief	Send the form data to a specified URL.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] formData		A pointer to the data buffer which is to be sent.
	 * @param[in] length		The size(in bytes), of the buffer <i>formData</i>.
	 * @param[in] URL			The URL, to which the form data will be sent.
	 *
	 * @return	None.
	 */
	void		(*SubmitForm)(FS_LPVOID clientData, FS_LPVOID formData, FS_DWORD length, FS_LPCWSTR URL);
	
	/*
	 * @brief	Show a file selection dialog, and return the selected file path.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[out] filePath		A pointer to the data buffer used to receive the file path. It can be <b>NULL</b>.
	 * @param[in,out] length	The length of the buffer, count of bytes. It can be 0.
	 *
	 * @return	Count of bytes the filePath consumes, including trailing zeros.
	 *
	 * @note		The <i>filePath</i> shoule be always inputted in the local encoding.
	 */
	FS_INT32	(*Browse)(FS_LPVOID clientData, FS_LPVOID filePath, FS_DWORD length);

	/** @brief pointer to ::FPDF_FORMFILL_INFO.*/
	FS_LPVOID	formfillinfo;

}FPDF_FORMFILL_JSPLATFORM;
#endif

/** @name Macro Definitions for Cursor Type. */
/**@{*/
#define FS_CURSOR_ARROW	0	/**< @brief Cursor Type: Arrow. */
#define FS_CURSOR_NESW	1	/**< @brief Cursor Type: NESW. */
#define FS_CURSOR_NWSE	2	/**< @brief Cursor Type: NWSE. */
#define FS_CURSOR_VBEAM	3	/**< @brief Cursor Type: Vertical Beam. */
#define FS_CURSOR_HBEAM	4	/**< @brief Cursor Type: Horizontal Beam. */
#define FS_CURSOR_HAND	5	/**< @brief Cursor Type: Hand. */
/**@}*/

/**
 * @brief Declares of a pointer type to the callback function for the FFI_SetTimer method.
 *
 * @param[in] idEvent			Identifier of the timer. 
 *
 * @return None. 
 */	
typedef void	(*FS_LPFTimerCallback)(FS_INT32 idEvent);

#ifndef _FS_DEF_SYSTEMTIME_
#define _FS_DEF_SYSTEMTIME_
/** @brief Declares of a struct type to the local system time. */
typedef struct _FPDF_SYSTEMTIME_ 
{
    FS_WORD wYear;			/**< brief years since 1900 */
    FS_WORD wMonth;			/**< brief months since January - [0,11] */
    FS_WORD wDayOfWeek;		/**< brief days since Sunday - [0,6] */
    FS_WORD wDay;			/**< brief day of the month - [1,31] */
    FS_WORD wHour;			/**< brief hours since midnight - [0,23] */
    FS_WORD wMinute;		/**< brief minutes after the hour - [0,59] */
    FS_WORD wSecond;		/**< brief seconds after the minute - [0,59] */
    FS_WORD wMilliseconds;	/**< brief milliseconds after the second - [0,999] */
}FPDF_SYSTEMTIME;
#endif

#ifndef _FS_DEF_FORMFILLINFO_
#define _FS_DEF_FORMFILLINFO_
/** @brief Structure for formfill information. */
typedef struct  _FPDF_FORMFILL_INFO_
{	
	/** @brief The size of the data structure. It must be set to <b>sizeof</b>(::FPDF_FORMFILL_INFO). */
	FS_DWORD		lStructSize; 
	
	/** @brief The user-supplied data. */
	FS_LPVOID		clientData;

	/** 
	 * @brief Give implementation a chance to release any data after the interface is no longer used.
	 *
	 * @details Called by Foxit SDK during the final cleanup process.
	 *
	 * @param[in] clientData	Pointer to the interface structure itself
	 *
	 * @return None
	 */

	void			(*Release)(FS_LPVOID clientData);

	/** 
	 * @brief Invalidate the client area within the specified rectangle.
	 *
	 * @details All positions are measured in PDF "user space".
	 *			Implementation should call ::FPDF_RenderPage_Start function for repainting a specified page area.
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] page			Handle to the page. Returned by ::FPDF_Page_Load.
	 * @param[in] left			Left position of the client area in PDF page coordinate.
	 * @param[in] top			Top  position of the client area in PDF page coordinate.
	 * @param[in] right			Right position of the client area in PDF page  coordinate.
	 * @param[in] bottom		Bottom position of the client area in PDF page coordinate.
	 *
	 * @return None.
	 *
	 */
	void			(*Invalidate)(FS_LPVOID clientData,FPDF_PAGE page, FS_FLOAT left, FS_FLOAT top, 
								  FS_FLOAT right, FS_FLOAT bottom);
	
	/** 
	 * @brief When user is taking the mouse to select texts on a form field, this callback function will keep 
	 *			returning the selected areas to the implementation.
	 *
	 * @details This CALLBACK function is useful for implementing special text selection effect. Implementation should
	 *			first records the returned rectangles, then draw them one by one at the painting period, last,remove all 
	 *			the recorded rectangles when finish painting.
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] page			Handle to the page. Returned by ::FPDF_Page_Load.
	 * @param[in] left			Left position of the client area in PDF page coordinate.
	 * @param[in] top			Top  position of the client area in PDF page coordinate.
	 * @param[in] right			Right position of the client area in PDF page  coordinate.
	 * @param[in] bottom		Bottom position of the client area in PDF page coordinate.
	 *
	 * @return None.
	 *
	 */
	void			(*OutputSelectedRect)(FS_LPVOID clientData,FPDF_PAGE page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom);

	/** 
	 * @brief Set the Cursor shape.
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] nCursorType	Cursor type. see Flags for Cursor type for the details.
	 *
	 * @return 		None.
	 */
	void			(*SetCursor)(FS_LPVOID clientData, FS_INT32 nCursorType);

	/** 
	 * @brief This method installs a system timer. A time-out value is specified, 
	 *			and every time a time-out occurs, the system passes a message to
	 *			the TimerProc callback function. 
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] uElapse		Specifies the time-out value, in milliseconds.
	 * @param[in] lpTimerFunc	A pointer to the callback function-TimerCallback.
	 *
	 * @return The timer identifier of the new timer if the function is successful.
	 *		   An application passes this value to the FFI_KillTimer method to kill 
	 *		   the timer. Nonzero if it is successful; otherwise, it is zero.
	 */
	FS_INT32		(*SetTimer)(FS_LPVOID clientData, FS_INT32 uElapse, FS_LPFTimerCallback lpTimerFunc);

	/** 
	 * @brief This method kills the timer event identified by nIDEvent, set by an earlier call to FFI_SetTimer. 
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] nTimerID		The timer ID return by FFI_SetTimer function.
	 *
	 * @return None.
	 */
	void			(*KillTimer)(FS_LPVOID clientData, FS_INT32 nTimerID);


	/** 
	 * @brief This method receives the current local time on the system. 
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 *
	 * @return A system time structure.
	 */
	FPDF_SYSTEMTIME (*GetLocalTime)(FS_LPVOID clientData);

	/** 
	 * @brief This method will be invoked to notify implementation when the value of any FormField on the document had been changed.
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 *
	 * @return None.
	 */
	void			(*OnChange)(FS_LPVOID clientData);

	/** 
	 * @brief This method receives the page pointer associated with a specified page index.
	 *
	 * @details In some cases, the document-level JavaScript action may refer to a page which hadn't been loaded yet.
	 *		To successfully run the javascript action, implementation need to load the page for SDK.
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] document		Handle to document. Returned by ::FPDF_Doc_Load.
	 * @param[in] nPageIndex  	Index number of the page. 0 for the first page.
	 *
	 * @return Handle to the page. Returned by ::FPDF_Page_Load function.
	 */
	FPDF_PAGE		(*GetPage)(FS_LPVOID clientData, FPDF_DOCUMENT document, FS_INT32 nPageIndex);

	/** 
	 * @brief This method receives the current page pointer.
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] document		Handle to document. Returned by ::FPDF_Doc_Load.
	 *
	 * @return Handle to the page. Returned by ::FPDF_Page_Load function.
	 */
	FPDF_PAGE		(*GetCurrentPage)(FS_LPVOID clientData, FPDF_DOCUMENT document);

	/** 
	 * @brief This method receives currently rotation of the page view.
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] page			Handle to page. Returned by ::FPDF_Page_Load.
	 *
	 * @return	The page rotation. This should be:(in a clockwise direction)<br>
	 *			<ul>
	 *			<li>0: 0 degree</li>
	 *			<li>1: 90 degree</li>
	 *			<li>2: 180 degree</li>
	 *			<li>3: 270 degree</li>
	 *			</ul>
	 */
	FS_INT32		(*GetRotation)(FS_LPVOID clientData, FPDF_PAGE page);

	/** 
	 * @brief This method will execute an named action.
	 *
	 * @details See the named actions description of <<PDF Reference, version 1.7>> for more details.  
	 *
	 * @param[in] clientData	Pointer to the interface structure itself.
	 * @param[in] namedAction	A byte string which indicates the named action, terminated by 0.
	 *
	 * @return None.
	 */
	void			(*ExecuteNamedAction)(FS_LPVOID clientData, FS_LPCSTR namedAction);
	
	/** 
	* @brief This method will be called when the focus is set to a formfield.
	*
	* @param[in] clientData		Pointer to the interface structure itself.
	* @param[in] field			A handler to the form field.
	* @param[in] focusText		The text on the focus.
	* @param[in] nTextLen		The length of the focus text.
	*
	* @return None.
	*
	* @note Currently,only support text field and combobox field.
	* */
	void			(*OnSetFieldInputFocus)(FS_LPVOID clientData,FPDF_FORMFIELD field, FS_LPCWSTR focusText, FS_DWORD nTextLen);

	/** @brief pointer to ::FPDF_FORMFILL_JSPLATFORM interface */
	FPDF_FORMFILL_JSPLATFORM*	jsPlatform;

} FPDF_FORMFILL_INFO;
#endif

/** @name Macro Definitions for Additional Action Types of Document. */
/**@{*/
#define FPDFDOC_AACTION_WC		0x10		/**< @brief WC, before closing document, JavaScript action. */
#define FPDFDOC_AACTION_WS		0x11		/**< @brief WS, before saving document, JavaScript action. */
#define FPDFDOC_AACTION_DS		0x12		/**< @brief DS, after saving document, JavaScript action. */
#define FPDFDOC_AACTION_WP		0x13		/**< @brief WP, before printing document, JavaScript action. */
#define FPDFDOC_AACTION_DP		0x14		/**< @brief DP, after printing document, JavaScript action. */
/**@}*/

/** @name Macro Definitions for Additional Action Types of Page */
/**@{*/
#define FPDFPAGE_AACTION_OPEN		0		/**< @brief /O -- An action to be performed when the page is opened */
#define FPDFPAGE_AACTION_CLOSE		1		/**< @brief /C -- An action to be performed when the page is closed */
/**@}*/

/** @name Macro Definitions for Form Field Types */
/**@{*/
#define FPDF_FORMFIELD_UNKNOWN			0	/**< @brief Unknown. */
#define FPDF_FORMFIELD_PUSHBUTTON		1	/**< @brief push button type. */
#define FPDF_FORMFIELD_CHECKBOX			2	/**< @brief check box type. */
#define FPDF_FORMFIELD_RADIOBUTTON		3	/**< @brief radio button type. */
#define FPDF_FORMFIELD_COMBOBOX			4	/**< @brief combo box type. */
#define FPDF_FORMFIELD_LISTBOX			5	/**< @brief list box type. */
#define FPDF_FORMFIELD_TEXTFIELD		6	/**< @brief text field type. */
/**@}*/

/**
 * @brief Init form fill environment. 
 *
 * @details This function should be called before any form fill operation.
 *
 * @param[in]  document			Handle to document. Returned by ::FPDF_Doc_Load.
 * @param[in]  formInfo			Pointer to a ::FPDF_FORMFILL_INFO structure.
 * @param[out] hFormEnv			The handler to the form fill module. <b>NULL</b> means fails. 
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_InitEnvironment(FPDF_DOCUMENT document, FPDF_FORMFILL_INFO* formInfo, FPDF_FORMENV* hFormEnv);

/**
 * @brief Exit form fill environment. 
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_ExitEnvironment(FPDF_FORMENV hFormEnv);

/**
 * @brief This method is required for implementing all the form related functions. Should be invoked after user 
 *			successfully loaded a PDF page, and method ::FPDF_FormFill_InitEnvironment had been invoked.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnAfterLoadPage(FPDF_FORMENV hFormEnv, FPDF_PAGE page);

/**
 * @brief This method is required for implementing all the form related functions. Should be invoked before user 
 *			close the PDF page.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnBeforeClosePage(FPDF_FORMENV hFormEnv, FPDF_PAGE page);

/**
 * @brief This method is required for performing Document-level JavaScript action. It should be invoked after the PDF document
 *		  had been loaded.
 *
 * @details If there is Document-level JavaScript action embedded in the document, this method will execute the javascript action;
 *			otherwise, the method will do nothing.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_DoDocumentJSAction(FPDF_FORMENV hFormEnv);

/**
 * @brief This method is required for performing open-action when the document is opened.
 *
 * @details This method will do nothing if there is no open-actions embedded in the document. 
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_DoDocumentOpenAction(FPDF_FORMENV hFormEnv);
 
/**
 * @brief This method is required for performing the document's additional-action.
 *
 * @details This method will do nothing if there is no document additional-action corresponding to the specified aaType.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] aaType			The additional action type of a document . See macro definitions <b>FPDFDOC_AACTION_XXX</b>.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_DoDocumentAAction(FPDF_FORMENV hFormEnv, FS_INT32 aaType);

/**
 * @brief This method is required for performing the page object's additional-action when opened or closed.
 *
 * @details This method will do nothing if no additional-action corresponding to the specified aaType exists.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] aaType	        The additional action type of a page. See macro definitions <b>FPDFPAGE_AACTION_XXX</b>.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_DoPageAAction(FPDF_FORMENV hFormEnv, FPDF_PAGE page,  FS_INT32 aaType);

/**
 * @brief You can call this member function when the mouse cursor moves. 
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 * @param[in] page_x			Specifies the x-coordinate of the cursor in PDF user space. 
 * @param[in] page_y			Specifies the y-coordinate of the cursor in PDF user space.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnMouseMove(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 eventflag, FS_FLOAT page_x, FS_FLOAT page_y);

/**
 * @brief You can call this member function when a user rotates the mouse wheel. 
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG. 
 * @param[in] deltaX			Indicates horizontal distance rotated. 
 * @param[in] deltaY			Indicates vertical distance rotated. 
 * @param[in] page_x			Specifies the x-coordinate of the cursor in PDF user space. 
 * @param[in] page_y			Specifies the y-coordinate of the cursor in PDF user space.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnMouseWheel(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 eventflag, 
									 FS_FLOAT deltaX, FS_FLOAT deltaY, FS_FLOAT page_x, FS_FLOAT page_y);

/**
 * @brief You can call this member function when the user presses the left mouse button.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG. 
 * @param[in] page_x			Specifies the x-coordinate of the cursor in PDF user space. 
 * @param[in] page_y			Specifies the y-coordinate of the cursor in PDF user space.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnLButtonDown(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 eventflag, 
									  FS_FLOAT page_x, FS_FLOAT page_y);

/**
 * @brief You can call this member function when the user releases the left mouse button.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG. 
 * @param[in] page_x			Specifies the x-coordinate of the cursor in device. 
 * @param[in] page_y			Specifies the y-coordinate of the cursor in device.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnLButtonUp(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 eventflag, 
									FS_FLOAT page_x, FS_FLOAT page_y);

/**
 * @brief You can call this member function when a non-system key is pressed. 
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] nKeyCode			Indicates whether various virtual keys are down. 
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnKeyDown(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 nKeyCode, FS_INT32 eventflag);

/**
 * @brief You can call this member function when a non-system key is released. 
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] nKeyCode			The virtual-key code of the given key.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnKeyUp(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 nKeyCode, FS_INT32 eventflag);

/**
 * @brief You can call this member function when a keystroke translates to a non-system character.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] nChar				The character code value of the key.  
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnChar(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 nChar, FS_INT32 eventflag);

/**
 * @brief You can call this member function to set text string to a focused text field, when is in the input status.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] wText				The input text string in UTF-16LE format.
 * @param[in] nTextLen			Number of characters(not bytes) of the input text string.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnSetText(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_LPCWSTR wText, FS_DWORD nTextLen, FS_INT32 eventflag);

/**
 * @brief Kill the current focus on the form field.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_OnKillFocus(FPDF_FORMENV hFormEnv);

/**
 * @name The following functions are not essential for FormFiller implementation, which are not implemented yet.
 * We add them here as supplement of FormFiller implementation.
 */
/**@{*/
/**
 * @brief You can call this member function when the user FS_FLOAT-clicks the left mouse button.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 * @param[in] page_x			Specifies the x-coordinate of the cursor in device. 
 * @param[in] page_y			Specifies the y-coordinate of the cursor in device.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note This funtion is not implemented yet,it is possible to implement it in the future!
 */
FS_RESULT FPDF_FormFill_OnLButtonDblClk(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 eventflag, 
										FS_FLOAT page_x, FS_FLOAT page_y);

/**
 * @brief You can call this member function when the user presses the right mouse button.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 * @param[in] page_x			Specifies the x-coordinate of the cursor in device. 
 * @param[in] page_y			Specifies the y-coordinate of the cursor in device.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note This funtion is not implemented yet,it is possible to implement it in the future!
 */
FS_RESULT FPDF_FormFill_OnRButtonDown(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 eventflag, 
									  FS_FLOAT page_x, FS_FLOAT page_y);

/**
 * @brief You can call this member function when the user releases the right mouse button.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 * @param[in] page_x			Specifies the x-coordinate of the cursor in device. 
 * @param[in] page_y			Specifies the y-coordinate of the cursor in device.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note This funtion is not implemented yet,it is possible to implement it in the future!
 */
FS_RESULT FPDF_FormFill_OnRButtonUp(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 eventflag,
									FS_FLOAT page_x, FS_FLOAT page_y);

/**
 * @brief You can call this member function when the user click the right mouse button. 
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in] eventflag			Indicates the status of various virtual keys, default is 0. See enumeration ::FWL_EVENTFLAG.
 * @param[in] page_x			Specifies the x-coordinate of the cursor in device. 
 * @param[in] page_y			Specifies the y-coordinate of the cursor in device.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note This funtion is not implemented yet,it is possible to implement it in the future!
 */
FS_RESULT FPDF_FormFill_OnRButtonDblClk(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_INT32 eventflag, 
										FS_FLOAT page_x, FS_FLOAT page_y);
/**@}*/

/**
 * @brief Check the form filed position by point.
 *
 * @param[in]  hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in]  page				Handle to the page. Returned by ::FPDF_Page_Load.
 * @param[in]  page_x			X position in PDF "user space".
 * @param[in]  page_y			Y position in PDF "user space".
 * @param[out] fieldtype		The type of the formfield; -1 indicates no fields.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_HasFormFieldAtPoint(FPDF_FORMENV hFormEnv, FPDF_PAGE page, FS_FLOAT page_x, FS_FLOAT page_y, FS_INT32* fieldtype);

/**
 * @brief Set the highlight color of specified or all the form fields in the document.
 *
 * @details When the parameter fieldType is set to zero, the highlight color will be applied to all the form fields in the 
 *			document.<br>
 *			Please refresh the client window to show the highlight immediately if necessary.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] fieldType			A 32-bit integer indicating the type of a form field(defined above).
 * @param[in] color				The highlight color of the form field. Constructed by 0xxxrrggbb.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_SetFormFieldHighlightColor(FPDF_FORMENV hFormEnv, FS_INT32 fieldType, FS_ARGB color);

/**
 * @brief Set the transparency of the form field highlight color in the document.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] alpha				The transparency of the form field highlight color.(between 0-255)
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_SetFormFieldHighlightAlpha(FPDF_FORMENV hFormEnv, FS_BYTE alpha);

/**
 * @brief Remove the form field highlight color in the document.
 *
 * @details Please refresh the client window to remove the highlight immediately if necessary.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_RemoveFormFieldHighlight(FPDF_FORMENV hFormEnv);

/**
 * @brief Run a javascript. 			
 *
 * @details This method is only for javascript testing.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] javascript		The javascript string,terminated by NULL character (unicode 0).
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_RunJS(FPDF_FORMENV hFormEnv, FS_LPCWSTR javascript);

/**
 * @brief Render FormFeilds(widgets) on a page to a device independent bitmap. 	
 *
 * @details This method is designed to only render FormFields on the page. 
 *			With '::FPDF_RENDER_ANNOT|::FPDF_RENDER_NOWIDGET' specified for flags, rendering functions (like ::FPDF_RenderPage_Start) will only render page contents and annotations without widgets to a bitmap.
 *			In order to implement the FormFill functions,Implementation should call this method after rendering functions finish rendering the page contents and annotations.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 * @param[in] bitmap			Handle to the device independent bitmap (as the output buffer).
 *								Bitmap handle can be created by ::FS_Bitmap_Create .
 * @param[in] page				Handle to the page. Returned by ::FPDF_Page_Load .
 * @param[in] start_x			Left pixel position of the display area in the device coordinate.
 * @param[in] start_y			Top pixel position of the display area in the device coordinate.
 * @param[in] size_x			Horizontal size (in pixels) for displaying the page.
 * @param[in] size_y			Vertical size (in pixels) for displaying the page.
 * @param[in] rotate			Page orientation:<br>
 *								<ul>
 *								<li>0 : normal</li>
 *								<li>1 : rotated 90 degrees clockwise</li>
 *								<li>2 : rotated 180 degrees</li>
 *								<li>3 : rotated 90 degrees counter-clockwise</li>
 *								</ul>
 * @param[in] flags				0 means normal display. This can also be the combination of macro definitions <b>FPDF_RENDER_XXX</b>.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 * @see Macro Definitions <b>FPDF_RENDER_XXX</b>.
 */
FS_RESULT FPDF_FormFill_Draw(FPDF_FORMENV hFormEnv, FS_BITMAP bitmap, FPDF_PAGE page, FS_INT32 start_x, FS_INT32 start_y, 
							 FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, FS_INT32 flags);

/**
 * @brief Update the form appearance. 
 *
 * @details In certain circumstances, When the value of formfield is changed, the associated appearance of formfield
 *			may not be drew immediately. In that case ,you may need to invoke this function to update the form appearance immediately.
 *
 * @param[in] hFormEnv			Handle to the form fill module. Returned by ::FPDF_FormFill_InitEnvironment.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormFill_Update(FPDF_FORMENV hFormEnv);

/**
 * @brief Construct a form from a document. The updateAP parameter specifies whether we need to 
 * regenerate appearance streams for the fields.
 *
 * @param[in]  document			The PDF document.
 * @param[in]  updateAP			Whether we need to regenerate appearance streams for fields.
 * @param[out] interform		A handle to the interactive form. If this function is failed, return <b>NULL</b>.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Form_Create(FPDF_DOCUMENT document, FS_BOOL updateAP, FPDF_FORM* interform);

/**
 * @brief Destroy the interactive form.
 *
 * @param[in] interform			A handle to the interactive form.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Form_Release(FPDF_FORM interform);

/**
 * @brief Export the form data to a XML file
 *
 * @param[in] interform			The handler to the interactive form.
 * @param[in] filewrite			A pointer to a custom ::FS_FILEWRITE structure.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Form_ExportToXML(FPDF_FORM interform, FS_FILEWRITE filewrite);

/**
 * @brief Import the form data from a XML file
 *
 * @param[in] interform			The handler to the interactive form.
 * @param[in] fileread			A pointer to a ::FS_FILEREAD structure.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Form_ImportFromXML(FPDF_FORM interform, FS_FILEREAD fileread);

/**
 * @brief Get the count of specified fields.
 *
 * @param[in] interform			The handler to the interactive form.
 * @param[in] fieldname			The field name to be matched.<br>
 *								If this is empty, that means for all fields; otherwise the system will match the field name.<br>
 *								As the shortest string, for example, text1 will match text1.0, text1.2.0, etc. 
 *								But text1 will not match test10 or test11.1 . 
 * @param[out] count			The count of fields with specified field name to be matched. 
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Form_CountFields(FPDF_FORM interform, FS_LPCWSTR fieldname, FS_INT32* count);

/**
 * @brief Get a form field.
 *
 * @param[in] interform			The handler to the interactive form.
 * @param[in] fieldindex		The zero-based field index in the matched field array.
 * @param[in] fieldname			The field name to be matched.<br>
 *								If this is empty, that means for all fields; otherwise the system will match the field name.<br>
 *								As the shortest string, for example, text1 will match text1.0, text1.2.0, etc. 
 *								But text1 will not match test10 or test11.1 . 
 * @param[out] field			A handler to the form field.
 *	
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Form_GetField(FPDF_FORM interform, FS_INT32 fieldindex, FS_LPCWSTR fieldname, FPDF_FORMFIELD* field);

/** 
 * @brief Get the field flags. 
 *
 * @param[in] formfield			The handler to the form field.
 * @param[out] flag				The field flags. See macro definitions <b>FPDF_FIELDFLAG_XXX</b>.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormField_GetFlags(FPDF_FORMFIELD formfield, FS_LPDWORD flag);

/** 
 * @brief Get the field type.
 *
 * @param[in] formfield			The handler to the form field.
 * @param[out] fieldType		The field type. See macro definitions <b>FPDF_FORMFIELD_XXX</b>. 
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormField_GetType(FPDF_FORMFIELD formfield, FS_LPDWORD fieldType);

/** 
 * @brief Get the full name of the field.
 *
 * @param[in] formfield			The handler to the form field.
 * @param[out] namebuf			A unicode buffer to receive the name string.
 * @param[in,out] buflen		Number of characters (not bytes) for the buffer, excluding an additional terminator.
 *								If parameter <i>namebuf</i> is <b>NULL</b>, return number of characters (not bytes) needed,
 *								otherwise, return number of characters copied into the buffer.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormField_GetFullName(FPDF_FORMFIELD formfield, FS_LPWSTR namebuf, FS_LPDWORD buflen);

/** 
 * @brief Get the value of the field.
 *
 * @param[in]		formfield	The handler to the form field.
 * @param[out]		valuebuf	A unicode buffer to receive the value string.
 * @param[in,out]	buflen		Number of characters (not bytes) for the buffer, excluding an additional terminator.
 *								If valuebuf is NULL, return number of characters (not bytes) needed,
 *								otherwise, return number of characters copied into the buffer.
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_FormField_GetValue(FPDF_FORMFIELD formfield, FS_LPWSTR valuebuf, FS_LPDWORD buflen);

/**
 * @brief Set the value of the field. not applicable to non-unison radio box.
 *
 * @param[in] formfield			The handler to the form field.
 * @param[in] value				The input field value, UTF16-LE format, terminated by zero.
 * @param[in] notify			Whether do notifying. We do not care about this parameter for the moment,could be true or false.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	
 *  <ul>
 *  <li>For a text field, the value is a text string;</li>
 *  <li>For a radio button or check box field, the value is the state of the selected button, <b>on</b> or <b>off</b>;</li>
 *  <li>For a list box field, the value is the value of the first selected item, if there's any;</li>
 *  <li>For a comb box field, the value is a text string.</li>
 *  </ul>
 */
FS_RESULT FPDF_FormField_SetValue(FPDF_FORMFIELD formfield, FS_LPCWSTR value, FS_BOOL notify);

#ifdef __cplusplus
};
#endif

#endif
// _FPDF_FORMFIELD_H_
/**@}*/
