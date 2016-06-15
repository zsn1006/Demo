/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPDFTEXT" module to be enabled in your SDK license.
  * @file	fpdf_text.h
  * @author	Foxit Corporation
  * @brief	Header file for the Text module.<br>
  *			The Text module is mainly used to deal with text(including weblink) data inside a page.
  *			Before any operation, users should gain the handle of FPDF_TEXTPAGE by using function ::FPDF_Text_LoadPage 
  *			and remember to call function ::FPDF_Text_CloseTextPage to release the handle.
  *			By using this handle, users can do a lot of operations with the text and characters inside a page, 
  *			such as retrieve the data and the feature of the text, searching in a page, and so on. 
  *			Users can even retrieve data and feature of weblinks appearing in a page.
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Text module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPDFTEXT Text 
 * @brief Methods in this module are included in fpdf_text.h .
 */
/**@{*/

#ifndef _FPDFTEXT_H_
#define _FPDFTEXT_H_

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
 * @brief Prepare the information about all characters in a page.
 *
 * @param[in]  page				A page handle returned by ::FPDF_Page_Load. 
 * @param[out] textPage			Used to receive a text page handle. If an error occurs, this will be set to <b>NULL</b>.
 *
 * @return	::FS_ERR_SUCCESS means load the text page successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note The application must call ::FPDF_Text_CloseTextPage to release the text page information. 
 */
FS_RESULT FPDF_Text_LoadPage(FPDF_PAGE page, FPDF_TEXTPAGE* textPage);

