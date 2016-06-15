/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPDFANNOT" module to be enabled in your SDK license.
  * @file	fpdf_annot.h
  * @author	Foxit Corporation
  * @brief	Header file for the Annotation module.<br>
  *			The annot module offers methods to do some operations on annotations.
  *			Following the specification of PDF, users can retrieve the annotation in the current position 
  *			as well as add, delete or modify annotations by using these methods.
  *			This module currently supports annotations of these types: fileattachment, stamp, pencil, highlight, note and link annotation.
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Annotation module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPDFANNOT Annotation
 * @brief Methods in this module are included in fpdf_annot.h
 */
/**@{*/

#ifndef _FPDF_ANNOT_H_
#define _FPDF_ANNOT_H_

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
 * @brief	Get the count of hyperlinks inside a page.
 *
 * @param[in] page			A page handle.
 * @param[out] link_count	Pointer to an integer that receives the count of links.
 *
 * @return	::FS_ERR_SUCCESS means get the count successfully.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function only supports the link annotation.
 */
FS_RESULT FPDF_Annot_GetLinkCount(FPDF_PAGE page, FS_INT32* link_count);

/**
 * @brief	Get the specified hyperlink handler.
 *
 * @param[in] page			A page handle
 * @param[in] link_index	A zero-based index used to specify the link.
 * @param[out] link_annot	Pointer to a annot handle that receives the handle of the first action.
 *
 * @return	::FS_ERR_SUCCESS means get the handler successfully.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function only supports link annotation.
 */
FS_RESULT FPDF_Annot_GetLink(FPDF_PAGE page, FS_INT32 link_index, FPDF_ANNOT* link_annot);

/**
 * @brief	Get action(s) associated with the specified hyperlink.
 *
 * @param[in] page			A page handle
 * @param[in] link_annot	The handler of the specified hyperlink.
 * @param[out] action		Pointer to a <b>FPDF_ACTION</b> structure that receives the handle of the first action.
 *
 * @return	::FS_ERR_SUCCESS means get the action(s) successfully.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function only supports link annotation.
 */
FS_RESULT FPDF_Annot_GetLinkAction(FPDF_PAGE page, FPDF_ANNOT link_annot, FPDF_ACTION* action);

/**
 * @brief	Get the count of areas(quadrilaterals) for the specified link.
 *
 * @param[in] page			A page handle.
 * @param[in] link_annot	A zero-based index used to specify the link.
 * @param[out] count		Pointer to an integer that receives the count of quadrilaterals.
 *
 * @return	::FS_ERR_SUCCESS means get the count successfully. <br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function only supports link annotation.
 */
FS_RESULT FPDF_Annot_GetLinkAreaCount(FPDF_PAGE page, FPDF_ANNOT link_annot, FS_INT32* count);

/**
 * @brief	Get a particular quadrilateral for a link.
 *
 * @details	The result in "points" array are the X/Y coordinations for the four vertices
 *			of the quadrilateral. Vertices are in the following order: lower left, lower
 *			right, upper right, upper left.
 *
 * @param[in] page			A page handle.
 * @param[in] link_annot	A Zero-based index used to specify for the link.
 * @param[in] area_index	A Zero-based index used to specify the quadrilateral.
 * @param[out] points		Pointer to an array, which consists 4 points, that receives coordinations.
 *
 * @return	::FS_ERR_SUCCESS means get the particular quadrilateral successfully.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function only supports link annotation.
 */
FS_RESULT FPDF_Annot_GetLinkArea(FPDF_PAGE page, FPDF_ANNOT link_annot, FS_INT32 area_index, FS_POINTF* points);

/**
 * @brief	Get the count of annotations in the page.
 *
 * @param[in] page			A page handle.
 * @param[out] count		Pointer to an integer that receives the count of annotations.
 *
 * @return	::FS_ERR_SUCCESS means get the count successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function supports the following annotations:<br>
 *			fileattachment, stamp, pencil, highlight, note.
 */
FS_RESULT FPDF_Annot_GetCount(FPDF_PAGE page, FS_INT32* count);

