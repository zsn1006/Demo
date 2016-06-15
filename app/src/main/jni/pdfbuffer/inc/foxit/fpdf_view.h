/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPDFVIEW" module to be enabled in your SDK license.
  * @file	fpdf_view.h
  * @author	Foxit Corporation
  * @brief	Header file for the View module.<br>
  *			The view module offers:<br>
  *			1. Methods for basic rendering of pages.<br>
  *			2. Methods for retrieving datas from a page and other related operations.<br>
  *			3. Methods for operating fonts.<br>
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling View module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPDFVIEW View 
 * @brief Methods in this module are included in fpdf_view.h .
 */
/**@{*/

#ifndef _FPDF_VIEW_H_
#define _FPDF_VIEW_H_

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
 * @brief Get the size of a page
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[out] width		Used to receive the width of the page.
 * @param[out] height		Used to receive the height of the page.
 *
 * @return	::FS_ERR_SUCCESS means get the size successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_GetSize(FPDF_PAGE page, FS_FLOAT* width, FS_FLOAT* height);

/**
 * @brief Get the displayable area (bounding box) of a page.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[out] rect			A ::FS_RECTF structure used to receive the rectangle of the bounding box.
 *
 * @return	::FS_ERR_SUCCESS means get the displayable area successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_GetBBox(FPDF_PAGE page, FS_RECTF* rect);

/**
 * @brief Get the content margin.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[out] left			Used to receive the left margin.
 * @param[out] top			Used to receive the top margin.
 * @param[out] right		Used to receive the right margin.
 * @param[out] bottom		Used to receive the bottom margin.
 * @param[in] backcolor		The color for intended background in the page, in the format 0xRRGGBB.
 *
 * @return	::FS_ERR_SUCCESS means get the content margin successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Page_GetContentMargin(FPDF_PAGE page, FS_FLOAT* left, FS_FLOAT* top, FS_FLOAT* right, FS_FLOAT* bottom, 
									 FS_ARGB backcolor);


/**
 * @brief Set the pixel count limit for using halftone when displaying image.
 *
 * @details By default, EMB SDK displays all bitmaps using downsamping, which means
 *			if the image is shrinked onto the screen, only part of pixels will be picked
 *			and displayed. This saves a lot of calculation, especially for big images
 *			with millions of pixels. However the display quality can be bad. <br>In order to
 *			reach a balance between performance and quality, the application can use this
 *			function to set a limit: if the count of pixels in an image is more than this
 *			limit, then EMB SDK will use downsampling for quick drawing; otherwise, if
 *			the image has less pixels, EMB SDK will use halftoning for better quality.
 *
 * @param[in] limit			The count of pixels for the limit.
 *
 * @return None.
 */
void FPDF_RenderPage_SetHalftoneLimit(FS_INT32 limit);

/**
 * @brief Convert the device coordinations of a point(::FS_INT32) to the page coordinations.
 *
 * @details The page coordination system has its origin at the left-bottom corner of the page, 
 *			with X axis goes along the bottom side to the right, and Y axis goes along the 
 *			left side upward. No matter how you zoom, scroll, or rotate a page, a particular
 *			element (like a text or an image) on the page should always have the same coordination 
 *			values in the page coordination system. <br>
 *			The device coordination system is device dependent. For a bitmap device, its origin 
 *			is at the left-top corner of the window. You must make sure the start_x, start_y, size_x, 
 *			size_y and rotate parameters have exactly the same values as you used in calling
 *			::FPDF_RenderPage_Start function.<br>
 *			For the rectangle conversion, the result rectangle is always "normalized", meaning for
 *			page coordinations: left is always smaller than right, and bottom is smaller than top.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in,out] point		A ::FS_POINT structure with the device coordinations upon the call,
 *							and also used to receive the result page coordinations.
 *
 * @return None.
 */
void FPDF_Page_DeviceToPagePoint(FPDF_PAGE page, 
								 FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate,
								 FS_POINT* point);

