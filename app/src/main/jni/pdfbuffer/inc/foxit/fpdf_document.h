/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPDFDOC" module to be enabled in your SDK license.
  * @file	fpdf_document.h
  * @author	Foxit Corporation
  * @brief	Header file for the Document module.<br>
  *			The document module offers:<br>
  *			1. Basic methods for doing some operation on documents and pages, such as loading, parsing, saving and closing.<br>
  *			2. Methods for getting some basic information of documents or pages.<br>
  *			3. Methods for getting useful information of bookmarks.<br>
  *			<br>
  *			Before doing operations on documents or pages, users should load documents or pages first.
  *			And when finishing their operations, users should remeber to close pages or docuemnts in time.
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Document module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPDFDOC Document
 * @brief Methods in this module are included in fpdf_document.h .
 */
/**@{*/

#ifndef _FPDF_DOCUMENT_H_
#define _FPDF_DOCUMENT_H_

#ifndef _FS_BASE_H_
#include "fs_base.h"
#endif

#ifndef _FPDF_BASE_H_
#include "fpdf_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Load a PDF document
 *
 * @details	Document loading is a progressive process. It might take a long time to
 *			load a document, especially when a file is corrupted, and it will try to
 *			recover the document contents by scanning the whole file. <br>
 *
 * @param[in] file			A pointer to a ::FS_FILEREAD structure.
 *							This structure must be kept valid as long as the document is open.
 * @param[in] password		A pointer to a zero-terminated byte string, for the password.
 *							Or <b>NULL</b> for no password.
 * @param[out] pDocument	Pointer to a <b>FPDF_DOCUMENT</b> structure that receives the loaded document handle.
 *
 * @return	::FS_ERR_SUCCESS means load the document successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Doc_Load(FS_FILEREAD* file, FS_LPCSTR password, FPDF_DOCUMENT* pDocument);

/**
 * @brief	Close a PDF document and release all associated resources.
 *
 * @param[in] document		The document handle.
 *
 * @return	::FS_ERR_SUCCESS means close the document successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Doc_Close(FPDF_DOCUMENT document);

/**
 * @brief	Save the modified document to a new file.
 *
 * @param[in] document		The document handle.
 * @param[in] file			A pointer to a ::FS_FILEWRITE structure.
 * @param[in] flag			The saving flags. See macro definitions:
 *							<ul>
 *							<li>::FPDF_SAVEAS_INCREMENTAL</li>
 *							<li>::FPDF_SAVEAS_NOORIGINAL</li>
 *							</ul>
 * @param[in] pause			A callback mechanism allowing the document saving process
 *							to be paused before it's finished. <br>This can be <b>NULL</b> if you
 *							don't want to pause.
 *
 * @return	::FS_ERR_SUCCESS means save the document successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Doc_SaveAs(FPDF_DOCUMENT document, FS_FILEWRITE* file, FS_INT32 flag, FS_PAUSE* pause);

/**
 * @brief	Get the information string about the document, such as the creator, the modification date, etc.
 *
 * @details	The string is output in Unicode, using UTF-16LE format. It's terminated by
 *			two consecutive zero bytes.<br>
 *			If the parameter <i>buffer</i> is <b>NULL</b>, then the parameter <i>bufsize</i> will receive
 *			the count of bytes required to store the string (including the two-byte terminator).
 *
 * @param[in] document		A handle to the document
 * @param[in] key			A byte string for the information key.<br>
 *							Currently it can be one of the followings:
 *							<ul>
 *							<li>"Title"</li> 
 *							<li>"Author"</li>
 *							<li>"Subject"</li>
 *							<li>"Keywords"</li>
 *							<li>"Creator"</li>
 *							<li>"Producer"</li>
 *							<li>"CreationDate"</li>
 *							<li>"ModDate"</li>
 *							<li>Some other custom information keys, if they're supported by the PDF file.</li>
 *							</ul>
 * @param[out] buffer		A buffer allocated by the application, or <b>NULL</b>.
 * @param[in,out] bufsize	Pointer to a number that indicates the buffer size (count of bytes),
 *							before this function is called. Then after the function returns, this paremeter will store
 *							the actual count of bytes of the meta text.
 *
 * @return	::FS_ERR_SUCCESS means get the information string successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Doc_GetMetaText(FPDF_DOCUMENT document, FS_LPCSTR key, FS_LPVOID buffer, FS_LPDWORD bufsize);

/**
 * @brief	Get permission flags specified by a PDF document
 *
 * @param[in] document		A document handle returned by the function ::FPDF_Doc_Load.
 * @param[out] permission	Pointer to a number that receives the permission flag. See macro definitions <b>FPDF_PERMISSION_XXX</b>.<br>
 *							Document permissions are set bitwise in the return value.<br>
 *
 * @return	::FS_ERR_SUCCESS means get the permission successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Doc_GetPermissions(FPDF_DOCUMENT document, FS_LPDWORD permission);

/**
 * @brief	Get the count of pages in the document.
 * 
 * @param[in] document		A document handle.
 * @param[out] count		Pointer to an integer that receives the count of the pages.
 * @return	::FS_ERR_SUCCESS means get the count of pages successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *			
 */