/**
 * @brief	Get the handler of the annotation by the index.
 *
 * @param[in] page			A page handle.
 * @param[in] annot_index	The zero-based annotation index.
 * @param[out] annot		The annotation handler.
 *
 * @return	::FS_ERR_SUCCESS means get the index successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Annot_Get(FPDF_PAGE page, FS_INT32 annot_index, FPDF_ANNOT* annot);

/**
 * @brief	Get the handler of the annotation which is the nearest one to a certain position on the page.
 *
 * @details	When the point is inside an annotation, this annotation is the nearest one to the point.
 *			Otherwise this annotation cannot be the nearest one to the point.<br>
 *			If there is no nearest annotation, the parameter <i>annot</i> will be -1.
 *
 * @param[in] page			A page handle
 * @param[in] x				The value of X position in the PDF page coordination system.
 * @param[in] y				The value of Y position in the PDF page coordination system.
 * @param[out] annot		Pointer to the annotation handler.
 *
 * @return	::FS_ERR_SUCCESS means get the index successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Annot_GetAtPos(FPDF_PAGE page, FS_FLOAT x, FS_FLOAT y, FPDF_ANNOT* annot);

/**
 * @brief	Get the index of the annotation by the annotation handler.
 *
 * @param[in] page			A page handle.
 * @param[in] annot			The annotation handler.
 * @param[out] index		Pointer to an integer that receives the zero-based annotation index.
 *
 * @return	::FS_ERR_SUCCESS means get the index successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Annot_GetIndex(FPDF_PAGE page, FPDF_ANNOT annot, FS_INT32* index);

/**
 * @brief	Get the information of the specified annotation.
 *
 * @details	The application must call ::FPDF_Annot_GetCount first before it can call this function
 *			for any specified annotation.<br>
 *			If the parameter <i>buffer</i> is <b>NULL</b>, then the parameter <i>bufsize</i> will receive
 *			the count of bytes required to store the information of the specified annotation.<br>
 *			If the paramter <i>bufsize</i>(the size of <i>buffer</i>) is smaller than the required size, then this function
 *			will not copy any data, and return the error code ::FS_ERR_PARAM. And the required buffer size will 
 *			also be put into <i>bufsize</i>.<br>
 *			The data type of <i>buffer</i> depends on the parameter <i>infotype</i>,
 *			and different types are used:
 *			<ul>
 *			<li>::FPDF_ANNOTINFO_TYPE:			::FS_INT32</li>
 *			<li>::FPDF_ANNOTINFO_AUTHOR:		::FS_WCHAR</li>
 *			<li>::FPDF_ANNOTINFO_RECT:			::FS_RECT</li>
 *			<li>::FPDF_ANNOTINFO_CONTENTS:		::FS_WCHAR</li>
 *			<li>::FPDF_ANNOTINFO_COLOR:			::FS_DWORD</li>
 *			<li>::FPDF_ANNOTINFO_OPACITY:		::FS_INT32</li>
 *			<li>::FPDF_ANNOTINFO_LINEWIDTH:		::FS_INT32</li>
 *			</ul>
 *			When the data type is ::FS_WCHAR, the output buffer will be in Unicode, using UTF-16LE format. 
 *			It's terminated by two consecutive zero bytes.
 *
 * @param[in] page			A page handle
 * @param[in] annot			The annotation handler.
 * @param[in] infotype		The type of the information. See the macro definitions <b>FPDF_ANNOTINFO_XXX</b>.
 * @param[out] buffer		A buffer allocated by the application, or <b>NULL</b>. 
 *							If it has enough or larger size, it will be used to receive the information of the annotation.<br>
 *							Otherwise <i>buffer</i> will get nothing, and should be allocated enough size by using <i>bufsize</i> returned by this function.
 * @param[in,out] bufsize	Pointer to a number which indicates the size(count of bytes) of <i>buffer</i>
 *							 before this function is called. Then after the function returns, this paremeter will store
 *							the actual count of bytes of the annotation's information.
 *
 * @return	::FS_ERR_SUCCESS means get the information successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function supports the following annotation types:<br>
 *			fileattachment, stamp, pencil, highlight, note.
 */