/**
 * @brief Convert the device coordinations of a point(::FS_FLOAT) to the  page coordinations.
 *
 * @details The page coordination system has its origin at the left-bottom corner of the page, 
 *			with X axis goes along the bottom side to the right, and Y axis goes along the 
 *			left side upward. No matter how you zoom, scroll, or rotate a page, a particular
 *			element (like a text or an image) on the page should always have the same coordination 
 *			values in the page coordination system. <br>
 *			The device coordination system is device dependent. For a bitmap device, its origin 
 *			is at the left-top corner of the window. You must make sure the start_x, start_y, size_x, 
 *			size_y and rotate parameters have exactly the same values as you used in calling
 *			::FPDF_RenderPage_Start function.<br>
 *			For the rectangle conversion, the result rectangle is always "normalized", meaning for
 *			page coordinations: left is always smaller than right, and bottom is smaller than top.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in,out] point		A ::FS_POINTF structure with the device coordinations upon the call,
 *							and also used to receive the result page coordinations.
 *
 * @return None.
 */
void FPDF_Page_DeviceToPagePointF(FPDF_PAGE page, 
								  FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, 
								  FS_POINTF* point);

/**
 * @brief Convert the device coordinations of a rectangle(::FS_INT32) to the page coordinations.
 *
 * @details The page coordination system has its origin at the left-bottom corner of the page, 
 *			with X axis goes along the bottom side to the right, and Y axis goes along the 
 *			left side upward. No matter how you zoom, scroll, or rotate a page, a particular
 *			element (like a text or an image) on the page should always have the same coordination 
 *			values in the page coordination system. <br>
 *			The device coordination system is device dependent. For a bitmap device, its origin 
 *			is at the left-top corner of the window. You must make sure the start_x, start_y, size_x, 
 *			size_y and rotate parameters have exactly the same values as you used in calling
 *			::FPDF_RenderPage_Start function.<br>
 *			For the rectangle conversion, the result rectangle is always "normalized", meaning for
 *			page coordinations: left is always smaller than right, and bottom is smaller than top.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in,out] rect		A ::FS_RECT structure with the device coordinations upon the call,
 *							and also used to receive the result page coordinations.
 *
 * @return None.
 */
void FPDF_Page_DeviceToPageRect(FPDF_PAGE page, 
								FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, 
								FS_RECT* rect);

/**
 * @brief Convert the device coordinations of a rectangle(::FS_FLOAT) to the page coordinations.
 *
 * @details The page coordination system has its origin at the left-bottom corner of the page, 
 *			with X axis goes along the bottom side to the right, and Y axis goes along the 
 *			left side upward. No matter how you zoom, scroll, or rotate a page, a particular
 *			element (like a text or an image) on the page should always have the same coordination 
 *			values in the page coordination system. <br>
 *			The device coordination system is device dependent. For a bitmap device, its origin 
 *			is at the left-top corner of the window. You must make sure the start_x, start_y, size_x, 
 *			size_y and rotate parameters have exactly the same values as you used in calling
 *			::FPDF_RenderPage_Start function.<br>
 *			For the rectangle conversion, the result rectangle is always "normalized", meaning for
 *			page coordinations: left is always smaller than right, and bottom is smaller than top.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in,out] rect		A ::FS_RECTF structure with the device coordinations upon the call,
 *							and also used to receive the result page coordinations.
 *
 * @return None.
 */
void FPDF_Page_DeviceToPageRectF(FPDF_PAGE page, 
								 FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, 
								 FS_RECTF* rect);

/**
 * @brief Convert the page coordinations of a point(::FS_INT32) to device coordinations.
 *
 * @details For the rectangle conversion, the result rectangle is always "normalized", meaning for
 *			device coordinations: left is always smaller than right, and top is smaller than bottom.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in,out] point		A point structure(::FS_POINT) with the page coordinations upon the call,
 *							and also used to receive the result device coordinations.
 *
 * @return None.
 *
 * @see ::FPDF_Page_DeviceToPagePoint
 */
void FPDF_Page_PageToDevicePoint(FPDF_PAGE page, 
								 FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, 
								 FS_POINT* point);

