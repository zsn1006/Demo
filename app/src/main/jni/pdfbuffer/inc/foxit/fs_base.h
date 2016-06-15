/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation.
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  *	You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FSBASEDATA" module to be enabled in your SDK license.
  * @file	fs_base.h
  * @author	Foxit Corporation
  * @brief	Header file for the Base Data module.<br>
  *			This is the base module in this SDK. Other modules all depend on this base module.
  *			It contains:<br>
  *			1. Definitions of basic data types used in this SDK.<br>
  *			2. Methods for initializiing the SDK.<br>
  *			3. Methods for loading font and graphic image engine.<br>
  *			4. Basic methods for operating images.<br>
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Base Data module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FSBASEDATA Base Data 
 * @brief Definitions and Methods in this module are included in fs_base.h and fpdf_base.h .
 */
/**@{*/

#ifndef _FS_BASE_H_
#define _FS_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name Basic Types Definitions
 */
/**@{*/
/** @brief A pointer to any type. */
typedef void*					FS_LPVOID;

/** @brief A pointer to any constant type. */
typedef void const*				FS_LPCVOID;

/** @brief Boolean variable (This should be <b>TRUE</b> or <b>FALSE</b>). */
typedef int						FS_BOOL;

/** @brief 32-bit unsigned integer. */
typedef unsigned int			FS_UINT;

/** @brief A byte (8 bits). */
typedef unsigned char			FS_BYTE;

/** @brief A pointer to a ::FS_BYTE. */
typedef unsigned char*			FS_LPBYTE;
	
/** @brief A pointer to a constant ::FS_BYTE. */
typedef unsigned char const*	FS_LPCBYTE;
	
/** @brief 16-bit unsigned integer. */
typedef unsigned short			FS_WORD;

/** @brief A pointer to a ::FS_WORD. */
typedef unsigned short*			FS_LPWORD;
	
/** @brief 32-bit unsigned integer. */
typedef unsigned int			FS_DWORD;

/** @brief A pointer to a ::FS_DWORD. */
typedef unsigned int*			FS_LPDWORD;

/** @brief The ARGB color type. */
typedef unsigned int			FS_ARGB;
	
/** @brief The 32-bit floating-point number. */
typedef float					FS_FLOAT;
		
/** @brief 8-bit Windows (ANSI) character. */
typedef char					FS_CHAR;

/** @brief A pointer to 8-bit Windows (ANSI) characters. */
typedef char*					FS_LPSTR;
	
/** @brief A pointer to a byte string */
typedef char const*				FS_LPCSTR;
	
/** @brief 16-bit Unicode character. */
typedef unsigned short			FS_WCHAR;

/** @brief A pointer to 16-bit Unicode character. */
typedef unsigned short*			FS_LPWSTR;
	
/** @brief A pointer to a wide string encoded in UTF-16LE */
typedef unsigned short const*	FS_LPCWSTR;
	
/** @brief The result code for functions. */
typedef int						FS_RESULT;

/** @brief 8-bit integer types. */
typedef char					FS_INT8;

/** @brief 8-bit unsigned integer types. */
typedef unsigned char			FS_UINT8;

/** @brief 16-bit integer types. */
typedef short					FS_INT16;

/** @brief 16-bit unsigned integer types. */
typedef unsigned short			FS_UINT16;

/** @brief 32-bit integer types. */
typedef int						FS_INT32;

/** @brief 32-bit unsigned integer types. */
typedef unsigned int			FS_UINT32;

/** @cond IGNORE*/
//Macro definition of defining a handle type.
#define FS_DEFINEHANDLE(name)	typedef struct _##name {FS_LPVOID pData;} * name;
/** @endcond*/

/** @brief Bitmap handle type. */
FS_DEFINEHANDLE(FS_BITMAP);

/** @brief System or File font handler type */
FS_DEFINEHANDLE(FS_FONT);
/**@}*/

#ifndef _FS_DEF_RECT_
#define _FS_DEF_RECT_
/**  @brief Rectangle area in device or page coordination system. */
typedef struct _FS_RECT_
{
	/** @brief The x-coordinate of the left-top corner. */
	FS_INT32		left;
	/** @brief the y-coordinate of the left-top corner. */
	FS_INT32		top;
	/** @brief The x-coordinate of the right-bottom corner. */
	FS_INT32		right;
	/** @brief The y-coordinate of the right-bottom corner. */
	FS_INT32		bottom;
} * FS_LPRECT, FS_RECT;
/** @brief Const Pointer to ::FS_RECT structure.*/
typedef const FS_RECT*	FS_LPCRECT;
#endif

#ifndef _FS_DEF_RECTF_
#define _FS_DEF_RECTF_
/**
 * @brief Rectangle area(float) in device or page coordination system
 */
typedef struct _FS_RECTF_
{
	/** @brief The x-coordinate of the left-top corner. */
	FS_FLOAT	left;
	/** @brief The y-coordinate of the left-top corner. */
	FS_FLOAT	top;
	/** @brief The x-coordinate of the right-bottom corner. */
	FS_FLOAT	right;
	/** @brief The y-coordinate of the right-bottom corner. */
	FS_FLOAT	bottom;
}* FS_LPRECTF, FS_RECTF;
/** @brief Const Pointer to ::FS_RECTF structure.*/
typedef const FS_RECTF*	FS_LPCRECTF;
#endif

#ifndef _FS_DEF_POINT_
#define _FS_DEF_POINT_
/**
 * @brief A point in device or page coordination system
 */
typedef struct _FS_POINT_
{
	/** @brief The x-coordinate. */
	FS_INT32	x;
	/** @brief The y-coordinate. */
	FS_INT32	y;
} FS_POINT;
#endif

#ifndef _FS_DEF_POINTF_
#define _FS_DEF_POINTF_
/**
 * @brief A point(float) in device or page coordination system
 */
typedef struct _FS_POINTF_
{
	/** @brief The x-coordinate. */
	FS_FLOAT	x;
	/** @brief The y-coordinate. */
	FS_FLOAT	y;
} FS_POINTF;
#endif

#ifndef _FS_DEF_PAUSE_
#define _FS_DEF_PAUSE_
/**
 * @brief Structure for pausing a progressive process.
 */
