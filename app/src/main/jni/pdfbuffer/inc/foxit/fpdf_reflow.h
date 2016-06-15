/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPDFREFLOW" module to be enabled in your SDK license.
  * @file	fpdf_reflow.h
  * @author	Foxit Corporation
  * @brief	Header file for the Reflow module.<br>
  *			The reflow module manily offers the following functions:<br>
  *			1. Rearrange the content of parsed PDF pages.<br>
  *			2. Reparse PDF pages, which have been already parsed, according to the user's required page size.<br>
  *			3. Render the parsed reflow-page to DIBitmap.<br>
  *			4. Get data and coordinate values pointing to the current reading position.<br>
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Reflow module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPDFREFLOW Reflow
 * @brief Methods in this module are included in fpdf_reflow.h .
 */
/**@{*/

#ifndef _FPDF_REFLOW_H_
#define _FPDF_REFLOW_H_

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
 * @brief Alloc a reflow page.
 *
 * @param[out] reflowPage			Pointer to a <b>FPDF_REFLOWPAGE</b> structure that receives the handle of the reflow page.
 *
 * @return ::FS_ERR_SUCCESS means allocate the reflow page successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Reflow_AllocPage(FPDF_REFLOWPAGE* reflowPage);

/**
 * @brief Destroy a reflow page
 *
 * @param[in] Page					A reflow page handle.
 *
 * @return None.
 */
void FPDF_Reflow_DestroyPage(FPDF_REFLOWPAGE Page);

/**
 * @brief Get the width and height of a reflow page.
 *
 * @param[in] page					A reflow page handle.
 * @param[out] width				Pointer to a float that receives the width of the page.
 * @param[out] height				Pointer to a float that receives the height of the page.
 * 
 * @return	::FS_ERR_SUCCESS means get the size of the reflow page successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 * 
 */
FS_RESULT FPDF_Reflow_GetPageSize(FPDF_REFLOWPAGE page, FS_FLOAT* width, FS_FLOAT* height);
	
/**
 * @brief Start parsing a loaded PDF page into a reflow page
 *
 * @details Page Reflowing is a progressive process. It might take a long time to reflow
 *			a page. If the parameter <i>pause</i> parameter is provided, this function may return
 *			::FS_ERR_TOBECONTINUED any time during reflowing.<br>
 *			When ::FS_ERR_TOBECONTINUED is returned, the reflowing is not finished. The
 *			application must call ::FPDF_Reflow_ContinueParse to continue reflowing.
 *
 * @param[in] page					A reflow page handle returned by ::FPDF_Reflow_AllocPage function.
 * @param[in] pdfPage				A PDF page handle.
 * @param[in] width					The desired page width. This value should be above 20.
 * @param[in] fitPageHeight			The desired page height.
 * @param[in] pause					Pointer to a structure that can pause the rendering process.
 *									Can be NULL if no pausing is needed.
 *									If provided, this pointer has to be valid during the whole reflowing.
 * @param[in] flags					The parser flag. See macro definitions <b>FPDF_PARSER_xxx</b>.
 *
 * @return	::FS_ERR_SUCCESS means parsing successfully finishes.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 * 
 */
FS_RESULT FPDF_Reflow_StartParse(FPDF_REFLOWPAGE page, FPDF_PAGE pdfPage, 
 								 FS_FLOAT width, FS_FLOAT fitPageHeight, FS_PAUSE* pause, FS_INT32 flags);

