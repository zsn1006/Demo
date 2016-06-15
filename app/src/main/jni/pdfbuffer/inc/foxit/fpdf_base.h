/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * @file	fpdf_base.h
  * @author	Foxit Corporation
  * @brief	Extended Header file for the Base Data module.
  *			The content of this header file is an extension of the base module.
  *			It contains macro definitions and structures used in Foxit PDF Layer.
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Base Data module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FSBASEDATA Base Data
 * @brief Definitions and Methods in this module are included in fs_base.h and fpdf_base.h .
 */
/**@{*/

#ifndef _FPDF_BASE_H_
#define _FPDF_BASE_H_

#ifndef _FS_BASE_H_
#include "fs_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//							Structures and Types						//
//////////////////////////////////////////////////////////////////////////
/** @name PDF Base Types*/
/**@{*/
/** @brief PDF document handle type. */
FS_DEFINEHANDLE(FPDF_DOCUMENT);

/** @brief PDF page handle type. */
FS_DEFINEHANDLE(FPDF_PAGE);

/** @brief Reflow page handle type. */
FS_DEFINEHANDLE(FPDF_REFLOWPAGE);

/** @brief Handler type for detailed text information in a page */
FS_DEFINEHANDLE(FPDF_TEXTPAGE);

/** @brief Search information handle type */
FS_DEFINEHANDLE(FPDF_SCHHANDLE);

/** @brief PDF page extracted link information handle type. */
FS_DEFINEHANDLE(FPDF_PAGELINK);

/** @brief Form handler type handle type */
FS_DEFINEHANDLE(FPDF_FORMENV);

/** @brief PDF action handle type. */
FS_DEFINEHANDLE(FPDF_ACTION);

/** @brief PDF bookmark handle type. */
FS_DEFINEHANDLE(FPDF_BOOKMARK);

/** @brief Annot information handler type */
FS_DEFINEHANDLE(FPDF_ANNOT);

/** @brief PDF inter form handle type. */
FS_DEFINEHANDLE(FPDF_FORM);

/** @brief PDF form field handle type. */
FS_DEFINEHANDLE(FPDF_FORMFIELD);

/** @brief PDF object handle type.*/
FS_DEFINEHANDLE(FPDF_OBJECT);

/** @brief Decrypt output handle type.  */
FS_DEFINEHANDLE(FPDF_DECRYPT_OUTPUT);

/** @brief Enveloper array handle type.*/
FS_DEFINEHANDLE(FPDF_ENVELOPES);

/** @brief PSI handle type. */
FS_DEFINEHANDLE(FPSI_HANDLE);

/** @brief Signature Field handle type. */
FS_DEFINEHANDLE(FPDF_SIGNATURE_FIELD);
/**@}*/

#ifndef _FS_DEF_URLDEST_
#define _FS_DEF_URLDEST_
/** @brief Structure for a URL destination. */
typedef struct _FPDF_URLDEST_
{
	FS_LPSTR				url;		/**< @brief URL encoded in 7-bit ASCII. */
}FPDF_URLDEST;
#endif

#ifndef _FS_DEF_DEST_
#define _FS_DEF_DEST_
/** @brief Structure for a page destination. */
typedef struct _FPDF_DEST_
{
	FS_INT32				page_index;		/**< @brief Zero based index for the page. */
	FS_INT32				zoom_mode;		/**< @brief See macro definitions <b>FPDF_ZOOM_xxx</b>.*/
	FS_FLOAT				zoom_factor;	/**< @brief For FPDF_ZOOM_FACTOR only: the zoom factor (in percentage). */
	/**
	 * @brief A rectangle used to specify the position inside the page. 
	 * @details According to the zoom mode, different members of the rectangle are used:<br>
	 * <ul>
	 * <li>FPDF_ZOOM_NONE:			left, top</li>
	 * <li>FPDF_ZOOM_FACTOR:		left, top</li>
	 * <li>FPDF_ZOOM_FITPAGE:		none</li>
	 * <li>FPDF_ZOOM_FITWIDTH:		top</li>
	 * <li>FPDF_ZOOM_FITHEIGHT:		left</li>
	 * <li>FPDF_ZOOM_FITRECT:		left, top, bottom, right</li>
	 * <li>FPDF_ZOOM_FITCONTENT:	none</li>
	 * <li>FPDF_ZOOM_FITCONTENTW:	top</li>
	 * <li>FPDF_ZOOM_FITCONTENTH:	left</li>
	 * </ul>
	 */
	FS_RECTF				position;
}FPDF_DEST;
#endif