typedef struct _FS_PAUSE_
{
	
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FS_PAUSE). */
	FS_DWORD	lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	 * @brief Check if we need to pause a progressive process now.
	 *
	 * @details Typically implementation of this interface compares the current system tick
	 *		with the previous one, if the time elapsed exceeds certain threshold, then
	 *		the implementation returns <b>TRUE</b>, indicating a pause is needed.
	 *
	 * @param[in] clientData			The user-supplied data.
	 *
	 * @return <b>TRUE</b> means to pause now, while <b>FALSE</b> means to continue.
	 */
	FS_BOOL		(*NeedPauseNow)(FS_LPVOID clientData);

}FS_PAUSE;
#endif

#ifndef _FS_DEF_FILEREAD_
#define _FS_DEF_FILEREAD_
/** @brief Structure for reading a file. */
typedef struct  _FS_FILEREAD_
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FS_FILEREAD). */
	FS_DWORD	lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	* @brief A callback function used to get the total size of the file.
	*
	* @param[in] clientData		The user-supplied data.
	*
	* @return	The file size, in bytes. Implementation can return 0 for any error.
	*/
	FS_DWORD	(*GetSize)(FS_LPVOID clientData);

	/**
	* @brief A callback funtion used to read a data block from the file.
	*
	* @param[in] clientData		The user-supplied data.
	* @param[out] buffer		The pointer to a buffer receiving the read data
	* @param[in] offset			Byte offset for the block, from the beginning of the file.
	* @param[in] size			The number of bytes for the block.
	*
	* @return	::FS_ERR_SUCCESS means success.<br>
	*			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
	*/
	FS_RESULT	(*ReadBlock)(FS_LPVOID clientData, FS_LPVOID buffer, FS_DWORD offset, FS_DWORD size);

	/**
	* @brief A callback function can be called when to release everything.
	*
	* @param[in] clientData		The user-supplied data.
	*
	* @return None
	*/
	void		(*Release)(FS_LPVOID clientData);

} FS_FILEREAD;
#endif

#ifndef _FS_DEF_FILEWRITE_
#define _FS_DEF_FILEWRITE_
/** @brief Structure for writing a file. */
typedef struct  _FS_FILEWRITE_
{
	/** @brief The size of the data structure. It must be set to <b>sizeof</b>(::FS_FILEWRITE). */
	FS_DWORD	lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	 * @brief Get the current size of the file
	 *
	 * @param[in] clientData		The user-supplied data.
	 *
	 * @return	The file size, in bytes.<br> -1 means error.
	 */
	FS_DWORD	(*GetSize)(FS_LPVOID clientData);

	/**
	 * @brief	Write a data block to the file.
	 *
	 * @param[in] clientData		The user-supplied data.
	 * @param[in] buffer			A pointer to a buffer containing the data which is to be written into the file.
	 * @param[in] offset			The offset in byte for the block, from the beginning of the file.
	 * @param[in] size				The count of bytes of the block.
	 *
	 * @return	::FS_ERR_SUCCESS means success.<br>
	 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
	 */
	FS_RESULT	(*WriteBlock)(FS_LPVOID clientData, FS_LPCVOID buffer, FS_DWORD offset, FS_DWORD size);
	/**
	 * @brief	Flush a stream.
	 *
	 * @param[in] clientData		The user-supplied data.
	 *
	 * @return	::FS_ERR_SUCCESS means success.<br>
	 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
	 */
	FS_RESULT	(*Flush)(FS_LPVOID clientData);

} FS_FILEWRITE;
#endif

#ifndef _FS_DEF_MATRIX_
#define _FS_DEF_MATRIX_
/** @brief the matrix for transformation. */
typedef struct _FS_MATRIX_
{
	FS_FLOAT	a;	/**< @brief Coefficient a.*/
	FS_FLOAT	b;	/**< @brief Coefficient b.*/
	FS_FLOAT	c;	/**< @brief Coefficient c.*/
	FS_FLOAT	d;	/**< @brief Coefficient d.*/
	FS_FLOAT	e;	/**< @brief Coefficient e.*/
	FS_FLOAT	f;	/**< @brief Coefficient f.*/

} FS_MATRIX;
#endif

#ifndef _FS_DEF_MEMFIXEDMGR_
#define _FS_DEF_MEMFIXEDMGR_
/**
 * @brief Interfaces for extensible fixed memory manager. This kind of memory manager starts with
 *			a fixed memory pool but when it runs out, the manager will call out and ask for more
 *			memory.
 */
typedef struct _FS_MEM_FIXEDMGR_
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FS_MEM_FIXEDMGR). */
	FS_DWORD	lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	 * @brief Called when the memory manager needs more memory to allocate some block.
	 * 
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] alloc_size	Size of the object the SDK is trying to allocate.
	 * @param[out] new_memory	Receiving newly allocated memory pool.
	 * @param[out] new_size		Receiving newly allocated size. This size must be same or bigger than alloc_size.
	 *
	 * @return Nonzero for successfully allocated new memory pool, zero for memory not available.
	 */
	FS_BOOL		(*More)(FS_LPVOID clientData, FS_INT32 alloc_size, FS_LPVOID* new_memory, FS_INT32* new_size);

	/**
	 * @brief Called when a memory pool become empty and can be released
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] memory		Pointer to the memory pool.
	 *
	 * @return None.
	 */
	void		(*Free)(FS_LPVOID clientData, FS_LPVOID memory);

	/**
	 * @brief Fixed memory OOM(out-of-memory) handler type. 
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] memory		Pointer to the memory pool.
	 * @param[in] size			The size of the memory pool.
	 *
	 * @return None.
	 */
	void		(*OOMHandler)(FS_LPVOID clientData, FS_LPVOID memory, FS_INT32 size);

} FS_MEM_FIXEDMGR;
#endif

#ifndef _FS_DEF_MEMMGR_
#define _FS_DEF_MEMMGR_
/**
 * @brief Including interfaces implemented by host application, providing memory allocation
 *			facilities. All members are required.
 *			A memory manager structure is required to be valid during the entire period
 *			when an application using the Embedded SDK.
 *
 * @note    using of this interface is strongly not recommended, because
 *			the Embedded SDK now internally use ::FS_MEM_MGR_EX , which allows more
 *			advanced memory management. This interface is retained for backward compatibility
 *			only, and maybe discontinued in the future.
 */