/**
 * @brief Convert the page coordinations of a point(::FS_FLOAT) to the device coordinations.
 *
 * @details For the rectangle conversion, the result rectangle is always "normalized", meaning for
 *			device coordinations: left is always smaller than right, and top is smaller than bottom.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in,out] point		A ::FS_POINTF structure with page coordinations upon the call,
 *							and also used to receive the result device coordinations.
 *
 * @return None.
 *
 * @see ::FPDF_Page_DeviceToPagePointF
 */
void FPDF_Page_PageToDevicePointF(FPDF_PAGE page, 
								  FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, 
								  FS_POINTF* point);

/**
 * @brief Convert the page coordinations of a rectangle(::FS_INT32) to the device coordinations.
 *
 * @details For the rectangle conversion, the result rectangle is always "normalized", meaning for
 *			device coordinations: left is always smaller than right, and top is smaller than bottom.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in,out] rect		A ::FS_RECT structure with page coordinations upon the call,
 *							and also used to receive the result device coordinations.
 *
 * @return None.
 *
 * @see ::FPDF_Page_DeviceToPageRect
 */
void FPDF_Page_PageToDeviceRect(FPDF_PAGE page, 
								FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, 
								FS_RECT* rect);

/**
 * @brief Convert the page coordinations of a rectangle(::FS_FLOAT) to the device coordinations.
 *
 * @details For the rectangle conversion, the result rectangle is always "normalized", meaning for
 *			device coordinations: left is always smaller than right, and top is smaller than bottom.
 *
 * @param[in] page			A page handle returned by ::FPDF_Page_Load.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in,out] rect		A rectangle structure(::FS_RECTF) with page coordinations upon the call,
 *							and also used to receive the result device coordinations.
 *
 * @return None.
 *
 * @see ::FPDF_Page_DeviceToPageRectF
 */
void FPDF_Page_PageToDeviceRectF(FPDF_PAGE page, 
								 FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, 
								 FS_RECTF* rect);

/**
 * @brief Start drawing a quick preview of a page.
 *
 * @details It's often useful to present users a quick preview of a page, right after the
 *			page is parsed. This preview renders only a limited set of easy features in the
 *			page, so it'll be rather quick to finish this process.
 *
 * @param[in] dib			A DIB handle, as the rendering device.
 * @param[in] page			A page handle. The page has to be parsed first.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in] flags			This is reserved and it must be zero.
 * @param[in] pause			A ::FS_PAUSE structure that can pause the rendering process.
 *							This can be <b>NULL</b> if no pausing is needed.
 *
 * @return	::FS_ERR_SUCCESS means quickdraw successfully finished.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_RenderPage_StartQuickDraw(FS_BITMAP dib, FPDF_PAGE page,
										 FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate,
										 FS_INT32 flags, FS_PAUSE* pause);
/**
 * @brief Continue a quick draw processing
 *
 * @param[in] page			A page handle. The page has to be parsed first.
 * @param[in] pause			A ::FS_PAUSE structure that can pause the rendering process.
 *							This can be <b>NULL</b> if no pausing is needed.
 *
 * @return	::FS_ERR_SUCCESS means quickdraw successfully finished.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_RenderPage_ContinueQuickDraw(FPDF_PAGE page, FS_PAUSE* pause);


/**
 * @brief Start rendering of a page.
 *
 * @details Rendering is a progressive process. This function starts the rendering process,
 *			and may return before rendering is finished, if a pause structure is provided.<br>
 *			The application should call ::FPDF_RenderPage_Continue repeatedly to finish the rendering 
 *			when return value is ::FS_ERR_TOBECONTINUED.<br>
 *			There can be only one rendering procedure for a page at any time. And rendering
 *			can be started over and over again for the same page. If a page rendering is already
 *			active, starting another one will cancel the previous rendering.<br>
 *			Rendering of a page doesn't draw the page background, therefore, you usually need
 *			to draw the background in the DIB yourself.<br>
 *			You don't have to parse the page before you can render it. The engine will parse
 *			the page along with the rendering process. With this technique, along with
 *			::FPDF_RENDER_DROP_OBJECTS flag, you can really render very complicated pages without
 *			much memory consumption (because no page objects will be cached).
 *
 * @param[out] dib			A DIB handle, as the rendering device.
 * @param[in] page			A page handle. The page has to be parsed first.
 * @param[in] start_x		The left pixel position of the display area in the device coordination
 * @param[in] start_y		The top pixel position of the display area in the device coordination
 * @param[in] size_x		The horizontal size (in pixels) for displaying the page
 * @param[in] size_y		The vertical size (in pixels) for displaying the page
 * @param[in] rotate		The page orientation: <br>
 *							<ul>
 *							<li>0: normal</li>
 *							<li>1: rotated 90 degrees clockwise</li>
 *							<li>2: rotated 180 degrees</li>
 *							<li>3: rotated 90 degrees counter-clockwise</li>
 *							</ul>
 * @param[in] flags			0 means normal display. This can also be the combination of macro definitions:<br>
 *							<ul>
 *							<li>::FPDF_RENDER_ANNOT</li>
 *							<li>::FPDF_RENDER_LCD_TEXT</li>
 *							<li>::FPDF_RENDER_BGR_STRIPE</li>
 *							<li>::FPDF_RENDER_DROP_OBJECTS</li>
 *							<li>::FPDF_RENDER_NOWIDGET</li>
 *							</ul>
 * @param[in] clip			A ::FS_RECT structure of a clip rectangle (in DIB device coordinations),
 *							or NULL if no clipping is needed.
 * @param[in] pause			A ::FS_PAUSE structure that can pause the rendering process.
 *							This can be <b>NULL</b> if no pausing is needed.
 *
 * @return	::FS_ERR_SUCCESS means rendering successfully finished.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note	The render flag ::FPDF_RENDER_NOWIDGET should be set if method ::FPDF_FormFill_Draw is called after,
 *			because ::FPDF_FormFill_Draw takes care of "rendering widgets" in the FormFill environment.
 *
 */