#ifndef _FS_DEF_DOCDEST_
#define _FS_DEF_DOCDEST_
/** @brief Structure for a document destination. */
typedef struct _FPDF_DOCDEST_
{
	FPDF_DEST				page_data;	/**< @brief The page data. */
	FS_LPWSTR				file_name;	/**< @brief The file name, encoded in original charset (maybe MBCS). */
	FS_DWORD				length;		/**< @brief The length of <i>file_name</i>. */
}FPDF_DOCDEST;
#endif

#ifndef _FS_DEF_LAUNCHACTION_
#define _FS_DEF_LAUNCHACTION_
/** @brief Structure for a launch action. */
typedef struct _FPDF_LAUNCHACTION_
{
	FS_INT32				new_window;	/**< @brief Whether a new window should be opened. */
	FS_CHAR*				file_name;	/**< @brief The file name, encoded in original charset (maybe MBCS). */
}FPDF_LAUNCHACTION;
#endif

#ifndef _FS_DEF_ANNOTNOTEINFO_
#define _FS_DEF_ANNOTNOTEINFO_
/** @brief Structure for a note annotation. */
typedef struct _FPDF_ANNOT_NOTEINFO_
{
	FS_DWORD				size;			/**< @brief The size of the structure. */
	FS_WCHAR				author[64];		/**< @brief The author of the annotation. */
	FS_ARGB					color;			/**< @brief The color of the annotation. */
	FS_INT32				opacity;		/**< @brief The opacity of the annotation. */
	FS_INT32				icon;			/**< @brief This is reserved and must be 0. */
	FS_RECTF				rect;			/**< @brief The rectangle of the annotation. */
	FS_LPCWSTR				contents;		/**< @brief Contents of the annotation. */
}FPDF_ANNOT_NOTEINFO;  
#endif
   
#ifndef _FS_DEF_ANNOTQUAD_
#define _FS_DEF_ANNOTQUAD_
/** 
 * @brief Structure for a quadrilateral of annotation.
 * @note The text is oriented with the respect to edge connecting points (x1, y1) and (x2, y2).
 */
typedef struct _FPDF_ANNOT_QUAD_
{	
	FS_FLOAT				x1;				/**< @brief The value of x-coordinate of the first vertex. */
	FS_FLOAT				y1;				/**< @brief The value of y-coordinate of the first vertex. */
	FS_FLOAT				x2;				/**< @brief The value of x-coordinate of the second vertex. */
	FS_FLOAT				y2;				/**< @brief The value of y-coordinate of the second vertex. */
	FS_FLOAT				x3;				/**< @brief The value of x-coordinate of the third vertex. */
	FS_FLOAT				y3;				/**< @brief The value of y-coordinate of the third vertex. */
	FS_FLOAT				x4;				/**< @brief The value of x-coordinate of the fourth vertex. */
	FS_FLOAT				y4;				/**< @brief The value of y-coordinate of the fourth vertex. */
}FPDF_ANNOT_QUAD;
#endif