typedef struct _FS_MEM_MGR_ 
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FS_MEM_MGR). */
	FS_DWORD	lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	 * @brief Allocate a memory block.
	 *
	 * @details In order to handle OOM situation, application can use longjmp() inside 
	 *			implementation of this function. If underlying memory manager fails to 
	 *			allocate enough memory, then application can use longjmp() to jump to
	 *			OOM handling codes.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] size			Number of bytes for the memory block.
	 *
	 * @return The pointer to allocated memory block. NULL if no memory available.
	 */
	FS_LPVOID	(*Alloc)(FS_LPVOID clientData, FS_DWORD size);

	/**
	 * @brief Allocate a memory block, without leaving.
	 *
	 * @details Implementation MUST return NULL if no memory available, no exception
	 *			or longjmp() can be used.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] size			Number of bytes for the memory block.
	 *
	 * @return The pointer to allocated memory block. NULL if no memory available.
	 */
	FS_LPVOID	(*AllocNL)(FS_LPVOID clientData, FS_DWORD size);

	/**
	 * @brief Reallocate a memory block.
	 *
	 * @details If an existing memory block specified, the data in the memory block will
	 *			be copied to the new block, if reallocated.<br>
	 *			In order to handle OOM situation, application can use longjmp() inside 
	 *			implementation of this function. If underlying memory manager fails to 
	 *			allocate enough memory, then application can use longjmp() to jump to
	 *			OOM handling codes.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] pointer		An existing memory block, or NULL.
	 * @param[in] new_size		New size (number of bytes) of the memory block. This can be zero.
	 *
	 * @return The pointer of reallocated memory block, it could be a new block, or just
	 *		   the previous block with size modified.
	 */
	FS_LPVOID	(*Realloc)(FS_LPVOID clientData, FS_LPVOID pointer, FS_DWORD new_size);

	/**
	 * @brief Free a memory block.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] pointer		An existing memory block.
	 *
	 * @return None.
	 */
	void		(*Free)(FS_LPVOID clientData, FS_LPVOID pointer);

} FS_MEM_MGR;
#endif

#ifndef _FS_DEF_MEMMGREX_
#define _FS_DEF_MEMMGREX_

/**
 * @name Memory Management Flags
 */
/**@{*/
/** @brief Non-leave. */
#define FS_MEM_NONLEAVE			1
/**@}*/

/**
 * @brief This is an extended version of memory manager interface, allowing advanced
 *			memory management, including movable and discardable memory blocks.
 *
 * @see ::FS_Memory_InitEx.
 */
typedef struct _FS_MEM_MGR_EX_ 
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FS_MEM_MGR_EX). */
	FS_DWORD	lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	 * @brief Allocate a memory block.
	 *
	 * @details The implementation should not do any action if no memory available,
	 *			just return NULL. OOM handling can be done in OOM_Handler interface.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] size			Number of bytes for the memory block.
	 * @param[in] flags			A combination of flags. See macro definitions <b>FS_MEM_XXX</b>.
	 *
	 * @return The pointer to allocated memory block. NULL if no memory available.<br>
	 */
	FS_LPVOID	(*Alloc)(FS_LPVOID clientData, FS_DWORD size, FS_INT32 flags);

	/**
	 * @brief OOM (out-of-memory) situation handler.
	 *
	 * @details In order to handle OOM situation, application can use longjmp() inside 
	 *			implementation of this function.
	 *
	 * @param[in] clientData	The user-supplied data.
	 *
	 * @return None.
	 */
	void		(*OOMHandler)(FS_LPVOID clientData);

	/**
	 * @brief Reallocate a memory block
	 *
	 * @details If an existing memory block specified, the data in the memory block should
	 *			be copied to the new block, if reallocated.<br>
	 *			The implementation should not do any action if no memory available,
	 *			just return NULL. OOM handling can be done in OOM_Handler interface.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] pointer		Pointer to an existing memory block, or handle to a movable
	 *							block. Can not be NULL.
	 * @param[in] new_size		New size (number of bytes) of the memory block. Can not be zero.
	 * @param[in] flags			A combination of flags. See macro definitions <b>FS_MEM_XXX</b>.
	 *
	 * @return The pointer of reallocated memory block, it could be a new block, or just
	 *		   the previous block with size modified.
	 */
	FS_LPVOID	(*Realloc)(FS_LPVOID clientData, FS_LPVOID pointer, FS_DWORD new_size, FS_INT32 flags);

	/**
	 * @brief Free a memory block
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] pointer		Pointer to an existing memory block, or handle to a movable block.
	 * @param[in] flags			A combination of flags. See macro definitions <b>FS_MEM_XXX</b>.
	 *
	 * @return None.
	 */
	void		(*Free)(FS_LPVOID clientData, FS_LPVOID pointer, FS_INT32 flags);

} FS_MEM_MGR_EX;
#endif

/**
 * @name Macro Definitions for Font Flags.
 */
/**@{*/
/** @brief Fixed pitch. */
#define FS_FONT_FIXEDPITCH		1
/** @brief Serif. */
#define FS_FONT_SERIF			2
/** @brief Symbolic. */
#define FS_FONT_SYMBOLIC		4
/** @brief Script. */
#define FS_FONT_SCRIPT			8
/** @brief Non-symbolic. */
#define FS_FONT_NONSYMBOLIC		32
/** @brief Italic. */
#define FS_FONT_ITALIC			64
/** @brief All cap. */
#define FS_FONT_ALLCAP			0x10000
/** @brief Small cap. */
#define FS_FONT_SMALLCAP		0x20000
/** @brief Force bold. */
#define FS_FONT_FORCEBOLD		0x40000
/**@}*/

#ifndef _FS_DEF_GLYPHPROVIDER_
#define _FS_DEF_GLYPHPROVIDER_
/**
 * @brief Interface for providing glyph bitmap of non-latin characters.
 *			This is usually used for embedded devices with Chinese/Japanese/Korean 
 *			fonts installed, but those fonts are not in TrueType or Type1 format.
 */