/**
 * @brief Continue parsing a reflow page
 *
 * @param[in] page					A reflow page handle used in ::FPDF_Reflow_StartParse function.
 *
 * @return	::FS_ERR_SUCCESS means parsing successfully finishes.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Reflow_ContinueParse(FPDF_REFLOWPAGE page);

/**
 * @brief Set dither bits for rendering.
 *
 * @param[in] page					A reflow page handle. The page has to be parsed first.
 * @param[in] DitherBits			Default value: 2.
 *
 * @return	::FS_ERR_SUCCESS means set dither bits successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Reflow_SetDitherBits(FPDF_REFLOWPAGE page, FS_INT32 DitherBits);

/**
 * @brief Start rendering of a reflow page.
 *
 * @details Rendering is a progressive process. This function starts the rendering process,
 *			and may return before the rendering is finished, if a ::FS_PAUSE structure is provided.<br>
 *			Application should call ::FPDF_Reflow_ContinueRender repeatedly to finish the rendering 
 *			when the return value is ::FS_ERR_TOBECONTINUED.<br>
 *			There can be only one rendering procedure for a page at any time. And the rendering
 *			can be started over and over again for the same page. If a page rendering is already
 *			active, starting another one will cancel the previous rendering.<br>
 *			Rendering of a page doesn't draw the page background, therefore, you usually need
 *			to draw the background in the DIB yourself.
 *
 * @param[in] dib					A DIB handle, as the rendering device.
 * @param[in] page					A reflow page handle. The page has to be parsed first.
 * @param[in] start_x				The left pixel position of the display area in the device coordination
 * @param[in] start_y				The top pixel position of the display area in the device coordination
 * @param[in] size_x				The horizontal size (in pixels) for displaying the page
 * @param[in] size_y				The vertical size (in pixels) for displaying the page
 * @param[in] rotate				The page orientation: 
 *									<ul>
 *									<li>0:	Normal</li>
 *									<li>1:	Rotate 90 degrees clockwise</li>
 *									<li>2:	Rotate 180 degrees</li>
 *									<li>3:	Rotate 90 degrees counter-clockwise</li>
 *									</ul>
 * @param[in] pause					A pointer to a ::FS_PAUSE structure that can pause the rendering process.
 * 									Thsi can be <b>NULL</b> if no pausing is needed.
 *									If provided, this pointer has to be valid during the whole rendering.
 *
 * @return	::FS_ERR_SUCCESS means rendering successfully finishes.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Reflow_StartRender(FS_BITMAP dib, FPDF_REFLOWPAGE page, FS_INT32 start_x, FS_INT32 start_y, 
								  FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, FS_PAUSE* pause);

/**
 * @brief Continue rendering a reflow page.
 *
 * @param[in] page					A reflow page handle used in ::FPDF_Reflow_StartRender function
 *
 * @return	::FS_ERR_SUCCESS means rendering successfully finishes.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Reflow_ContinueRender(FPDF_REFLOWPAGE page);


/**
 * @brief Get the reflow page matrix.
 *
 * @param[in] page					A reflow page handle. The page has to be parsed first.
 * @param[in] start_x				The left pixel position of the display area in the device coordination
 * @param[in] start_y				The top pixel position of the display area in the device coordination
 * @param[in] size_x				The horizontal size (in pixels) for displaying the page
 * @param[in] size_y				The vertical size (in pixels) for displaying the page
 * @param[in] rotate				The page orientation: 
 *									<ul>
 *									<li>0:	Normal</li>
 *									<li>1:	Rotate 90 degrees clockwise</li>
 *									<li>2:	Rotate 180 degrees</li>
 *									<li>3:	Rotate 90 degrees counter-clockwise</li>
 *									</ul>
 * @param[out] matrix				Pointer to a ::FS_MATRIX handle that receives the output matrix.
 *
 * @return	::FS_ERR_SUCCESS means get the data successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
*/
FS_RESULT FPDF_Reflow_GetMatrix(FPDF_REFLOWPAGE page, FS_INT32 start_x, FS_INT32 start_y, 
								FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, FS_MATRIX* matrix);


/**
 * @brief Get the data pointing to the current reading position
 *
 * @details This function retrieves the data pointing to the given reading position.
 *			Application can convert it to the physical position by calling ::FPDF_Reflow_GetFocusPosition.
 *
 * @param[in] page					A reflow page
 * @param[in] matrix				A matrix handle returned by ::FPDF_Reflow_GetMatrix
 * @param[in] x						The value of the x-coordinate of the destination position
 * @param[in] y						The value of the y-coordinate of the destination position
 * @param[out] buffer				Pointer to a buffer(allocated by the application) that receives the destination data
 *									If this parameter is <b>NULL</b>, then the data won't be retrieved.
 * @param[in,out] bufsize			Pointer to a number used to input the size of <i>buffer</i> and then receive the data block size for the destination.
 *									If the parameter <i>buffer</i> is <b>NULL</b>, this will get the actual size of the destination data.
 *
 * @return	::FS_ERR_SUCCESS means get the data successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Reflow_GetFocusData(FPDF_REFLOWPAGE page, FS_MATRIX* matrix, FS_INT32 x, FS_INT32 y, FS_LPVOID buffer, FS_LPDWORD bufsize);

/**
 * @brief Get a point pointing to the current reading position
 *
 * @details This function Get a point from data retrieved from ::FPDF_Reflow_GetFocusData.
 *
 * @param[in] page					A reflow page handle
 * @param[in] matrix				A matrix handle returned by ::FPDF_Reflow_GetMatrix.
 * @param[in] data					The data returned by ::FPDF_Reflow_GetFocusData.
 * @param[in] size					The size of the data.	
 * @param[out] x					Pointer to an integer that receives the value of the x-coordinate of the destination position
 * @param[out] y					Pointer to an integer that receives the value of the y-coordinate of the destination position
 *
 * @return	::FS_ERR_SUCCESS means get the position successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Reflow_GetFocusPosition(FPDF_REFLOWPAGE page, FS_MATRIX* matrix, 
									   FS_LPVOID data, FS_INT32 size, FS_INT32* x, FS_INT32* y);

#ifdef __cplusplus
};
#endif

#endif
// _FPDF_REFLOW_H_
/**@}*/