FS_RESULT FPDF_Doc_GetPageCount(FPDF_DOCUMENT document, FS_INT32* count);

/**
 * @brief	Reload a document after it has been modified .
 *
 * @details The document reloading is a progressive process. It might take a long time to
 *			reload the document.<br>
 *			Actually, this function just parses the document again after it has been modified.<br>
 *
 * @param[in] document		A document handle 
 * @param[in] file			Pointer to a ::FS_FILEREAD structure.
 *							This structure must be kept valid before next reloading.
 * @param[in] password		Pointer to a zero-terminated byte string, for the password.
 *							Or <b>NULL</b> for no password.
 *
 * @return	::FS_ERR_SUCCESS means reload the document successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Doc_ReloadDocument(FPDF_DOCUMENT document, FS_FILEREAD* file, FS_LPCSTR password);

/**
 * @brief Set the size of the internal buffer, used to read from source file.
 *
 * @details Currently EMB SDK uses 512 bytes as the default buffer size. The new buffer size 
 *			takes effect the next time you call ::FPDF_Doc_Load.
 *
 * @param[in] size			The count of bytes, used to set to be the new buffer size.
 *
 * @return None.
 */
void FPDF_Doc_SetFileBufferSize(FS_INT32 size);

/**
 * @brief	Add a mark string on the special page. Only English text string is supported now.
 *
 * @param[in] document		A document handle returned by the function ::FPDF_Doc_Load.
 * @param[in] page			A page handle.
 * @param[in] x				The value of the X position in the PDF page coordination system.
 * @param[in] y				The value of the Y position in the PDF page coordination system.
 * @param[in] fontsize		The font size which is to be used to write the string.<br>
 * @param[in] string		The text string which is to be written on the page.<br>
 *
 * @return	::FS_ERR_SUCCESS means add the mark string successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Doc_AddModifyMark(FPDF_DOCUMENT document, FPDF_PAGE page, FS_FLOAT x, FS_FLOAT y, FS_INT32 fontsize, FS_LPCSTR string);

/**
 * @brief	Get the page mode of the document, which specifies how the document should be displayed when opened. 
 *
 * @param[in] document		A document handle returned by the function ::FPDF_Doc_Load .
 *
 * @return	The page mode of the document. See macro definitions <b>FPDF_PAGEMODE_XXX</b>.
 */
FS_INT32 FPDF_Doc_GetPageMode(FPDF_DOCUMENT document);