#ifndef _FS_DEF_ANNOTHIGHLIGHTINFO_
#define _FS_DEF_ANNOTHIGHLIGHTINFO_
/** @brief Structure for a highlight annotation. */
typedef struct _FPDF_ANNOT_HIGHLIGHTINFO_
{
	FS_DWORD				size;			/**< @brief The size of the structure. */
	FS_WCHAR				author[64];		/**< @brief The author of the annotation. */
	FS_ARGB					color;			/**< @brief The color of the annotation. */
	FS_INT32				opacity;		/**< @brief The opacity of the annotation, from 0 to 100 */
	FS_INT32				quad_count;		/**< @brief The quadrilateral count of the annotation. */
	FPDF_ANNOT_QUAD*		quads;			/**< @brief Quadrilaterals of the annotation. */
}FPDF_ANNOT_HIGHLIGHTINFO;
#endif

#ifndef _FS_DEF_ANNOTLINE_
#define _FS_DEF_ANNOTLINE_
/** @brief Structure for a pencil line. */
typedef struct _FPDF_ANNOT_LINE_
{
	FS_INT32				point_count;	/**< @brief The point count of the line. */
	FS_POINTF*				points;			/**< @brief Points of the line in the annotation. */
}FPDF_ANNOT_LINE;
#endif

#ifndef _FS_DEF_ANNOTPENCILINFO_
#define _FS_DEF_ANNOTPENCILINFO_
/** @brief Structure for a pencil annotation. */
typedef struct _FPDF_ANNOT_PENCILINFO_
{
	FS_DWORD				size;			/**< @brief The size of the structure. */
	FS_WCHAR				author[64];		/**< @brief The author of the annotation. */
	FS_ARGB					color;			/**< @brief The color of the annotation. */
	FS_INT32				opacity;		/**< @brief The opacity of the annotation. */
	FS_INT32				busebezier;		/**< @brief Whether to use Bezier curve to connect two points. */
	FS_BOOL					boptimize;		/**< @brief Whether to optimize to decrease points on the line or not. */
	FS_INT32				reserved;		/**< @brief This is reserved, and it must be 0. */
	FS_INT32				line_width;		/**< @brief The line width of the annotation. */
	FS_INT32				line_count;		/**< @brief The line count of the annotation. */
	FPDF_ANNOT_LINE*		lines;			/**< @brief Lines of the annotation. */
}FPDF_ANNOT_PENCILINFO;
#endif

#ifndef _FS_DEF_ANNOTSTAMPINFO_
#define _FS_DEF_ANNOTSTAMPINFO_
/** @brief Structure for a stamp annotation. */
typedef struct _FPDF_ANNOT_STAMPINFO_
{
	FS_DWORD				size;			/**< @brief The size of the structure. */
	FS_WCHAR				author[64];		/**< @brief The author of the annotation. */
	FS_ARGB					color;			/**< @brief The color of the annotation. */
	FS_INT32				opacity;		/**< @brief The opacity of the annotation. */
	FS_RECTF				rect;			/**< @brief The rectangle of the annotation. */
	FS_WCHAR				name[32];		/**< @brief The subject of the annotation. */
	FS_INT32				imgtype;		/**< @brief The image type of the image data. See macro definitions <b>FPDF_IMAGETYPE_XXX</b>*/
	FS_INT32				imgdatasize;	/**< @brief The size of the image data buffer. */
	FS_LPVOID				imgdata;		/**< @brief The image data buffer. */
}FPDF_ANNOT_STAMPINFO;
#endif

#ifndef _FS_DEF_ANNOTFILEATTACHMENTINFO_
#define _FS_DEF_ANNOTFILEATTACHMENTINFO_
/** @brief Structure for a file attachment annotation. */
typedef struct _FPDF_ANNOT_FILEATTACHMENTINFO_
{
	FS_DWORD				size;			/**< @brief The size of the structure. */
	FS_WCHAR				author[64];		/**< @brief The author of the annotation. */
	FS_ARGB					color;			/**< @brief The color of the annotation. */
	FS_INT32				opacity;		/**< @brief The opacity of the annotation. */
	FS_INT32				icon;			/**< @brief This is reserved and must be 0 */
	FS_RECTF				rect;			/**< @brief The rectangle of the annotation. */
	FS_WCHAR				filename[256];	/**< @brief The file name. */
	FS_INT32				file_size;		/**< @brief The size of the file data buffer. */
	FS_LPVOID				file_data;		/**< @brief The file data buffer. */
}FPDF_ANNOT_FILEATTACHMENTINFO;
#endif