/**
 * @brief Release all resources allocated for a text page handle.   
 *			
 * @param[in] textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 *
 * @return	::FS_ERR_SUCCESS means load the text page successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_CloseTextPage(FPDF_TEXTPAGE textPage);

/**
 * @brief Get the count of characters in a page. 
 *
 * @details Generated characters, such as additional space and new line characters, are also counted.
 *			Characters in a page are from a "stream". Inside the stream, each character has an index. 
 *			Index parameters are used in most of the Text module functions. The first character in 
 *			the page has an index value of zero.
 *			
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[out] count			Used to receive the count of characters in the page. If an error occurs, this will be set to -1.
 *
 * @return	::FS_ERR_SUCCESS means count characters successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_CountChars(FPDF_TEXTPAGE textPage, FS_INT32* count);


/**
 * @brief Get the unicode representation of a character in a page.   
 *			
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index of the character.
 * @param[out] unicode			Used to receive the unicode representation of the character specified by the parameter <i>index</i>.
 *								If a character is not encoded in Unicode (the Foxit engine will not 
 *								convert it to unicode), this parameter will be set to 0.
 *
 * @return	::FS_ERR_SUCCESS means get the unicode successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetUnicode(FPDF_TEXTPAGE textPage, FS_INT32 index, FS_LPDWORD unicode);

/**
 * @brief Indicate whether a character is a generated character or not.   
 *			
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index of the character.
 * @param[out] isGenChar		Used to receive a boolean value as the result of calling this function.<br>
 *								<ul>
 *								<li><b>TRUE</b>: indicate the character is a generated character.</li>
 *								<li><b>FALSE</b>: indicates it is an actual character in the PDF page.</li>
 *								</ul>
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_IsGenerated(FPDF_TEXTPAGE textPage, FS_INT32 index, FS_BOOL* isGenChar);

/**
 * @brief Get the font size of a character.   
 *			
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index of the character.
 * @param[out] fontSize			Used to receive the font size of the character specified by the parameter <i>index</i>. The size is 
 *								measured in points (about 1/72 inch). This is the typographic size of the font referred to as "em size".
 *
 * @return	::FS_ERR_SUCCESS means get the font size successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetFontSize(FPDF_TEXTPAGE textPage, FS_INT32 index, FS_FLOAT* fontSize);

/**
 * @brief Get the origin position of a particular character. 
 *		
 * @details The X and Y positions are measured in the PDF "user space".
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index of the character.
 * @param[out] origin			A pointer to an ::FS_POINT structure used to receive the X and Y position 
 *								of the character origin.
 *
 * @return	::FS_ERR_SUCCESS means get the origin position successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetOrigin(FPDF_TEXTPAGE textPage, FS_INT32 index, FS_POINT* origin);

/**
 * @brief Get the bounding box of a particular character.
 *
 * @details All positions are measured in the PDF "user space".
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index of the character.
 * @param[out] rect				A pointer to an ::FS_RECT used to receive the the 4 positions (left, right, 
 *								bottom, top)of the character box.
 *
 * @return	::FS_ERR_SUCCESS means get the origin position successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetCharBox(FPDF_TEXTPAGE textPage, FS_INT32 index, FS_RECT* rect);

/**
 * @brief Get the matrix of a particular character. 
 *
 * @details A matrix defines the transformation of coordinates from one space to another. 
 * In PDF, a matrix is defined by the following equations: <br>
 * <ul>
 * <li>x' = a * x + c * y + e; </li>
 * <li>y' = b * x + d * y + f; </li>
 * </ul>
 * ::FPDF_Text_GetMatrix is used to get a,b,c,d,e,f coefficients of the transformation from 
 * the "text space" to the "user space".
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index of the character.
 * @param[out] matrix			Used to receive matrix.
 *
 * @return	::FS_ERR_SUCCESS means get the matrix successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetMatrix(FPDF_TEXTPAGE textPage, FS_INT32 index, 
							  FS_MATRIX* matrix);

/**
 * @brief Get the index of a character at or nearby a specified position on the page.
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  x				The value of x position in the PDF "user space". 
 * @param[in]  y				The value of y position in the PDF "user space". 
 * @param[in]  xTolerance		The x-axis tolerance value for character hit detection, in point units. This should not be a negative.
 * @param[in]  yTolerance		The y-axis tolerance value for character hit detection, in point units. This should not be a negative.
 * @param[out] charIndex		The zero-based index of the character at, or nearby the point (x,y).<br> 
 *								If there is no character at or nearby the point, this parameter will 
 *								be set to -1 as a result. <br>
 *								If an error occurs, -3 will be returned. 
 *
 * @return	::FS_ERR_SUCCESS means get the index of the character successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetCharIndexAtPos(FPDF_TEXTPAGE textPage, FS_FLOAT x ,FS_FLOAT y, 
									  FS_FLOAT xTolerance, FS_FLOAT yTolerance, FS_INT32* charIndex);

/**
 * @brief Move the character at <i>index</i> in the specified <i>direction</i> and return the new 
 *        index of the character.
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index for the current character.
 * @param[in]  direction		A macro definition indicating the moving direction. This can be one of the following: <br>
 *								<ul>
 *								<li>FPDF_TEXT_LEFT</li>
 *								<li>FPDF_TEXT_UP</li>  
 *								<li>FPDF_TEXT_RIGHT</li>   
 *								<li>FPDF_TEXT_DOWN</li> 
 *								</ul>
 * @param[out] charIndex		Used to receive a zero-base character index for the new position. <br>
 *								The meaning of some special return values are:<br>
 *								<ul>
 *								<li>-1 : reach the beginning of the page</li> 
 *								<li>-2 : reach the end of the page</li>
 *								<li>-3 : failure</li>
 *								</ul>
 *
 * @return	::FS_ERR_SUCCESS means get the new index of the character successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetCharIndexByDirection(FPDF_TEXTPAGE textPage, FS_INT32 index, 
											FS_INT32 direction, FS_INT32* charIndex);

/**
 * @brief Get the font of a particular character.
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index of the character.
 * @param[out] font				Used to receive a font handle used by the character specified by the parameter <i>index</i> .
 *								This handle can be used by <b>FPDF_Font_XXX</b> functions to get more 
 *								information about the font. 
 *
 * @return	::FS_ERR_SUCCESS means get the font successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetFont(FPDF_TEXTPAGE textPage, FS_INT32 index, FS_FONT* font);

/**
 * @brief Get the font ascent (in 1/1000 em).
 *
 * @param[in]  font				A font handle returned by ::FPDF_Text_GetFont.
 * @param[out] ascent			Used to receive the accent (typically the above-baseline height of letter "h"),
 *								measured in 1/1000 of em size. <br>
 *								If a character uses a font size (em size) of 10 points, 
 *								and it has an ascent value of 500 (meaning half of the em), 
 *							   then the ascent height will be 5 points (5/72 inch). 
 *
 * @return	::FS_ERR_SUCCESS means get the font ascent successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetFontAscent(FS_FONT font, FS_INT32* ascent);

/**
 * @brief Get the font descent (in 1/1000 em).
 *
 * @param[in]  font				A font handle returned by ::FPDF_Text_GetFont.
 * @param[out] descent			Used to receive the descent (typically the under-baseline height of letter "g"), 
 *								measured in 1/1000 of em size. Most fonts have a negative descent value.   
 *
 * @return	::FS_ERR_SUCCESS means get the font descent successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetFontDescent(FS_FONT font, FS_INT32* descent);

/**
 * @brief Get the name of a font.
 *    
 * @param[in]	   font			A font handle returned by ::FPDF_Text_GetFont.
 * @param[out]     buffer		A buffer (allocated by the application) used to receive the font name text.
 * @param[in,out]  buflen		If the parameter <i>buffer</i> is <b>NULL</b>, 
 *								the count of byetes(including the null terminator) of the font name will be set to this parameter as a result.
 *								Otherwise, the number of characters will be copied.
 *
 * @return	::FS_ERR_SUCCESS means get the font name successfully or just get the actual size of the name.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetFontName(FS_FONT font, FS_LPSTR buffer, FS_LPDWORD buflen);

/** 
 * @brief Extract a Unicode text string from the page.  
 *
 * @details This function ignores characters without the Unicode information. 
 *
 * @param[in]      textPage		A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]      start		A zero-based index for the start character.
 * @param[in]      count		The count of characters to be extracted. 
 * @param[out]     buffer		A Unicode buffer (allocated by the application) used to receive the text.
 * @param[in,out]  buflen		If the parameter <i>buffer</i> is <b>NULL</b>, 
 *								the count of characters(including the Unicode string terminator) will be set to this parameter as a result.
 *								Otherwise, the number of characters will be copied.
 *
 * @return	::FS_ERR_SUCCESS means get the text successfully or just get the actual size of the text string.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note When <i>count</i> = -1 or the result of (<i>start</i> + <i>count</i>)is larger than the whole count of characters in the input text page, 
 *		 it means this function will get all charaters in the text page from the index <i>start</i> 
 *		 and set <i>buflen</i> to be the count of charaters it actually got.<br>
 */