/**
 * @brief	Load the specified page.
 *
 * @param[in] document		A document handle.
 * @param[in] index			A zero-based index of the page which is to be loaded.
 * @param[out] page			Pointer to a <b>FPDF_PAGE</b> structure that receives the loaded page handler.
 *
 * @return	::FS_ERR_SUCCESS means load the page successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_Load(FPDF_DOCUMENT document, FS_INT32 index, FPDF_PAGE* page);

/**
 * @brief Close a page and release all related resources
 *
 * @param[in] page		A page handle.
 *
 * @return	::FS_ERR_SUCCESS means close the page successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_Close(FPDF_PAGE page);

/**
 * @brief Start parsing a page, so it can be rendered or searched.
 *
 * @details Parsing is a progressive process. This function starts the parsing process,
 *			and may return before the parsing is finished, if a ::FS_PAUSE structure is provided.<br>
 *			Application should call ::FPDF_Page_ContinueParse repeatedly to finish the parsing
 *			when return value is ::FS_ERR_TOBECONTINUED.<br>
 *			There can be only one parsing procedure active for a page, and if a page
 *			has already been parsed, you can't start a parsing again.
 *
 * @param[in] page			A page handle
 * @param[in] text_only		A boolean value used to decide whether to parsing texts only (used for searching) or not.
 * @param[in] pause			A ::FS_PAUSE structure that can pause the parsing process.
 *							Or <b>NULL</b> if you don't want to pause the process.
 *
 * @return	::FS_ERR_SUCCESS means parsing the page successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_StartParse(FPDF_PAGE page, FS_BOOL text_only, FS_PAUSE* pause);

/**
 * @brief Continue parsing the page.
 *
 * @details ::FPDF_Page_StartParse should be called first before render the page or search on the page.
 *			Application should call ::FPDF_Page_ContinueParse repeatedly to finish the parsing
 *			when its return value is ::FS_ERR_TOBECONTINUED.
 *
 * @param[in] page			A page handle
 * @param[in] pause			A ::FS_PAUSE structure that can pause the parsing process.
 *							Or <b>NULL</b> if you don't want to pause the process.
 *
 * @return	::FS_ERR_SUCCESS means parsing successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_ContinueParse(FPDF_PAGE page, FS_PAUSE* pause);


/**
 * @brief Get an estimated parsing progress in percentage.
 *
 * @param[in] page			A Page handle
 *
 * @return  An integer between 0 and 100 (inclusive) indicating the parsing progress.
 *			The result is just a rough estimation.<br>
 *			-1 means parameter error.
 */
FS_INT32 FPDF_Page_GetParseProgress(FPDF_PAGE page);