#ifndef _FS_DEF_ANNOTLINKINFO_
#define _FS_DEF_ANNOTLINKINFO_
/** @brief Structure for a link annotation. */
typedef struct _FPDF_ANNOT_LINKINFO_
{
	FS_DWORD				size;			/**< @brief The size of the structure. */
	FS_WCHAR				author[64];		/**< @brief The author of the annotation. */
	FS_ARGB					color;			/**< @brief The color of the annotation. */
	FS_INT32				opacity;		/**< @brief The opacity of the annotation. */
	FS_RECTF				rect;			/**< @brief The rectangle of the annotation. */
	FPDF_ANNOT_QUAD			quad;			/**< @brief Quadrilaterals of the annotation. */
	FS_INT32				line_width;		/**< @brief The line width of the annotation. */
	FS_INT32				dest_type;		/**< @brief The destination type. See macro definitions <b>FPDF_DEST_XXX</b>. */
	FS_LPVOID				data;			/**< @brief The destination data, depend on <i>dest_type</i>. */
}FPDF_ANNOT_LINKINFO;
#endif

#ifndef _FS_DEF_UNSUPPORTINFO_
#define _FS_DEF_UNSUPPORTINFO_
/** @brief Structure for the unsupport object information. */
typedef	struct _FPDF_UNSUPPORT_INFO_
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FPDF_UNSUPPORT_INFO). */
	FS_DWORD lStructSize;

	/**< @brief A user pointer, used by the application. */
	FS_LPVOID clientData;

	/** 
	 * @brief	UnSupport object process handling function.
	 *
	 * @param[in] clientData	A pointer to the interface structure itself.
	 * @param[in] type			The type of the unsupport object. See macro definitions <b>FPDF_UNSP_XXX</b>.
	 *
	 * @return	None.
	 */
	void (*UnsupportHandler)(FS_LPVOID clientData, FS_INT32 type);

}FPDF_UNSUPPORT_INFO;
#endif


//////////////////////////////////////////////////////////////////////////
//							Macro Definitions							//
//////////////////////////////////////////////////////////////////////////

/** @name Macro Definition for Reflow Module ID */
/**@{*/
/** @brief Reflow module ID. */
#define REFLOW_MODULE (FS_LPVOID)1
/**@}*/

/**
 * @name Macro Definitions for Annotation Types
 */
/**@{*/
/** @brief No or unsupported annotation. */
#define FPDF_ANNOTTYPE_UNKNOWN				0
/** @brief Note annotation. */
#define FPDF_ANNOTTYPE_NOTE					1
/** @brief Highlight annotation. */
#define FPDF_ANNOTTYPE_HIGHLIGHT			2
/** @brief Pencil annotation. */
#define FPDF_ANNOTTYPE_PENCIL				3
/** @brief Stamp annotation. */				
#define FPDF_ANNOTTYPE_STAMP				4
/** @brief File attachment annotation. */				
#define	FPDF_ANNOTTYPE_FILEATTACHMENT		5
/** @brief Link annotation. */
#define FPDF_ANNOTTYPE_LINK					6
/**@}*/

/**
 * @name Macro Definitions for Supported Information Flags of Annotation.
 */
/**@{*/
/** @brief The type of annotation. */
#define FPDF_ANNOTINFO_TYPE				1
/** @brief The author of annotation. */
#define FPDF_ANNOTINFO_AUTHOR			2
/** @brief The rectangle of annotation. */
#define FPDF_ANNOTINFO_RECT				3
/** @brief The contents of note annotation. */
#define FPDF_ANNOTINFO_CONTENTS			4
/** @brief The color of annotation. */
#define FPDF_ANNOTINFO_COLOR			5
/** @brief The opacity of annotation. */
#define FPDF_ANNOTINFO_OPACITY			6
/** @brief The line width of pencil annotation. */
#define FPDF_ANNOTINFO_LINEWIDTH		7
/**@}*/