typedef struct _FS_GLYPH_PROVIDER_
{	
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FS_GLYPH_PROVIDER). */
	FS_DWORD	lStructSize;
	
	/** @brief A user pointer, used by the application. */
	FS_LPVOID	clientData;

	/**
	 * @brief Return an internal handle for a font
	 * 
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] name			Font name
	 * @param[in] charset		Charset ID. See macro definitions <b>FS_CHARSET_XXX</b>.
	 * @param[in] flags			Font flags. See macro definitions <b>FS_FONT_XXX</b>.
	 * @param[in] weight		Weight of the font. Range from 100 to 900. 400 is normal,
	 * 							700 is bold.
	 *
	 * @return	An internal handle to the mapped font. If the embedded device supports
	 *			multiple fonts, then this value can serve as an identifier to differentiate
	 *			among them. If the device supports only one font, then implementation of
	 *			this function can simply return NULL.
	 */
	FS_LPVOID	(*MapFont)(FS_LPVOID clientData, FS_LPCSTR name, FS_INT32 charset,
						   FS_DWORD flags, FS_INT32 weight);
	/**
	 * @brief Get glyph bounding box
	 *
	 * @details The bounding box is measure in a glyph coordination system, in which the
	 *			origin is set to character origin, and unit is set to one-thousandth of
	 *			"em size" (representing the font size).<br>
	 *			In most CJK fonts, all CJK characters (except some symbols or western 
	 *			characters) have same glyph bounding box:
	 *			left = 0, right = 1000, bottom = -220, top = 780.<br>
	 *			It's OK to return a box that's larger than the actual glyph box.
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] font			Internal handle to the font. Returned by MapFont interface.
	 * @param[in] unicode		Unicode of the character
	 * @param[in] CID			Adobe CID for this character. This is reserved and it should be set 0.
	 * @param[out] bbox			Receiving the result bounding box. See comments below.
	 *
	 * @return None.
	 */
	void		(*GetGlyphBBox)(FS_LPVOID clientData, FS_LPVOID font,
								FS_WCHAR unicode, FS_WORD CID,
								FS_RECT* bbox);

	/**
	 * @brief Get bitmap of a glyph
	 *
	 * @details The buffer should be allocated by implementation. And it must be allocated
	 *			using ::FS_Memory_Alloc. The result buffer will be destroyed by
	 *			Embedded SDK, so implementation should not destroy it.<br>
	 *			The implementation should write "coverage" data into allocated buffer, one byte 
	 *			for each pixel, from top scanline to bottom scanline, within each scan line, 
	 *			from left pixel to right. Coverage 0 means the pixel is outside the glyph, 
	 *			coverage 255 means the pixel is inside the glyph.<br>
	 *			The "pdf_width" parameter can be used to scale the character in system font
	 *			horizontally to match the font width specified in PDF. For example, if we have
	 *			a PDF file which requires a character in half-width (pdf_width is 500), but
	 *			in the system font the character has full-width (1000), then the glyph provider
	 *			implementation should scale the font horizontally to half of its original width.
	 *
	 * @param[in] clientData		The user-supplied data.
	 * @param[in] font				Internal handle to the font. Returned by MapFont interface.
	 * @param[in] unicode			Unicode of the character
	 * @param[in] CID				Adobe CID for this character. This is reserved and it should be set 0.
	 * @param[in] font_width		Width of the font em square, measured in device units.
	 * @param[in] font_height		Height of the font em square, measured in device units.
	 * @param[out] left				Receiving the left offset, from the character origin, of the
	 *								result glyph bitmap. Positive value will move the bitmap to
	 *								the right side, negative to the left.
	 * @param[out] top				Receiving the top offset, from the character origin, of the
	 *	 							result glyph bitmap. Positive value will move the bitmap upward,
	 *								negative downward.
	 * @param[out] bitmap_width		Receiving number of width pixels in the result bitmap
	 * @param[out] bitmap_height	Receiving number of height pixels in the result bitmap
	 * @param[out] buffer			Receiving a data buffer pointer, allocated by the implementation.
	 * @param[out] stride			Receiving number of bytes per scanline, in the data buffer.
	 * @param[in] pdf_width			Width of the character specified in PDF. It is measured in one-
	 *								thousandth of the font width. It can be 0 if width not specified in PDF. 
	 *
	 * @return Non-zero for success. 0 for failure. In this case the glyph can not be displayed.
	 */
	FS_BOOL		(*GetGlyphBitmap)(FS_LPVOID clientData, FS_LPVOID font,
								  FS_WCHAR unicode, FS_WORD CID,
								  FS_FLOAT font_width, FS_FLOAT font_height, FS_INT32* left, FS_INT32* top,
								  FS_INT32* bitmap_width, FS_INT32* bitmap_height, 
								  FS_LPVOID* buffer, FS_INT32* stride, FS_INT32 pdf_width);

} FS_GLYPH_PROVIDER;
#endif

#ifndef _FS_DEF_FONTMAPPER_
#define _FS_DEF_FONTMAPPER_
/**
 * @brief Defines interface for system font mapper.
 * @note This should be declared to be a global variable.
 */
typedef struct _FS_FONT_FILE_MAPPER_
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FS_FONT_FILE_MAPPER). */
	FS_DWORD	lStructSize;

	/** @brief A user pointer, used by the application. */
	FS_LPVOID	clientData;

	/**
	 * @brief Find font file path for a particular PDF font
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] name			Font name
	 * @param[in] charset		Charset ID. See macro definitions <b>FS_CHARSET_XXX</b>.
	 * @param[in] flags			Font flags. See macro definitions <b>FS_FONT_XXX</b>.
	 * @param[in] weight		Weight of the font. Range from 100 to 900. 400 is normal,
	 * 							700 is bold.
	 * @param[out] path			Receiving the full file path. The buffer size is 512 bytes.
	 * @param[out] face_index	Receiving an zero-based index value for the font face, if the 
	 * 							mapped font file is a "collection" (meaning a number of faces 
	 *							are stored in the same file). If the font file is not a 
	 *							collection, the index value should be zero.
	 *
	 * @return	Non-zero for a substitution font has be specified.
	 *			Zero if the mapper doesn't map the font, or something is wrong.
	 */
	FS_BOOL		(*MapFont)(FS_LPVOID clientData, FS_LPCSTR name, FS_INT32 charset,
						   FS_DWORD flags, FS_INT32 weight,
						   FS_CHAR* path, FS_INT32* face_index);

} FS_FONT_FILE_MAPPER;
#endif

#ifndef _FS_DEF_FONTMAPPEREX_
#define _FS_DEF_FONTMAPPEREX_
/**
 * @brief Defines interface for system font mapper.
 */