FS_RESULT FPDF_RenderPage_Start(FS_BITMAP dib, FPDF_PAGE page,
								FS_INT32 start_x, FS_INT32 start_y, FS_INT32 size_x, FS_INT32 size_y, FS_INT32 rotate, FS_INT32 flags,
								FS_RECT* clip, FS_PAUSE* pause);

/**
 * @brief Set fore and background color.
 *
 * @param[out] dib			A DIB handle, as the rendering device.
 * @param[in] page			A page handle. The page has to be parsed first.
 * @param[in] backColor     The background Color, in ABGR order.
 * @param[in] foreColor		The foreground Color, in ABGR order.
 *
 * @return	::FS_ERR_SUCCESS means set optons successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_RenderPage_SetOptions(FS_BITMAP dib, FPDF_PAGE page,
									 FS_ARGB backColor, FS_ARGB foreColor);

/**
 * @brief Continue the page rendering
 *
 * @details This function may return any time when the pause interface indicates 
 *			a pause is needed. The application can call ::FPDF_RenderPage_Continue any number
 *			of times, until ::FS_ERR_TOBECONTINUED is not returned.
 *
 * @param[in] page			A page handle
 * @param[in] pause			A ::FS_PAUSE structure that can pause the rendering process.
 *							This can be <b>NULL</b> if no pausing is needed.
 *
 * @return	::FS_ERR_SUCCESS means rendering successfully finished.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_RenderPage_Continue(FPDF_PAGE page, FS_PAUSE* pause);

/**
 * @brief Get an estimated rendering progress in percentage
 *
 * @param[in] page					A page handle.
 *
 * @return	An integer between 0 and 100 (inclusive) indicating the rendering progress.
 *			The result is just a rough estimation.<br>
 *			If the rendering just finished, this function will return 0.<br>
 *			-1 means parameter error.
 */
FS_INT32 FPDF_RenderPage_GetProgress(FPDF_PAGE page);

/**
 * @brief Cancel the page rendering.
 *
 * @param[in] page					A page handle
 *
 * @return	::FS_ERR_SUCCESS means cancel rendering successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_RenderPage_Cancel(FPDF_PAGE page);

#ifdef __cplusplus
};
#endif

#endif
// _FPDF_VIEW_H_
/**@}*/