/**
 * @brief Get the complexity estimate before parsing a page to help decide how to handle page operations.
 *
 * @details Size of the page content stream returned by this function
 *          may not accurately reflect the complexity of the page because the page content
 *          stream may be inline content which artificially inflates the size.
 *
 * @param[in]	page		A page handle
 * @param[out]	size		Pointer to an integer that receives the size of page content stream
 *
 * @return	::FS_ERR_SUCCESS means get the estimate successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_EstimatePageComplexity(FPDF_PAGE page, FS_INT32* size);

/**
 * @brief Get the page label.
 *
 * @details The string is output in Unicode, using UTF-16LE format. It's terminated by
 *			two consecutive zero bytes.<br>
 *			If the parameter <i>buffer</i> is <b>NULL</b>, then the parameter <i>bufsize</i> will receive
 *			the count of bytes required to store the string (including the two-byte terminator).
 *			If the paramter <i>bufsize</i>(the size of <i>buffer</i>) is smaller than the required size, then this function
 *			will not copy any data, return ::FS_ERR_SUCCESS, and the required buffer size will 
 *			also be put in the parameter <i>bufsize</i>.
 *
 * @param[in] document		A document handle.
 * @param[in] nPage			The index of the page label.
 * @param[out] buffer		Pointer to a Unnicode buffer allocated by the application.<br>This can be <b>NULL</b> in order to get the required size in <i>bufsize</i>.
 * @param[in,out] bufsize	Pointer to a number indicating the buffer size (count of bytes),
 *							before this function is called. After this function returns, this place will store
 *							the count of bytes used by the output (including theterminator).
 *
 * @return	::FS_ERR_SUCCESS means get the page label successfully or just get the actual size of the label successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_GetPageLabel(FPDF_DOCUMENT document, FS_INT32 nPage, FS_LPVOID buffer, FS_LPDWORD bufsize);

/**
 * @brief Get the index of the page by the specified page label.
 *
 * @param[in] document		A document handle.
 * @param[in] bIsNoCase		A boolean value used to decide whether to find the page label without case-sensitivity or not.
 * @param[in] pagelabel		A zero-terminated unicode page label string to be found. 
 * @param[out] pageNum		Pointer to an integer that receives the found page index.
 *
 * @return	::FS_ERR_SUCCESS means get the index of the page by the specified page label successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_PageLabelToPageNum(FPDF_DOCUMENT document, FS_BOOL bIsNoCase, FS_LPCWSTR pagelabel,FS_INT32* pageNum);

/**
 * @brief Get the type of an action.
 *
 * @details Each different type of the destination has different data structure. The result of the parameter <i>data_size</i> 
 *			indicates how many bytes is required to hold the destination data structure. The application
 *			can then allocate sufficient buffer and then call ::FPDF_Action_GetData function to
 *			get the real data.
 *
 * @param[in] document		A document handle.
 * @param[in] action		An action handle.
 * @param[out] dest_type	Pointer to an integer that receives the type of the destination. See macro definitions <b>FPDF_DEST_xxx</b>.
 * @param[out] data_size	Pointer to a number that receives the size of the data block for the destination.
 *							If this parameter is <b>NULL</b>, then the data size won't be retrieved.
 *
 * @return	::FS_ERR_SUCCESS means get the type successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Action_GetType(FPDF_DOCUMENT document, FPDF_ACTION action, FS_INT32* dest_type, FS_LPDWORD data_size);

/**
 * @brief Get the detailed data of a particular action.
 *
 * @details The following data structures are used for different action types returned by ::FPDF_Action_GetType:
 *			<ul>
 *			<li>::FPDF_DEST</li>
 *			<li>::FPDF_DOCDEST</li>
 *			<li>::FPDF_URLDEST</li>
 *			<li>::FPDF_LAUNCHACTION</li>
 *			</ul>
 *			See the definitions of each structure above for more details.
 *			Please note the actual action data might use more space than the structure definition
 *			shows, in order to store things like file name or URL. So you should always call
 *			::FPDF_Action_GetType first to get the data size then allocate enough buffer
 *			for calling this function.
 *
 * @param[in] document		A document handle.
 * @param[in] action		An action handle.
 * @param[out] buffer		Pointer to a buffer(allocated by the appication) that receives the destination data.
 *
 * @return	::FS_ERR_SUCCESS means get the data successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Action_GetData(FPDF_DOCUMENT document, FPDF_ACTION action, FS_LPVOID buffer);

/**
 * @brief Get the next action in an action chain.
 *
 * @param[in] action		A handle to the current action
 * @param[out] next			Pointer to a <b>FPDF_ACTION</b> structure that receives the handle to the next action.
 *
 * @return	::FS_ERR_SUCCESS means get the next action successfully or there's no next action, with the parameter <i>next</i> set to <b>NULL</b>.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Action_GetNext(FPDF_ACTION action, FPDF_ACTION* next);

/**
 * @brief Get the first child of a bookmark item, or the first top level bookmark item.
 *
 * @param[in] document		A document handle.
 * @param[in] parent		A handle to the parent bookmark. 
 *							This can be <b>NULL</b> if you want to get the first top level item.
 * @param[out] bookmark		Pointer to a <b>FPDF_BOOKMARK</b> structure that receives the handle to the first child or the first top level bookmark item. 
 *							If it is <b>NULL</b> as a result, it means the bookmark is not found.
 *
 * @return	::FS_ERR_SUCCESS means get the first child of the input bookmark or the top level bookmark successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Bookmark_GetFirstChild(FPDF_DOCUMENT document, FPDF_BOOKMARK parent, FPDF_BOOKMARK* bookmark);

/**
 * @brief Get the next sibling of a bookmark item.
 *
 * @param[in] document		A document handle.
 * @param[in] bookmark		A bookmark handle.
 * @param[out] sibling		Pointer to a <b>FPDF_BOOKMARK</b> structure that receives the handle of the next sibling.
 *							If it is <b>NULL</b> as a result, it means the input bookmark  is the last bookmark in this level.
 *
 * @return	::FS_ERR_SUCCESS means get the next sibling of the input bookmark successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Bookmark_GetNextSibling(FPDF_DOCUMENT document, FPDF_BOOKMARK bookmark, FPDF_BOOKMARK* sibling);

/**
 * @brief Get the title of a bookmark.
 *
 * @details The title is output in Unicode, using UTF-16LE format. It's terminated by
 *			two consecutive zero bytes.<br>
 *			If the parameter <i>buffer</i> is <b>NULL</b>, then the paramter <i>bufsize</i> will receive
 *			the count of bytes required to store the bookmark title (including the two-
 *			byte terminator).<br>
 *			If the paramter <i>bufsize</i>(the size of <i>buffer</i>) is smaller than the required size, then this function
 *			will not copy any data, return ::FS_ERR_SUCCESS, and the required buffer size will 
 *			also be set to the parameter <i>bufsize</i>.
 *
 * @param[in] bookmark		A bookmark handle.
 * @param[out] buffer		Pointer to a Unicode buffer allocated by the application. <br>This can be <b>NULL</b> in order to get the required size in <i>bufsize</i>.
 * @param[in,out] bufsize	Pointer to a number indicating the size of <i>buffer</i> before this function is called. <br>
 *							After this function returns, this parameter will store
 *							the count of bytes used by the output (including the terminator).
 *
 * @return	::FS_ERR_SUCCESS means get the title of the bookmark successfully or just get the actual size of the title successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Bookmark_GetTitle(FPDF_BOOKMARK bookmark, FS_LPVOID buffer, FS_LPDWORD bufsize);

/**
 * @brief Get the index of a page, to which the input bookmark is pointed.
 *
 * @details Some bookmarks might not point to a page, while some bookmarks might have more than one destination
 *			(action). To get detailed information about a bookmark, you should call ::FPDF_Bookmark_GetAction.<br>
 *			This function only checks the first action of the input bookmark. If the first action doesn't point to a page,
 *			the return value will be ::FS_ERR_PARAM and the parameter will be set to be -1.
 *
 * @param[in] document		A document handle.
 * @param[in] bookmark		A bookmark handle.
 * @param[out] page			Pointer to an integer receives the index of the page.<br> -1 means the input bookmark doesn't actually
 *							point to a page inside the document.
 *
 * @return	::FS_ERR_SUCCESS means get index of the destination page successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Bookmark_GetPage(FPDF_DOCUMENT document, FPDF_BOOKMARK bookmark, FS_INT32* page);

/**
 * @brief Get the action(s) associated with a particular bookmark
 *
 * @param[in] document		A document handle
 * @param[in] bookmark		A bookmark handle
 * @param[out] action		Pointer to a <b>FPDF_ACTION</b> structure that receives the handle of the first action
 *
 * @return	::FS_ERR_SUCCESS means get the actions(s) successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Bookmark_GetAction(FPDF_DOCUMENT document, FPDF_BOOKMARK bookmark, FPDF_ACTION* action);

#ifdef __cplusplus
};
#endif

#endif
// _FPDF_DOCUMENT_H_
/**@}*/