typedef struct _FS_FONT_DATA_MAPPER_
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FS_FONT_DATA_MAPPER). */
	FS_DWORD	lStructSize;

	/** @brief A user pointer, used by the application. */
	FS_LPVOID	clientData;	

	/**
	 * @brief Find font Data for a particular PDF font
	 *
	 * @param[in] clientData	The user-supplied data.
	 * @param[in] name			The font name.
	 * @param[in] charset		Charset ID. See macro definitions <b>FS_CHARSET_XXX</b>.
	 * @param[in] flags			Font flags. See macro definitions <b>FS_FONT_XXX</b>. 
	 * @param[in] weight		Weight of the font. Range from 100 to 900. 400 is normal,
	 * 							700 is bold.
	 * @param[out] fontdata		Pointer to the external data.
	 * @param[out] fontsize     Number of bytes in the external data.
 	 * @param[out] face_index	Receiving an zero-based index value for the font face, if the 
	 * 							mapped font file is a "collection" (meaning a number of faces 
	 *							are stored in the same file). If the font file is not a 
	 *							collection, the index value should be zero.
	 *
	 * @return	Non-zero for a substitution font has be specified.
	 *			Zero if the mapper doesn't map the font, or something is wrong.
	 */
	FS_BOOL		(*MapFont)(FS_LPVOID clientData, FS_LPCSTR name, FS_INT32 charset,
						   FS_DWORD flags, FS_INT32 weight,
						   FS_LPSTR* fontdata, FS_INT32* fontsize,FS_INT32* face_index);

} FS_FONT_DATA_MAPPER;
#endif

/**
 * @name Macro Definitions for Charset defines.
 */
/**@{*/
/** @brief Default. */
#define FS_CHARSET_DEFAULT		0
/** @brief For simplified Chinese. */
#define FS_CHARSET_GB			936
/** @brief For traditional Chinese. */
#define FS_CHARSET_BIG5			950
/** @brief For Japanese. */
#define FS_CHARSET_JIS			932
/** @brief For Korea. */
#define FS_CHARSET_KOREA		949
/** @brief Unicode. */
#define FS_CHARSET_UNICODE		1200
/**@}*/

/**
 * @name Macro Definitions for Error Codes 
 */
/**@{*/
/** @brief Success. */
#define FS_ERR_SUCCESS			0
/** @brief Out of memory. */
#define FS_ERR_MEMORY			1
/** @brief Error of any kind, without specific reason. */
#define FS_ERR_ERROR			2
/** @brief Incorrect password. */
#define FS_ERR_PASSWORD			3
/** @brief File or data format error. */
#define FS_ERR_FORMAT			4
/** @brief File access error. */
#define FS_ERR_FILE				5
/** @brief Parameter error. */
#define FS_ERR_PARAM			6
/** @brief Not in correct status. */
#define FS_ERR_STATUS			7
/** @brief To be continued. */
#define FS_ERR_TOBECONTINUED	8
/** @brief Search result not found. */
#define FS_ERR_NOTFOUND			9
/** @brief Returned when FPDF_PAGE has not been parsed by ::FPDF_Page_StartParse. */
#define FS_ERR_UNPARSEDPAGE		10
/** @brief Returned when there is no text information in the PDF page. */
#define FS_ERR_NOTEXTONPAGE		11
/** 
 * @brief License might be invalid, if the return value is ::FS_ERR_INVALID_LICENSE, check if ::FS_Library_Unlock was properly called. If the return value is still ::FS_ERR_INVALID_LICENSE, please contact Foxit at sales@foxitsoftware.com or support@foxitsoftware.com.
 */
#define FS_ERR_INVALID_LICENSE	0x80
/**@}*/ 

/**
 * @name DIB format.
 */
/**@{*/
/** @brief 3 bytes per pixel, byte order: Blue, Green, Red. */
#define FS_DIBFORMAT_BGR		1
/** @brief 4 bytes per pixel, byte order: Blue, Green, Red, not used. */
#define FS_DIBFORMAT_BGRx		2
/** @brief 4 bytes per pixel, byte order: Blue, Green, Red, Alpha. */
#define FS_DIBFORMAT_BGRA		3
/** @brief 1 byte per pixel (grayscale). */
#define FS_DIBFORMAT_GRAY		4
/** @brief 3 bytes per pixel, byte order: Red, Green, Blue. */
#define FS_DIBFORMAT_RGB		5
/** @brief 4 bytes per pixel, byte order: Red, Green, Blue, not used. */
#define FS_DIBFORMAT_RGBx		6
/** @brief 4 bytes per pixel, byte order: Red, Green, Blue, Alpha. */
#define FS_DIBFORMAT_RGBA		7
/** @brief 2 bytes per pixel, byte order: Red 5 bits, Green 6 bits, Blue 5 bits. If this format available, other DIB formats will not be used. */
#define FS_DIBFORMAT_RGB565		8
/**@}*/

//////////////////////////////////////////////////////////////////////////
//							Interfaces									//
//////////////////////////////////////////////////////////////////////////
	
/**
 * @brief Initialize the sdk library 
 *
 * @param[in] hInstance			For WIN32 system only: the instance of the executable or DLL module.
 *								NULL for other system.
 *
 * @return	None.
 *
 * @note	This has already been used in ::FS_Memory_InitFixed, ::FS_Memory_Init and ::FS_Memory_InitEx, 
 *			so you don't need to call this function if you've called one of the three functions.
*/
void FS_Library_Init(FS_LPVOID hInstance);

/**
 * @brief Release all resources allocated by the sdk library.
 *
 * @return	None.
 *
 * @note	This has already been used in ::FS_Memory_Destroy, so you don't need to call this function if you've called 
 *			::FS_Memory_Destroy.
 */
void FS_Library_Destroy();

/**
 * @brief Unlock the library using license key information received from Foxit.
 *
 * @param[in] license_id		A string received from Foxit identifying the SDK license.
 * @param[in] unlock_code		A string received from Foxit for unlocking the library.
 *
 * @return	None.
 * @note	For SDK evaluators, this function call is not required, then all
 *			rendered pages will come with an evaluation mark.
 *			For purchased SDK customers, this should be the first function
 *			to be called after the memory has been initialized.
*/
void FS_Library_Unlock(FS_LPSTR license_id, FS_LPSTR unlock_code);

