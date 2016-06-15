/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPSI" module to be enabled in your SDK license.
  * @file	fpsi.h
  * @author	Foxit Corporation
  * @brief	Header file for the Pressure Sensitive Ink module.<br>
  *			The module is based on the pressure sensitivity technology. <br>
  *			Users can use the methods in this module to:<br>
  *			1. Connect to an external tablet and set the ARGB value and the diameter size of the brush.<br>
  *			2. Initial a transparent canvas in the client zone to enable users to write or draw on it.<br>
  *			3. Display the strokes with different states and pressure according to the user's drawing speed and pressure.<br>
  *			4. Generate the PSI information to a PDF annotation and add the annotation to the page.
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Pressure Sensitive Ink module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPSIBASIC Pressure Sensitive Ink
 * @brief Methods and Definitions in this module are included in fpsi.h
 */
/**@{*/

#ifndef _FPSI_H_
#define _FPSI_H_

#ifndef _FS_BASE_H_
#include "fs_base.h"
#endif

#ifndef _FPDF_BASE_H_
#include "fpdf_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//					Structures and Macro Definitions					//
//////////////////////////////////////////////////////////////////////////

#ifndef _FS_DEF_APPCALLBACK_
#define _FS_DEF_APPCALLBACK_
/** @brief Structure for the PSI application callback. */
typedef struct _FPSI_APPCALLBACK_
{
	
	/** @brief The size of the structure. It must be set to <b>sizeof</b>(::FPSI_APPCALLBACK). */
	FS_DWORD			lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID			clientData;

	/**
	 * @brief Invalidate the client area within the specified rectangle.
	 * 
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] left			The left position of the client area in the PSI coordinate.
	 * @param[in] top			The top  position of the client area in the PSI coordinate.
	 * @param[in] right			The right position of the client area in the PSI coordinate.
	 * @param[in] bottom		The bottom position of the client area in the PSI coordinate.
	 *
	 * @return None.
	 */
	void		(*Invalidate)(FS_LPVOID clientData, FS_INT32 left, FS_INT32 top, FS_INT32 right, FS_INT32 bottom);

	/**
	 * @brief Get the opacity of the paint foreground, in range [0, 1].
	 * 
	 * @param[in] clientData	The user-supplied data.
	 *
	 * @return The opacity of the paint foreground.
	 */
	FS_FLOAT	(*GetOpacity)(FS_LPVOID clientData);

} FPSI_APPCALLBACK;
#endif

/**
 * @name Macro Definitions for Point Flags 
 * @note These definitions will be used in ::FPSI_AddPoint.
 */
/**@{*/
/** @brief Point adding flags of LINETO */
#define FPSI_PT_LINETO			0x02
/** @brief Point adding flags of MOVETO */
#define FPSI_PT_MOVETO			0x06
/** @brief Point adding flags of ENDPATH */
#define FPSI_PT_ENDPATH			0x08
/**@}*/

/**
 * @brief	Init the pressure sensitive ink environment. 
 *
 * @param[in] pCallBack		A Pointer to a ::FPSI_APPCALLBACK structure.
 * @param[in] bSimulate		Turn on/off the simulation of Pressure Sensitive Ink.
 * @param[out] psi			Pointer to a <b>FPSI_HANDLE</b> structure that receives the PSI handle.
 *
 * @return	::FS_ERR_SUCCESS means initialize the environment successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT	FPSI_InitEnvironment(FPSI_APPCALLBACK* pCallBack, FS_BOOL bSimulate, FPSI_HANDLE* psi);

/**
 * @brief	Destroy the pressure sensitive ink environment.
 *
 * @param[in] hHandle		A PSI handle returned by ::FPSI_InitEnvironment.
 *
 * @return None.
 */
void		FPSI_DestroyEnvironment(FPSI_HANDLE hHandle);

/**
 * @brief	Init a PSI canvas.
 *
 * @param[in] hHandle		A PSI handle.
 * @param[in] width			The width of the PSI panel. This must be greater than 0.	
 * @param[in] height		The height of the PSI panel. This must be greater than 0.
 *
 * @return	::FS_ERR_SUCCESS means init the canvas successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT	FPSI_InitCanvas(FPSI_HANDLE hHandle, FS_INT32 width, FS_INT32 height);

/**
 * @brief	Set the ink color.
 *
 * @param[in] hHandle		A PSI handle.
 * @param[in] color			The ink color. It's constructed by 0xrrggbb.
 *
 * @return	::FS_ERR_SUCCESS means set the ink color successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT	FPSI_SetInkColor(FPSI_HANDLE hHandle, FS_ARGB color);

/**
 * @brief	Set the ink diameter.
 *
 * @param[in] hHandle		A PSI handle.
 * @param[in] diameter		The ink diameter.
 *
 * @return	::FS_ERR_SUCCESS means set the ink diameter successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT	FPSI_SetInkDiameter(FPSI_HANDLE hHandle, FS_INT32 diameter);

/**
 * @brief	Add a pressure sensitive point to the environment.
 *
 * @param[in] hHandle		A PSI handle.
 * @param[in] x				The value of x-coordinate in the panel coordinate.
 * @param[in] y				The value of y-coordinate in the panel coordinate.
 * @param[in] pressure		The pressure of the current point between 0 to 1. 
 * @param[in] flag			The point flags. See macro definitions <b>FPSI_PT_XXX</b>
 *
 * @return	::FS_ERR_SUCCESS means add a point successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT	FPSI_AddPoint(FPSI_HANDLE hHandle, FS_FLOAT x, FS_FLOAT y, FS_FLOAT pressure, FS_INT32 flag);

/**
 * @brief	Render the pressure ink path to a device independent bitmap. 	
 *
 * @param[in] hHandle		A PSI handle.
 * @param[out] bitmap		A handle to the device independent bitmap (as the output buffer).
 * @param[in] xDes			The left pixel position of the area to render in the coordinate of destination.
 * @param[in] yDes			The top pixel position of the area to render in the coordinate of destination.
 * @param[in] nWidth		The width to render in the coordinate of destination.
 * @param[in] nHeight		The height to render in the coordinate of destination.
 * @param[in] xSrc			The left pixel position of the display area in the coordinate of canvas.	
 * @param[in] ySrc			The top pixel position of the display area in the coordinate of canvas.
 *
 * @return	::FS_ERR_SUCCESS means render the canvas to bitmap successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT	FPSI_Render(FPSI_HANDLE hHandle, FS_BITMAP bitmap, FS_INT32 xDes, FS_INT32 yDes, FS_INT32 nWidth, FS_INT32 nHeight, FS_INT32 xSrc, FS_INT32 ySrc);

/**
 * @brief	Generate the PSI information to a PDF page.
 *
 * @param[in] hHandle		A PSI handle.
 * @param[in] page			A page handle.
 * @param[in] pageRect		The Page rect in the device coordinate.
 * @param[in] left			The left pixel position of the display area in the page coordinate.
 * @param[in] top			The top pixel position of the display area in the page coordinate.
 * @param[in] right			The right pixel position of the display area in the page coordinate.
 * @param[in] bottom		The bottom pixel position of the display area in the page coordinate.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT	FPSI_GeneratePSIAnnot(FPSI_HANDLE hHandle, FPDF_PAGE page, FS_RECT pageRect, FS_FLOAT left, 
								  FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom);

#ifdef __cplusplus
};
#endif

#endif
// _FPSI_H_
/**@}*/