/**
 * @name Macro Definitions for Image Types
 */
/**@{*/
/** @brief The image type is BMP. */
#define FPDF_IMAGETYPE_BMP			1
/** @brief The image type is JPG. */
#define FPDF_IMAGETYPE_JPG			2
/** @brief The image type is PNG. */
#define FPDF_IMAGETYPE_PNG			3
/**@}*/

/**
 * @name Macro Definitions for Saving Flags
 * @brief These are used for saving document.
 */
/*@{*/
/** @brief Save the document incrementally. */
#define FPDF_SAVEAS_INCREMENTAL		1
/** @brief Save the document without the original one. */
#define FPDF_SAVEAS_NOORIGINAL		2
/**@}*/

/**
 * @name PDF document permissions, according to PDF Reference, Table 3.20.
 * 1-2 bits must be zero, 7-8, 13-32 bits must be 1. 
 */
/**@{*/
/**
 * @brief bit 3.
 * - (Revision 2) Print the document.
 * - (Revision 3 or greater) Print the document (possibly not at the highest
 * quality level, depending on whether bit 12 is also set). 
 */
#define FPDF_PERMISSION_PRINT			0x0004
/**
 * @brief bit 4. Modify the contents of the document by operations other than 
 * those controlled by bits 6, 9, and 11.
 */
#define FPDF_PERMISSION_MODIFY			0x0008
/**
 * @brief bit 5.
 * - (Revision 2) Copy or otherwise extract text and graphics from the 
 * document, including extracting text and graphics (in support of accessibility
 * to users with disabilities or for other purposes).
 * - (Revision 3 or greater) Copy or otherwise extract text and graphics 
 * from the document by operations other than that controlled by bit 10.
 */
#define FPDF_PERMISSION_EXTRACT			0x0010
/**
 * @brief bit 6. Add or modify text annotations, fill in interactive form fields, and, 
 * if bit 4 is also set, create or modify interactive form fields (including signature fields).
 */
#define FPDF_PERMISSION_ANNOTFORM		0x0020
/**
 * @brief bit 9. (Revision 3 or greater) Fill in existing interactive form fields (including signature fields),
 * even if bit 6 is clear. 
 */
#define FPDF_PERMISSION_FILLFORM		0x0100
/**
 * @brief bit 10. (Revision 3 or greater) Extract text and graphics (in support of accessibility
 * to users with disabilities or for other purposes). 
 */
#define FPDF_PERMISSION_EXTRACTACCESS	0x0200
/**
 * @brief bit 11. (Revision 3 or greater) Assemble the document (insert, rotate, or delete pages
 * and create bookmarks or thumbnail images), even if bit 4 is clear. 
 */
#define FPDF_PERMISSION_ASSEMBLE		0x0400
/**
 * @brief bit 12. (Revision 3 or greater) Print the document to a representation from 
 * which a faithful digital copy of the PDF content could be generated. 
 * When this bit is clear (and bit 3 is set), printing is limited to a low-level
 * representation of the appearance, possibly of degraded quality. 
 * (See implementation note 25 in Appendix H.) 
 */
#define FPDF_PERMISSION_PRINT_HIGH		0x0800
/**@}*/

/**
 * @name Macro definitions for Parser Flags
 */
/**@{*/
/** @brief Whether to parse images. */
#define FPDF_PARSER_IMAGE		0x1 
/** @brief Whether to slice images or texts for the page. */ 
#define FPDF_PARSER_PAGEMODE	0x4 
/**@}*/

/** 
 * @name Macro Definitions for Text Direction Flags
 * @see ::FPDF_Text_GetCharIndexByDirection
 */
