/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit.
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement.
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FSEXCEPTION" module to be enabled in your SDK license.
  * @file	fs_exception.h
  * @author	Foxit Corporation
  * @brief	Header file for the Exception module.<br>
  *			The exception module offers methods to allow the user setting a function
  *			which is used to write logs and output logs during debugging.
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Exception module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FSEXCEPTION Exception 
 * @brief Definitions and Methods in this module are included in fs_exception.h .
 */
/**@{*/

#ifndef _FS_EXCEPTION_H_
#define _FS_EXCEPTION_H_

#ifndef _FS_BASE_H_
#include "fs_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _FS_DEF_EXPTCALLBACK_
#define _FS_DEF_EXPTCALLBACK_
/** @brief Structure for an exception callback. */
typedef struct  _FS_LOG_CALLBACK_
{
	/** @brief The size of the data structure. It must be set to <b>sizeof</b>(::FS_LOG_CALLBACK). */
	FS_DWORD	lStructSize; 
	
	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	* @brief A callback function used to output the log message.
	*
	* @param[in] msg		The msg to be logged.
	*
	* @return None.
	*/
	void		(*OutputLogMsg)(FS_LPCSTR msg);
	
}FS_LOG_CALLBACK;
#endif

/**
 * @brief Set a logging procedure to output debugging messages from the library.
 *
 * @param[in] callbackProc	An exception callback struct, including a callback function used for output the log message.
 *
 * @return None.
 */
void FS_Log_SetProc(FS_LOG_CALLBACK callbackProc);

#ifdef __cplusplus
};
#endif

#endif
// _FS_EXCEPTION_H_
/**@}*/