/**
 * @brief Initialize the library using an extensible fixed memory manager. With this kind of manager,
 *			when current memory pool runs out, the application may provide some additional memory block
 *			for the manager to use. 
 *
 * @param[in] memory			Pointer to a pre-allocated memory block
 * @param[in] size				Number of bytes in the memory block. The size can't be lower than 8MB, 
 *								there is no upper limit.
 * @param[in] callbacks			Callback functions when EMB SDK runs out of current memory pool, or
 *								when a pool become empty and can be dropped.
 *
 * @return	::FS_ERR_SUCCESS means initialize fixed memory successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Memory_InitFixed(FS_LPVOID memory, FS_INT32 size, FS_MEM_FIXEDMGR* callbacks);

/**
 * @brief Get the size of the occupied memory.	
 *
 * @details 	This function is used to count memory applied for by More(), and exclude the part initialized by the ::FS_Memory_InitFixed .
 *
 * @return 		The size of the occupied memory. 
 */
FS_INT32 FS_Memory_GetExtraMemoryUsedSize();

/**
 * @brief Initialize the SDK module with the extended memory manager
 * @details This function will allocate necessary internal data structure for
 *			the whole module to operate.
 *
 * @param[in] mem_mgr			Pointer to memory manager structure.
 *
 * @return	::FS_ERR_SUCCESS means initialize  memory successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Memory_Init(FS_MEM_MGR* mem_mgr);

/**
 * @brief Initialize the SDK module with the extended memory manager, with a choice of using the judgement of OOM or not.
 *
 * @details This function will allocate necessary internal data structure for
 *			the whole module to operate.<br>
 *			When using this function, you can choose to use the judgement of OOM or not.
 *
 * @param[in] mem_mgr			Pointer to memory manager structure.
 * @param[in] useOOM			A boolean value used to decide whether to use the judgement of OOM or not.
 *
 * @return	::FS_ERR_SUCCESS means initialize memory successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Memory_InitEx(FS_MEM_MGR_EX* mem_mgr,  FS_BOOL useOOM);

/**
 * @brief Free the SDK the extended memory manager and release all resources
 *
 * @details This function is useful for OOM recovery: when your application hits
 *			an OOM situation, calling this function will clear all memory allocated
 *			by Embedded SDK, then you can call one of the initialization functions,
 *			reopen the document and recovery from OOM.
 *
 * @return None.
 */
void FS_Memory_Destroy();

/**
 * @brief Allocate memory
 *
 * @param[in] size				Number of bytes
 * @param[out] pointer			The allocated buffer pointer. 
 *
 * @return  ::FS_ERR_SUCCESS means initialize memory successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FS_Memory_Alloc(FS_DWORD size, FS_LPVOID* pointer);

/**
 * @brief Free allocated memory
 *
 * @param[in] pointer			Pointer returned by ::FS_Memory_Alloc.
 *
 * @return None.
 */
void FS_Memory_Free(FS_LPVOID pointer);

/**
 * @brief Free all expendable caches used by EMB SDK in order to save memory.
 *
 * @details When an application memory manager runs out of memory, the application can try this function before an OOM situation is raised
 *
 * @return None.
 */
void FS_Memory_FreeCaches();

/**
 * @brief Create a DIB (Device Independent Bitmap)
 *
 * @details If "buffer" parameter is not NULL, then the provided buffer must conform
 *			to standard DIB format.<br>
 *			This function doesn't initialize the pixels inside the DIB buffer. So if you
 *			want to use the DIB to display a PDF page, you usually need to initialize
 *			the DIB to white background by youself.
 *
 * @param[in] width				Width pixels.
 * @param[in] height			Height pixels.
 * @param[in] format			The format type. See macro definitions <b>FS_DIBFORMAT_XXX</b>.
 * @param[in] buffer			It's often set to be <b>NULL</b>. 
 *								If it's not <b>NULL</b>, it'll be an external buffer(<b>with enough size</b>) provided for the DIB.
 * @param[in] stride			Number of bytes for each scan line, for an external buffer only.
 *								If not specified, 4-byte alignment assumed.
 * @param[out] dib				Used to Receive the created DIB handle.
 *
 * @return	::FS_ERR_SUCCESS means create a bitmap successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Bitmap_Create(FS_INT32 width, FS_INT32 height, FS_INT32 format, 
						   FS_LPVOID buffer, FS_INT32 stride, FS_BITMAP* dib);

/**
 * @brief Destroy an bitmap and release all related buffers. 
 *
 * @details If external buffer is used (specified in "buffer" parameter when calling
 *			::FS_Bitmap_Create), the buffer will not be destroyed.
 *
 * @param[in] dib				Handle to the bitmap. 
 *
 * @return	::FS_ERR_SUCCESS means destroy a bitmap successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Bitmap_Destroy(FS_BITMAP dib);

/**
 * @brief Get the width (in pixels) of a DIB
 *
 * @param[in] dib				The handle to a bitmap. 
 *
 * @return	The width of the DIB in pixels.<br>
 *			-1 means parameter error.
 */
FS_INT32 FS_Bitmap_GetWidth(FS_BITMAP dib);

/**
 * @brief Get the height (in pixels) of a DIB
 *
 * @param[in] dib				The handle to a bitmap. 
 *
 * @return	The height of the DIB in pixels.<br>
 *			-1 means parameter error.
 */
FS_INT32 FS_Bitmap_GetHeight(FS_BITMAP dib);

/**
 * @brief Get data buffer of a DIB
 *
 * @details DIB data are organized in scanlines, from top down.
 *
 * @param[in] dib				Handle to the bitmap. 
 *
 * @return	The pointer to the first byte of the bitmap buffer.<br>
 *			<b>NULL</b> means parameter error.
 */
FS_LPVOID FS_Bitmap_GetBuffer(FS_BITMAP dib);

/**
 * @brief Get the number of bytes for each scan line in the bitmap buffer.
 *
 * @param[in] dib				The handle to the bitmap. 
 *
 * @return	The number of bytes for each scan line in the bitmap buffer.<br>
 *			-1 means parameter error.
 */
FS_INT32 FS_Bitmap_GetStride(FS_BITMAP dib);