/**@{*/
/** @brief Direction: left */
#define FPDF_TEXT_LEFT	-1
/** @brief Direction: right */
#define FPDF_TEXT_RIGHT	 1
/** @brief Direction: up */
#define FPDF_TEXT_UP	-2
/** @brief Direction: down */
#define FPDF_TEXT_DOWN	 2
/**@}*/

/**
 * @name Macro Definitions for Standard Font ID
 */
/**@{*/
/** @brief Courier. */
#define FPDF_FONT_COURIER			0
/** @brief Bold. Courier-Bold. */
#define FPDF_FONT_COURIER_B			1
/** @brief Bold italic. Courier-BoldOblique. */
#define FPDF_FONT_COURIER_BI		2
/** @brief Italic. Courier-Oblique. */
#define FPDF_FONT_COURIER_I			3
/** @brief Helvetica. */
#define FPDF_FONT_HELVETICA			4
/** @brief Bold. Helvetica-Bold. */
#define FPDF_FONT_HELVETICA_B		5
/** @brief Bold italic. Helvetica-BoldOblique. */
#define FPDF_FONT_HELVETICA_BI		6
/** @brief Italic. Helvetica-Oblique. */
#define FPDF_FONT_HELVETICA_I		7
/** @brief Times-Roman. */
#define FPDF_FONT_TIMES				8
/** @brief Bold. Times-Bold. */
#define FPDF_FONT_TIMES_B			9
/** @brief Bold italic. Times-BoldItalic. */
#define FPDF_FONT_TIMES_BI			10
/** @brief Italic. Times-Italic. */
#define FPDF_FONT_TIMES_I			11
/** @brief Symbol. */
#define FPDF_FONT_SYMBOL			12
/** @brief ZapfDingbats. */
#define FPDF_FONT_ZAPF_DINGBATS		13
/**@}*/

/**
 * @name Macro Definitions for Font Encodings
 */
/**@{*/
/** @brief Whatever internal encoding in the font. */
#define FPDF_ENCODING_INTERNAL		0
/** @brief Unicode encoding. */
#define FPDF_ENCODING_UNICODE		1
/**@}*/

/**
 * @name Macro Definitions for Render Flags.
 */
/**@{*/
/** @brief Set if annotations are to be rendered. */
#define FPDF_RENDER_ANNOT			0x01
/** @brief Set if using text rendering optimized for LCD display. */
#define FPDF_RENDER_LCD_TEXT		0x02
/** @brief Set if the device is using BGR LCD stripe. */
#define FPDF_RENDER_BGR_STRIPE		0x04
/** @brief Drop the page objects that are rendered. This will make the most complicated pages renderable
	within very limited memory. <b>HOWEVER</b>, after rendering the page will not be useable anymore! You will
	have to close the page and start it over! */
#define FPDF_RENDER_DROP_OBJECTS	0x08
/** @brief Set if the form fields(widgets) are not to be rendered.*/
#define FPDF_RENDER_NOWIDGET		0x10
/** @brief Grayscale output. Only used in ::FPDF_FormFill_Draw. */
#define FPDF_RENDER_GRAYSCALE		0x100		
/**@}*/ 

/**
 * @name Macro Definitions for Action Types
 */
/**@{*/
/** @brief No or unsupported destination. */
#define FPDF_DEST_NONE			0
/** @brief A page inside the same document. */
#define FPDF_DEST_PAGE			1
/** @brief An external PDF document. */
#define FPDF_DEST_DOC			2
/** @brief An external URL. */
#define FPDF_DEST_URL			3
/** @brief Launch an external file or command. */
#define FPDF_ACTION_LAUNCH		4
/**@}*/

/**
 * @name Macros Definition for Field Types.
 */