FS_RESULT FPDF_Text_GetText(FPDF_TEXTPAGE textPage, FS_INT32 start, FS_INT32 count, 
							FS_LPWSTR buffer, FS_LPDWORD buflen);

/** 
 * @brief Count number of rectangular areas occupied by a segment of texts.  
 *
 * @details This function, along with ::FPDF_Text_GetRect, can be used to detect the position 
 *			of the text segment on a page. When highlighting text, it is the area corresponding to 
 *			the text segment that is colored in. It will automatically merge small character 
 *			boxes into bigger ones if those characters are on the same line and use the same font settings. 
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  start			A zero-based index for the start character.
 * @param[in]  count			The count of characters to be extracted.  -1 means to count all the rectangles in the page.
 * @param[out] rectCount		The count of rectangles. Zero means error. 
 *
 * @return	::FS_ERR_SUCCESS means count the rectangles successfully. <br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note The value of the parameter <i>count</i> should be greater than or equal to -1.
 */
FS_RESULT FPDF_Text_CountRects(FPDF_TEXTPAGE textPage, FS_INT32 start, FS_INT32 count, FS_INT32* rectCount);

/** 
 * @brief Get a rectangular area from the result generated by ::FPDF_Text_CountRects.   
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index the rectangle to be got.
 * @param[out] rect				A pointer to a ::FS_RECTF structure used to receive the 4 boundaries(left, top, right, bottom) of the rectangle.
 *
 * @return	::FS_ERR_SUCCESS means get the rectangle successfully.  <br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note Assume that <i>nCount</i> is the result generated by ::FPDF_Text_CountRects.<br>
 *		 The range of <i>index</i> should be 0 to (<i>nCount</i> - 1), including 0 and (<i>nCount</i> - 1).
 */
FS_RESULT FPDF_Text_GetRect(FPDF_TEXTPAGE textPage, FS_INT32 index, FS_RECTF* rect);

/** 
 * @brief Extract a Unicode text within a rectangular boundary on the page.   
 *    
 * @param[in]      textPage		A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]      rect			A ::FS_RECT structure used to specify the 4 boundaries(left, top, right, bottom) of the rectangle.
 * @param[out]     buffer		A Unicode buffer (allocated by the application) used to receive the text.
 * @param[in,out]  buflen		If the parameter <i>buffer</i> is <b>NULL</b>, 
 *								this parameter will be set to the count of characters (not bytes) as a result,
 *								including the Unicode string terminator.
 *								Otherwise, the number of characters (not bytes) will be copied.
 *
 * @return	::FS_ERR_SUCCESS means get the bounded text successfully or just get the actual size of the text string.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetBoundedText(FPDF_TEXTPAGE textPage, FS_RECT rect, 
								   FS_LPWSTR buffer, FS_LPDWORD buflen);

/** 
 * @brief Get the count of text segments within a rectangular boundary on the page. 
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  rect				A specified rectangle, in which the count of segments will be got. 
 * @param[out] count			The count of segments.
 *
 * @return	::FS_ERR_SUCCESS means count segments successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_CountBoundedSegments(FPDF_TEXTPAGE textPage, FS_RECT rect, FS_INT32* count);

/** 
 * @brief Get a particular segment. The count of segments is returned by 
 *		  ::FPDF_Text_CountBoundedSegments. 
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  index			A zero-based index of the segment to be got.
 * @param[out] start			Used to receive the start character index of the segment.
 * @param[out] count			Used to receive the count of characters in the segment. 
 *
 * @return	::FS_ERR_SUCCESS means get the segment successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note Assume that <i>nCount</i> is the result generated by ::FPDF_Text_CountBoundedSegments.<br>
 *		 The range of <i>index</i> should be 0 to (<i>nCount</i> - 1), including 0 and (<i>nCount</i> - 1).
*/
FS_RESULT FPDF_Text_GetBoundedSegment(FPDF_TEXTPAGE textPage, FS_INT32 index, FS_INT32* start, FS_INT32* count);