/**
 * @brief Swap X/Y dimensions of a DIB to generate a rotated new DIB
 *
 * @param[in] dib				Handle to the bitmap. 
 * @param[in] bFlipX			Whether flip pixels on the destination X dimension (left/right)
 * @param[in] bFlipY			Whether flip pixels on the destination Y dimension (up/down)
 * @param[out] result_dib		Receiving the result DIB handle
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Bitmap_GetFlippedDIB(FS_BITMAP dib, 
							   FS_BOOL bFlipX, FS_BOOL bFlipY,
							   FS_BITMAP* result_dib);

/**
 * @brief Stretch a source DIB into another destination DIB
 *
 * @param[out] dest_dib			The destination DIB handle
 * @param[in] dest_left			Left position in the destination DIB
 * @param[in] dest_top			Top position in the destination DIB
 * @param[in] dest_width		Destination width, in pixels. This can be negative for horizontal flipping
 * @param[in] dest_height		Destination height, in pixels. This can be negative for vertical flipping
 * @param[in] clip_rect			Destination clipping rectangle, or NULL for no clipping.
 *								The coordinations are measured in destination bitmap.
 * @param[in] src_dib			Source DIB handle.
 * @param[in] interpol			Whether we use interpolation to improve the result quality
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Bitmap_Stretch(FS_BITMAP dest_dib, FS_INT32 dest_left, FS_INT32 dest_top,
							FS_INT32 dest_width, FS_INT32 dest_height, FS_RECT* clip_rect, 
							FS_BITMAP src_dib, FS_BOOL interpol);

/**
 * @brief Transform a source DIB into another destination DIB
 *
 * @details All coordinations and distances are measured in destination bitmap system.<br>
 *			This function places the bottom-left pixel of the image at the destination
 *			origin, then the bottom sideline along the destination X vector, and left 
 *			sideline along the destination Y vector.
 *
 * @param[out] dest_dib			The destination DIB handle
 * @param[in] clip_rect			Destination clipping rectangle, or NULL for no clipping.
 * 								The coordinations are measured in destination bitmap.
 * @param[in] src_dib			The source DIB handle.
 * @param[in] x					X coordination of the dest origin
 * @param[in] y					Y coordination of the dest origin
 * @param[in] xx				X distance of the dest X vector
 * @param[in] yx				Y distance of the dest X vector
 * @param[in] xy				X distance of the dest Y vector
 * @param[in] yy				Y distance of the dest Y vector
 * @param[in] interpol			Whether we use interpolation to improve the result quality
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Bitmap_Transform(FS_BITMAP dest_dib, FS_RECT* clip_rect, 
							  FS_BITMAP src_dib, FS_INT32 x, FS_INT32 y, FS_INT32 xx, FS_INT32 yx,
							  FS_INT32 xy, FS_INT32 yy, FS_BOOL interpol);

/**
 * @brief Output a glyph onto a DIB device
 *
 * @param[in] dib				DIB handle, as the output device
 * @param[in] x					DIB x-coordination for the glyph origin
 * @param[in] y					DIB y-coordination for the glyph origin.
 * @param[in] font_handle		Handle to the font
 * @param[in] font_size			Font size in pixels
 * @param[in] matrix			Matrix for the text output. Can be NULL.
 * @param[in] glyph_index		Index of glyph to be output
 * @param[in] color				Color of the text, in 0xrrggbb format.
 *
 * @return	::FS_ERR_SUCCESS means output a glyph successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Bitmap_OutputGlyph(FS_BITMAP dib, FS_INT32 x, FS_INT32 y, FS_FONT font_handle, FS_FLOAT font_size,
								FS_MATRIX* matrix, FS_DWORD glyph_index, FS_ARGB color);

/**
 * @brief Output text string onto a DIB device.
 *
 * @param[in] dib				DIB handle, as the output device
 * @param[in] x					DIB x-coordination for the origin point of the first character.
 * @param[in] y					DIB y-coordination for the origin point of the first character.
 * @param[in] font_handle		Handle to the font
 * @param[in] font_size			Font size in pixels
 * @param[in] text_matrix		Matrix for the text output. Can be NULL.
 * @param[in] text				Zero-terminated unicode text string
 * @param[in] color				Color of the text, in 0xrrggbb format.
 *
 * @return	::FS_ERR_SUCCESS means output a text string successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Bitmap_OutputText(FS_BITMAP dib, FS_INT32 x, FS_INT32 y, FS_FONT font_handle, FS_FLOAT font_size,
							   FS_MATRIX* text_matrix, FS_LPCWSTR text, FS_ARGB color);

/**
 * @brief Use a system font mapper (typically for Chinese/Japanese/Korean charsets) from file.
 *
 * @details This function is used with devices that come with one or more system fonts,
 *			and those fonts are in standard TT or T1 format.
 *
 * @param[in] mapper			Pointer to ::FS_FONT_FILE_MAPPER structure.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Font_SetFontFileMapper(FS_FONT_FILE_MAPPER* mapper);

/**
 * @brief Use a system font mapper (typically for Chinese/Japanese/Korean charsets) from memory.
 *
 * @details This function is used with devices that come with one or more system fonts,
 *			and those fonts are in standard TT or T1 format.
 *
 * @param[in] mapper			Pointer to ::FS_FONT_DATA_MAPPER structure.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Font_SetFontDataMapper(FS_FONT_DATA_MAPPER* mapper);

/**
 * @brief Make use of a glyph provider: generating glyph bitmap for non-Latin characters
 *
 * @details PDF SDK embeds some standard fonts for Latin characters and symbols, like
 *			Times, Courier and Helvetica (Arial). For non-Latin characters, however,
 *			the sdk has to ask glyph provide for help.
 *
 *			If an embedded device carries fonts for non-Latin character sets, especially
 *			those for CJK markets, then the application can implement a glyph provider,
 *			allowing PDFs using non-embedded CJK fonts to be properly displayed.
 *
 * @param[in] provider			Pointer to the glyph provider structure.
 *								This structure must stay valid throughout usage of EMB SDK.
 *
 * @return 	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FS_Font_SetGlyphProvider(FS_GLYPH_PROVIDER* provider);

/**
 * @brief Make use of external data for EMB SDK
 *
 * @details Some embedded system has limitation on program size, therefore we might not be able to
 *			embed a lot of data (like cmap data and embedded font data) into the library. 
 *			We introduced this function so device can choose to store those data outside of program, 
 *			and make it available to use this function. The data has to be provided by Foxit.<br>
 *			This function is only available in library specially built for using external data.
 *
 * @param[in]	data			Pointer to the external data
 * @param[in]	size			Number of bytes in the external data.
 *
 * @return None.
 */