/**@{*/
/** @brief Unknown. */
#define FPDF_FIELDTYPE_UNKNOWN		0
/** @brief Push button. */
#define FPDF_FIELDTYPE_PUSHBUTTON	1
/** @brief Check box. */
#define FPDF_FIELDTYPE_CHECKBOX		2
/** @brief Radio button. */
#define FPDF_FIELDTYPE_RADIOBUTTON	3		
/** @brief Combo box. */
#define FPDF_FIELDTYPE_COMBOBOX		4	
/** @brief List box. */
#define FPDF_FIELDTYPE_LISTBOX		5		
/** @brief Text field. */
#define FPDF_FIELDTYPE_TEXTFIELD	6			
/** @brief Signature. */
#define FPDF_FIELDTYPE_SIGNATURE	7
/**@}*/

/**
 * @name Macro Definitions for Form Field Flags.
 */
/**@{*/
/** @brief The field is read only and no editing is allowed. */
#define FPDF_FIELDFLAG_READONLY		0x01
/** @brief The field will be exported by a submit-form action. */
#define FPDF_FIELDFLAG_REQUIRED		0x02
/** @brief The field must not be exported by a submit-form action. */
#define FPDF_FIELDFLAG_NOEXPORT		0x04
/**@}*/

/**
 * @name Macro Definitions for Search Flags 
 * @see ::FPDF_Text_FindStart
 */
/**@{*/
/** @brief Whether matching case. */
#define FPDF_MATCHCASE			1
/** @brief Whether matching whole word. */
#define FPDF_MATCHWHOLEWORD		2
/** @brief (This is not supported now. Maybe this will be supported in a later release.)Whether matching consecutively (for example, "CC" will match twice in "CCC").*/
#define FPDF_CONSECUTIVE		4
/**@}*/

/**
 * @name Macro Definitions for Zoom Mode for Destination.
 */
/**@{*/
/** @brief The zoom mode is not specified. */
#define FPDF_ZOOM_NONE				0
/** @brief A specific zoom factor is used. */
#define FPDF_ZOOM_FACTOR		1
/** @brief Fit the whole page on the screen. */
#define FPDF_ZOOM_FITPAGE			2
/** @brief Fit the width of the page on the screen. */
#define FPDF_ZOOM_FITWIDTH			3
/** @brief Fit the height of the page on the screen. */
#define FPDF_ZOOM_FITHEIGHT			4
/** @brief Fit a particular rectangle on the screen. */
#define FPDF_ZOOM_FITRECT			5
/** @brief Fit the whole content of page on the screen. */
#define FPDF_ZOOM_FITCONTENT		6
/** @brief Fit the content width of page on the screen. */
#define FPDF_ZOOM_FITCONTENTW		7
/** @brief Fit the content height of page on the screen. */
#define FPDF_ZOOM_FITCONTENTH		8
/**@}*/

/**
 * @name Macro Definitions for Types of Unsupport Object.
 */
/**@{*/
/** @brief Document: XFA form. */
#define FPDF_UNSP_DOC_XFAFORM				1
/** @brief Document: portable collection. */
#define FPDF_UNSP_DOC_PORTABLECOLLECTION	2
/** @brief Document: attachment. */
#define FPDF_UNSP_DOC_ATTACHMENT			3
/** @brief Document: security. */
#define FPDF_UNSP_DOC_SECURITY				4
/** @brief Document: shared review. */
#define FPDF_UNSP_DOC_SHAREDREVIEW			5
/** @brief Document: shared form of Acrobat. */
#define FPDF_UNSP_DOC_SHAREDFORM_ACROBAT	6
/** @brief Document: shared form of file system. */
#define FPDF_UNSP_DOC_SHAREDFORM_FILESYSTEM	7
/** @brief Document: shared form of email. */
#define FPDF_UNSP_DOC_SHAREDFORM_EMAIL		8
/** @brief Annotation: 3D annotation. */
#define FPDF_UNSP_ANNOT_3DANNOT				11
/** @brief Annotation: movie. */
#define FPDF_UNSP_ANNOT_MOVIE				12
/** @brief Annotation: sound. */
#define FPDF_UNSP_ANNOT_SOUND				13
/** @brief Annotation: screen media. */
#define FPDF_UNSP_ANNOT_SCREEN_MEDIA		14
/** @brief Annotation: screen rich media. */
#define FPDF_UNSP_ANNOT_SCREEN_RICHMEDIA	15
/** @brief Annotation: attachment. */
#define FPDF_UNSP_ANNOT_ATTACHMENT			16
/** @brief Annotation: signature. */
#define FPDF_UNSP_ANNOT_SIG					17
/**@}*/