/**
 * @brief Search a pattern in a page quickly, without the page having been parsed.
 *
 * @details This function does a rough and quick search in a page, before the page is loaded. 
 *			The quick search will not generate an exact result saying where the pattern is
 *			found, and, it might be possible if a quick search result is "pattern found", and
 *			a real search for the same pattern, in the same page, will result in "not found".<br>
 *			However, if quick search doesn't find a pattern in a page, then we can be sure the
 *			pattern won't be found in this page when we do a real search. So, this function is 
 *			very useful when we search in a multiple-page document, and we want to quickly skip
 *			those pages in which the pattern can't possibly be found.
 *
 * @param[in] document			A document handle returned by ::FPDF_Doc_Load function
 * @param[in] page_index		A zero-based index of the page
 * @param[in] pattern			A zero-terminated unicode string to be found. 
 * @param[in] case_sensitive	Non-zero means case-sensitive searching, while zero means case-insensitive
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_QuickSearch(FPDF_DOCUMENT document, FS_INT32 page_index, 
								FS_LPCWSTR pattern, FS_INT32 case_sensitive);

/** 
 * @brief Start a search. 
 *
 * @param[in]  textPage			A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[in]  findWhat			A unicode match pattern. <br>
 *								If this parameter is <b>NULL</b> or the content to be find is <b>NULL</b>,
 *								the function will return ::FS_ERR_PARAM and set <i>searchHandle</i> to <b>NULL</b>.
 * @param[in]  flags			Indicate the find options.  Can be one of the following: <br>
 *								<ul>
 *									<li>::FPDF_MATCHCASE</li>
 *									<li>::FPDF_MATCHWHOLEWORD</li>	
 *								</ul>
 * @param[in]  startIndex		A zero-based index specifying the character from which the search will start. -1 means the end of the page.<br>
 * @param[out] searchHandle		A search context handle. ::FPDF_Text_FindClose must be called to release this handle. 		
 *
 * @return	::FS_ERR_SUCCESS means start a search successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 * @note Assume that <i>count</i> means the count of charaters in the page. So the range of the parameter <i>startIndex</i> is from 0 to (<i>count</i> - 1).
 */
FS_RESULT FPDF_Text_FindStart(FPDF_TEXTPAGE textPage, FS_LPCWSTR findWhat, FS_DWORD flags,
							  FS_INT32 startIndex, FPDF_SCHHANDLE* searchHandle);

/** 
 * @brief Search in the direction from page start to end. 
 *
 * @param[in]  handle			A search context handle returned by ::FPDF_Text_FindStart. 
 * @param[out] isMatch			Used to receive a boolean value which indicates whether a match is found or not.
 *
 * @return	::FS_ERR_SUCCESS means find the next matched content successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.

 *
 */
FS_RESULT FPDF_Text_FindNext(FPDF_SCHHANDLE handle, FS_BOOL* isMatch);