void FS_Font_UseExternalData(FS_LPCBYTE data, FS_DWORD size);

/**
 * @brief Load CMaps for Adobe-GB1 character collection. Make use of character encoding maps embedded with SDK.
 *
 * @details These functions add character encoding data to your application. Each call
 *			will increase the code size of your application. Total data size for all
 *			character sets is around 434K bytes.
 *
 * @return None.
 */
void FS_FontCMap_LoadGB();

/**
 * @brief Load CMaps for Adobe-GB1 character collection. Same as ::FS_FontCMap_LoadGB. 
 *
 * @details These functions add character encoding data to your application. Each call
 *			will increase the code size of your application. Total data size for all
 *			character sets is around 434K bytes.
 *
 * @return None.
 */
void FS_FontCMap_LoadGBExt();

/**
 * @brief Load CMaps for Adobe-CNS1 character collection.
 *
 * @details These functions add character encoding data to your application. Each call
 *			will increase the code size of your application. Total data size for all
 *			character sets is around 434K bytes.
 *
 * @return None.
 */
void FS_FontCMap_LoadCNS();

/**
 * @brief Load CMaps for Adobe-Korea1 character collection.
 *
 * @details These functions add character encoding data to your application. Each call
 *			will increase the code size of your application. Total data size for all
 *			character sets is around 434K bytes.
 *
 * @return None.
 */
void FS_FontCMap_LoadKorea();

/**
 * @brief Load CMaps for Adobe-Japan1 character collection.
 *
 * @details These functions add character encoding data to your application. Each call
 *			will increase the code size of your application. Total data size for all
 *			character sets is around 434K bytes.
 *
 * @return None.
 */
void FS_FontCMap_LoadJapan();

/**
 * @brief Load CMaps for Adobe-Japan1 character collection. Same as ::FS_FontCMap_LoadJapan.
 *
 * @details These functions add character encoding data to your application. Each call
 *			will increase the code size of your application. Total data size for all
 *			character sets is around 434K bytes.
 *
 * @return None.
 */
void FS_FontCMap_LoadJapanExt();

/**
 * @brief Get ready to use a standard PDF font
 *
 * @param[in] font_id			The ID of the font. See macro definitions <b>FPDF_FONT_XXX</b>.
 * @param[out] font_handle		Used to receive the font handle.
 *
 * @return	::FS_ERR_SUCCESS means open the standard font successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Font_OpenStandardFont(FS_INT32 font_id, FS_FONT* font_handle);

/**
 * @brief Load a font from a file
 *
 * @details EMB SDK only supports TrueType or Type1 font.
 *
 * @param[in] file				A pointer to a ::FS_FILEREAD structure.
 *								This structure must be kept valid as long as the font is open.
 * @param[out] font_handle		Used to receive the font handle.
 *
 * @return	::FS_ERR_SUCCESS means open a font from the file successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Font_OpenFileFont(FS_FILEREAD* file,  FS_FONT* font_handle);

/**
 * @brief Close a font handle.
 *
 * @param[in] font_handle		A font handle.
 *
 * @return	::FS_ERR_SUCCESS means close a font handle successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Font_CloseFont(FS_FONT font_handle);

/**
 * @brief Get the glyph index of a character
 *
 * @param[in] font_handle		A font handle.
 * @param[in] encoding			The font encoding. See macro definitions <b>FPDF_ENCODING_XXX</b>.
 * @param[in] char_code			The character code, depending on the used encoding. <br>For example, if <i>encoding</i>
 * 								is ::FPDF_ENCODING_UNICODE, then <i>char_code</i> should be unicode.
 * @param[out] glyph_index		Used to receive the result glyph index.
 *
 * @return	::FS_ERR_SUCCESS means get the glyph index successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Font_GetGlyphIndex(FS_FONT font_handle, FS_INT32 encoding, FS_DWORD char_code,
								FS_LPDWORD glyph_index);

/**
 * @brief Get the width of a glyph in a font
 *
 * @param[in] font_handle		A font handle.
 * @param[in] glyph_index		The index of the glyph in the font.
 * @param[out] width			Used to receive the character width, in 1/1000 of design size (em)
 *
 * @return	::FS_ERR_SUCCESS means get the width of a glyph successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FS_Font_GetGlyphWidth(FS_FONT font_handle, FS_DWORD glyph_index, FS_LPDWORD width);

/**
 * @brief Get the bitmap bearing of a glyph in a font
 *
 * @param[in] font_handle		A font handle.
 * @param[in] glyph_index		The index of the glyph in the font.
 * @param[in] matrix			The matrix for the glyph. This can be <b>NULL</b>.
 * @param[in] font_size			The font size in pixels.
 * @param[out] bitmap_left	    Used to receive the glyph bitmap's left bearing expressed in integer pixels.
 * @param[out] bitmap_top	    Used to receive the glyph bitmap's top bearing expressed in integer pixels.
 *
 * @return	::FS_ERR_SUCCESS means get the bitmap bearing of a glyph successfully.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
 FS_RESULT FS_Font_GetGlyphBitmapBearing(FS_FONT font_handle, FS_DWORD glyph_index, FS_MATRIX* matrix, 
										 FS_FLOAT font_size, FS_INT32* bitmap_left, FS_INT32* bitmap_top);

/**
 * @brief  Enable JBIG2 image decoder. For embedded platform only  and on desktop it's automatically loaded.
 *
 * @details If you want to display JBIG2 encoded images, you need to call 
 *			these functions after the SDK initialized.<br>
 *			Calling these functions will increase code size by about 200K-400K bytes.
 *
 * @return None.
 */
void FS_LoadJbig2Decoder();

/**
 * @brief Enable JBIG2000 image decoder. For embedded platform only  and on desktop it's automatically loaded.
 *
 * @details If you want to display JPEG2000 encoded images, you need to call 
 *			these functions after the SDK initialized.<br>
 *			Calling these functions will increase code size by about 200K-400K bytes.
 *			JPEG2000 decoder may not be available on some platforms.
 *
 * @return None.
 */
void FS_LoadJpeg2000Decoder();

#ifdef __cplusplus
};
#endif

#endif
// _FS_BASE_H_
/**@}*/