FS_RESULT FPDF_Annot_GetInfo(FPDF_PAGE page, FPDF_ANNOT annot, FS_INT32 infotype, FS_LPVOID buffer, FS_LPDWORD bufsize);

/**
 * @brief	Set the information of the specified annotation.
 *
 * @param[in] page			A page handle
 * @param[in] annot			The annotation handler.
 * @param[in] infotype		The type of the information. See macro definitions <b>FPDF_ANNOTINFO_xxx</b>.
 * @param[in] buffer		A buffer allocated by the application and used to set the information. <br>
 *							The data type of <i>buffer</i> depends on the parameter <i>infotype</i>.<br>
 *							See the comment of ::FPDF_Annot_GetInfo for more information.
 * @param[in] bufsize		The size(count of bytes) of <i>buffer</i>.
 *
 * @return	::FS_ERR_SUCCESS means set the information successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function supports the following annotation types:<br>
 *			fileattachment, stamp, pencil, highlight, note.<br>
 *			To pencil annotations, the <i>infotype</i> can use ::FPDF_ANNOTINFO_LINEWIDTH instead of ::FPDF_ANNOTINFO_RECT.<br>
 *			To other annotations, the <i>infortype</i> cannot use ::FPDF_ANNOTINFO_LINEWIDTH.
 */
FS_RESULT FPDF_Annot_SetInfo(FPDF_PAGE page, FPDF_ANNOT annot, FS_INT32 infotype, FS_LPVOID buffer, FS_DWORD bufsize);

/**
 * @brief	Add an annotation to the page
 *
 * @details	The data type of the parameter <i>data</i> depends on the parameter <i>annottype</i>,
 *			and different types are used:<br>
 *			Format:  Annot Type - Data type 
 *			<ul>
 *			<li>::FPDF_ANNOTTYPE_NOTE			-	::FPDF_ANNOT_NOTEINFO</li>
 *			<li>::FPDF_ANNOTTYPE_HIGHLIGHT		-	::FPDF_ANNOT_HIGHLIGHTINFO</li>
 *			<li>::FPDF_ANNOTTYPE_PENCIL			-	::FPDF_ANNOT_PENCILINFO</li>
 *			<li>::FPDF_ANNOTTYPE_STAMP			-	::FPDF_ANNOT_STAMPINFO</li>
 *			<li>::FPDF_ANNOTTYPE_FILEATTACHMENT -	::FPDF_ANNOT_FILEATTACHMENTINFO</li>
 *			<li>::FPDF_ANNOTTYPE_LINK			-	::FPDF_ANNOT_LINKINFO</li>	
 *			</ul>
 *
 * @param[in] page			A page handle.
 * @param[in] annottype		The type of the annotation which is to be added to the page. See macro definitions <b>FPDF_ANNOTTYPE_XXX</b>.
 * @param[in] data			The data struct of the annotation.
 * @param[in] datasize		The data size (count of bytes). This value should be above 0.
 * @param[out] annot		The annotation handler.
 *
 * @return	::FS_ERR_SUCCESS means add successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	This function supports the following annotation types:<br>
 *			fileattachment, stamp, pencil, highlight, note, link.<br>
 *			ATTENTION: If the type of the annotations to be added is link, this function doesn't support type ::FPDF_DEST_DOC now.
 */
FS_RESULT FPDF_Annot_Add(FPDF_PAGE page, FS_INT32 annottype, FS_LPVOID data, FS_DWORD datasize, FPDF_ANNOT* annot);

/**	
 * @brief	Delete the specified annotation of the page
 *
 * @param[in] page			A page handle.
 * @param[in] annot			The annotation handler.
 *
 * @return	::FS_ERR_SUCCESS means delete successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Annot_Delete(FPDF_PAGE page, FPDF_ANNOT annot);

#ifdef __cplusplus
};
#endif

#endif
// _FPDF_ANNOT_H_
/**@}*/