/** 
 * @brief Search in the direction from page end to start. 
 *
 * @param[in]  handle			A search context handle returned by ::FPDF_Text_FindStart. 
 * @param[out] isMatch			Used to receive a boolean value which indicates whether a match is found or not.
 *
 * @return	::FS_ERR_SUCCESS means find the previous matched content successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_FindPrev(FPDF_SCHHANDLE handle, FS_BOOL* isMatch);

/** 
 * @brief Get the starting character index of the search result. 
 *
 * @param[in]  handle			A search context handle returned by ::FPDF_Text_FindStart. 
 * @param[out] index			A zero-based index for the starting character. 
 *
 * @return	::FS_ERR_SUCCESS means get the index successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetSchResultIndex(FPDF_SCHHANDLE handle, FS_INT32* index);

/** 
 * @brief Get the count of matched characters in the search result.  
 *
 * @param[in]  handle			A search context handle returned by ::FPDF_Text_FindStart. 
 * @param[out] count			The cout of matched characters. 
 *
 * @return	::FS_ERR_SUCCESS means get the count of matched characters successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_GetSchCount(FPDF_SCHHANDLE handle, FS_INT32* count);

/** 
 * @brief Release a search context.     
 *
 * @param[in] handle			A search context handle returned by ::FPDF_Text_FindStart. 
 *
 * @return	::FS_ERR_SUCCESS means release a search context successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Text_FindClose(FPDF_SCHHANDLE handle);

/**
 * @brief Process the page text for URL formatted text.
 *
 * @details This function must be called before any other extracted link related function can be called for the page.
 *
 * @param[in]	textPage		A text page handle returned by ::FPDF_Text_LoadPage.
 * @param[out]	linkPage		Used to receive an extracted link handle.
 *
 * @return	::FS_ERR_SUCCESS means load successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Link_LoadWebLinks(FPDF_TEXTPAGE textPage, FPDF_PAGELINK* linkPage);

/**
 * @brief Get number of URL formatted strings inside a page.
 *
 * @param[in]	linkPage		An extracted links handle.
 * @param[out]	linkCount		Used to receive the count of links.
 *
 * @return	::FS_ERR_SUCCESS means count the URL successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Link_CountWebLinks(FPDF_PAGELINK linkPage, FS_INT32* linkCount);

/**
 * @brief Get a URL destination associated with a particular extracted hyperlink.
 *
 * @details If the parameter <i>buffer</i> is <b>NULL</b>, this function will set the data size to the parameter <i>buflen</i> as a result.
 *
 * @param[in]  linkPage			An extracted links handle.
 * @param[in]  linkIndex		A zero-based index of the link.
 * @param[out] buffer			A buffer (allocated by the application) for the entire ::FPDF_URLDEST structure.
 * @param[out] buflen			Used to receive the data block size for the destination, including the null terminator.
 *								If this parameter is <b>NULL</b>, the data size won't be retrieved.
 *
 * @return	::FS_ERR_SUCCESS means get the URL destination successfully or just get the actual size of the URL.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note In order to get a specified URL destination, the user should call this function twice. <br>
 *		 First, call this function to get the <i>buflen</i> of the URL which is to be got. And use the <i>buflen</i> to allocate the memory space for the member of ::FPDF_URLDEST.<br>
 *		 Then call this function again, using the allocated ::FPDF_URLDEST and the got <i>buflen</i>, to get the specified URL destination.<br>
 *		 Assume that <i>nCount</i> is the result generated by ::FPDF_Link_CountWebLinks.<br>
 *		 The range of <i>linkIndex</i> should be 0 to (<i>nCount</i> - 1), including 0 and (<i>nCount</i> - 1).
 */
FS_RESULT FPDF_Link_GetDest(FPDF_PAGELINK linkPage, FS_INT32 linkIndex, FPDF_URLDEST* buffer, FS_LPDWORD buflen);

/**
 * @brief Get the count of areas (rectangles) for an extracted link.
 *
 * @param[in]	linkPage		An extracted links handle.
 * @param[in]	linkIndex		A zero-based index of the link.
 * @param[out]	count			Used to receive the count of quadrilaterals.
 *
 * @return	::FS_ERR_SUCCESS means count the rectangles successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Link_CountRects(FPDF_PAGELINK linkPage, FS_INT32 linkIndex, FS_INT32* count);

/**
 * @brief Get a particular rectangle for an extracted link.
 *
 * @details The result in the parameter <i>points</i> array are the values of X/Y-coordinate for the four vertices
 *			of the rectangle. <br>Vertices are in the following order: lower left, lower
 *			right, upper right, upper left.
 *
 * @param[in] linkPage			An extracted links handle.
 * @param[in] linkIndex			A zero-based index of the link.
 * @param[in] areaIndex			A zero-based index of the quadrilateral.
 * @param[out] points			A pointer to an array consists 4 points, receiving coordinations.
 *
 * @return	::FS_ERR_SUCCESS means get the particular rectangle successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Link_GetRect(FPDF_PAGELINK linkPage, FS_INT32 linkIndex, FS_INT32 areaIndex, FS_POINTF* points); 

/**
 * @brief Release extracted hyperlink information.
 *
 * @param[in] linkPage			An extracted links handle.
 *
 * @return	::FS_ERR_SUCCESS means release extracted hyperlink information successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Link_CloseWebLinks(FPDF_PAGELINK linkPage);

#ifdef __cplusplus
};
#endif

#endif
// _FPDFTEXT_H_
/**@}*/