/**
 * @name Macro Definitions for Page Mode. 
 */
/**@{*/
/** @brief Unknown value. */
#define FPDF_PAGEMODE_UNKNOWN			-1
/** @brief Neither document outline nor thumbnail image is visible. */
#define FPDF_PAGEMODE_USENONE			0
/** @brief Document outline is visible. */
#define FPDF_PAGEMODE_USEOUTLINES		1
/** @brief Thumbnial images are visible. */
#define FPDF_PAGEMODE_USETHUMBS			2
/** @brief Full-screen mode: no menu bar and window controls, and any other window is invisible. */
#define FPDF_PAGEMODE_FULLSCREEN		3
/** @brief The optional content group panel is visible. */
#define FPDF_PAGEMODE_USEOC				4
/** @brief Attachments panel is visible. */
#define FPDF_PAGEMODE_USEATTACHMENTS	5
/**@}*/

/**
 * @name Macro Definitions for PDF object types.
 */
/**@{*/
/** @brief invalid object. */
#define FPDF_OBJECT_INVALID			0
/** @brief Boolean value. */
#define	FPDF_OBJECT_BOOLEAN			1
/** @brief Integer and real number. */
#define FPDF_OBJECT_NUMBER			2
/** @brief String. */
#define FPDF_OBJECT_STRING			3
/** @brief Name. */
#define FPDF_OBJECT_NAME			4
/** @brief Array. */
#define FPDF_OBJECT_ARRAY			5
/** @brief Dictionary. */
#define FPDF_OBJECT_DICTIONARY		6
/** @brief Stream. */
#define FPDF_OBJECT_STREAM			7
/** @brief the null object. */
#define FPDF_OBJECT_NULL			8
/**@}*/

/** 
* @name Macro Definitions for PDF Signature State
*/
/**@{*/
/** @brief unknown signature. */
#define FPDF_SIGNATURE_STATE_UNKNOWN		0
/** @brief unsigned signature. */
#define FPDF_SIGNATURE_STATE_UNSIGNED		1
/** @brief valid signature. */
#define FPDF_SIGNATURE_STATE_VALID			2
/** @brief invalid signature. */	
#define FPDF_SIGNATURE_STATE_INVALID		3
/**@}*/	

/** 
* @name Macro Definitions for Signature Show Flag
*/
/**@{*/
/** @brief Show text on signature appearance. */
#define FPDF_SIGNATURE_AP_SHOW_TEXT				0x100
/** @brief Show image on signature appearance. */
#define FPDF_SIGNATURE_AP_SHOW_IMAGE			0x080
/** @brief Show signer name on description. */
#define FPDF_SIGNATURE_AP_SHOW_NAME				0x040
/** @brief Show location on description. */
#define FPDF_SIGNATURE_AP_SHOW_LOCATION			0x020
/** @brief Show dn on description. */
#define FPDF_SIGNATURE_AP_SHOW_DN				0x010
/** @brief Show date on description. */
#define FPDF_SIGNATURE_AP_SHOW_DATE				0x008
/** @brief Show reason on description. */
#define FPDF_SIGNATURE_AP_SHOW_REASON			0x004
/** @brief Show label on description. */
#define FPDF_SIGNATURE_AP_SHOW_LABEL			0x002
/** @brief Show Foxit flag on signature appearance. */
#define FPDF_SIGNATURE_AP_SHOW_FOXITFLAG		0x001
/**@}*/	

#ifdef __cplusplus
};
#endif

#endif
// _FPDF_BASE_H_
/**@}*/


