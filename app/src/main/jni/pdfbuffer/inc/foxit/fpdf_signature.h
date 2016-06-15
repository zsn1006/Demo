/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPDFSIGNATURE" module to be enabled in your SDK license.
  * @file	fpdf_signature.h
  * @author	Foxit Corporation
  * @brief	Header file for the Signature module.<br>
  *			The module offers:<br>
  *			1. Methods for add unsigned signatures on pages.<br>
  *			2. Methods for set the appearance of signatures.<br>
  *			3. Methods for sign and verify signatures with 3rd filters.<br>
  *			4. Methods for remove signatures.<br>
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Signature module explicitly. 
  * @version 2.0
  */

/**
 * @addtogroup FPDFSIGNATURE Signature
 * @brief Methods in this module are included in fpdf_signature.h
 */
/**@{*/

#ifndef _FPDF_SIGNATURE_H_
#define _FPDF_SIGNATURE_H_

#ifndef _FS_BASE_H_
#include "fs_base.h"
#endif

#ifndef _FPDF_BASE_H_
#include "fpdf_base.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _FS_DEF_SIGNATUREHANDLER_
#define _FS_DEF_SIGNATUREHANDLER_
/**
 * @brief Structure for interfaces implemented by host application. 
 */
typedef struct _FPDF_SIGNATURE_HANDLER_
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FPDF_PKI_SECURITY_HANDLER). */
	FS_DWORD	lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;	

	/**
	 * @brief Sign the document with custom handler.
	 * 
	 * @param[in] clientData			Pointer to the "user" data.
	 * @param[in] sigField				Pointer to the signature to be signed.
	 * @param[in] srcbuffer				Pointer to the source content to be signed.
	 * @param[in] dwSize				Size of the source buffer.
	 * @param[out] signedbuffer			Pass the signed buffer into SDK.
	 * @param[out] dwSignedSize			Pass the buffer size into SDK.
	 *
	 * @return	::FS_ERR_SUCCESS means success.<br>
	 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
	 *
	 * @note Host application provides this callback method for custom sign mechanism.		
	 */
	FS_RESULT	(*SignDocument)(FS_LPVOID clientData, FPDF_SIGNATURE_FIELD* sigField, FS_LPBYTE srcbuffer, 
							    FS_DWORD dwSize, FS_LPBYTE* signedbuffer, FS_LPDWORD dwSignedSize);
	
	/**
	 * @brief Verify the specified signature.
	 * 
	 * @param[in] clientData			Pointer to the "user" data.
	 * @param[in] sigField				Pointer to the signature to be verified.
	 * @param[in] srcbuffer				Pointer to the the content to be verified.
	 * @param[in] dwSize				Size of the source content.
	 * @param[in] signedbuffer			Pointer to the signed value.
	 * @param[in] dwSignedSize			Size of the signed value.
	 * @param[out] state				Pass the verified state of signature into SDK.
	 *
	 * @return	::FS_ERR_SUCCESS means success.<br>
	 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
	 *
	 * @note Host application provides this method for custom verify mechanism.
	 */
	FS_RESULT	(*VerifySignature)(FS_LPVOID clientData, FPDF_SIGNATURE_FIELD* sigField, FS_LPBYTE srcbuffer, 
								   FS_DWORD dwSize, FS_LPBYTE signedbuffer, FS_DWORD dwSignedSize, FS_INT32* state);

	/** @brief The filter of custom signature mechanism. */
	FS_WCHAR	filter[128];

	/** @brief The subfilter of the custom signature mechanism. */
	FS_WCHAR	subfilter[128];

} FPDF_SIGNATURE_HANDLER;
#endif

/**
 * @brief Add a signature field with a blank appearance at the specified page.
 *	
 * @param[in] doc					Pointer to the document.
 * @param[in] page					The page to be placed the signature.
 * @param[in] rect					The rect to be placed the signature. For page system.
 * @param[out] sigField				Pointer to receive the added signature field handle.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_Add(FPDF_DOCUMENT doc, FPDF_PAGE page, FS_RECTF rect, FPDF_SIGNATURE_FIELD* sigField);

/**
 * @brief Set the signer name.
 * 
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] signer				Pointer to the signer name.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_SetSigner(FPDF_SIGNATURE_FIELD sigField, FS_LPCWSTR signer);

/**
 * @brief Set the signer distinguished name.
 * 
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] dn					Pointer to the distinguished name of signer.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_SetDN(FPDF_SIGNATURE_FIELD sigField, FS_LPCWSTR dn);
	 
/**
 * @brief Set the signing date.
 * 
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] date					Pointer to receive the signing date.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_SetDate(FPDF_SIGNATURE_FIELD sigField, FS_LPCWSTR date);

/**
 * @brief Set the text to be showed on signature appearance.
 *
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] text					Pointer to the appearance text.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_SetText(FPDF_SIGNATURE_FIELD sigField, FS_LPCWSTR text);

/**
 * @brief Set the signing reason.
 * 
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] reason				Pointer to signing reason.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_SetReason(FPDF_SIGNATURE_FIELD sigField, FS_LPCWSTR reason);

/**
 * @brief Set the signing location.
 * 
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] location				Pointer to the signing location.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_SetLocation(FPDF_SIGNATURE_FIELD sigField, FS_LPCWSTR location);

/**
 * @brief Set the appearance options to optionally show the signature appearance.
 *
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] dwFlag				The appearance options. See macro definitions <b>FPDF_SIGNATURE_AP_SHOW_XXX</b> for more information.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */ 
FS_RESULT FPDF_Signature_SetOption(FPDF_SIGNATURE_FIELD sigField, FS_DWORD dwFlag);

/**
 * @brief Set the using image type. See the definition in fpdf_base.h.
 *
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] nType				    The input image type. See macro definitions <b>FPDF_IMAGETYPE_XXX</b> for more information.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_SetImageType(FPDF_SIGNATURE_FIELD sigField, FS_INT32 nType);

/**
 * @brief Set the using image data.
 * 
 * @param[in] sigField				Pointer to the signature field.
 * @param[in] dwSize				The input image data size.
 * @param[in] data					Pointer to the input image data.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_SetImageData(FPDF_SIGNATURE_FIELD sigField, FS_DWORD dwSize, FS_LPBYTE data);

/**
 * @brief Count all signatures. 
 * @param[in] doc					Document handler.
 * @param[out] count				Pointer to receive the signatures' count.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT	FPDF_Signature_GetCount(FPDF_DOCUMENT doc, FS_INT32* count);

/**
 * @brief Get the specified signature.
 * @param[in] doc					Document handler.
 * @param[in] index					The specified index. Start from 0.
 * @param[out] sigField				Receive the specified signature.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT	FPDF_Signature_Get(FPDF_DOCUMENT doc, FS_INT32 index, FPDF_SIGNATURE_FIELD* sigField);

/**
 * @brief Sign the document by the custom way
 *
 * @param[in] doc					Pointer to the document.
 * @param[in] sigField				The signature to be signed.
 * @param[in] sigHandler			Pointer to the custom signature handler.
 * @param[in] fileWrite				Pointer to a ::FS_FILEWRITE struct.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT  FPDF_Signature_Sign(FPDF_DOCUMENT doc, FPDF_SIGNATURE_FIELD sigField, 
							   FPDF_SIGNATURE_HANDLER* sigHandler, FS_FILEWRITE* fileWrite);

/**
 * @brief Verify a signature by custom way.
 *
 * @param[in] doc					Pointer to the document.
 * @param[in] sigField				The signature to be verified.
 * @param[in] sigHandler			Pointer to a custom signature handler.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_Verify(FPDF_DOCUMENT doc, FPDF_SIGNATURE_FIELD sigField, FPDF_SIGNATURE_HANDLER* sigHandler);

/**
 * @brief Check whether the signature is signed.
 *
 * @param[in] doc					Pointer to the document.
 * @param[in] sigField				Pointer to the signature.
 * @param[out] pbSigned				Pointer to receive value indicates whether the signature is signed.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_IsSigned(FPDF_DOCUMENT doc, FPDF_SIGNATURE_FIELD sigField, FS_BOOL* pbSigned);

/**
 * @brief Clear the signed signature data.
 * 
 * @param[in] doc					Pointer to the document.
 * @param[in] sigField				Pointer to the signature to be cleared.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 * @note This method can be called to clear the signed signature data, includes its appearance and the signed data
 *		and then leave the blank signature on the page.
 */
FS_RESULT FPDF_Signature_Clear(FPDF_DOCUMENT doc, FPDF_SIGNATURE_FIELD sigField);

/**
 * @brief Remove an unsigned signature.
 * 
 * @param[in] doc					Pointer to the document.
 * @param[in] sigField				Pointer to the unsigned signature to be removed.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_Remove(FPDF_DOCUMENT doc, FPDF_SIGNATURE_FIELD sigField);

/**
 * @brief Delete the signature field handle.
 *
 * @param[in] doc					Pointer to the document.
 * @param[in] sigField				Pointer to the signature field.
 * 
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Signature_Release(FPDF_DOCUMENT doc, FPDF_SIGNATURE_FIELD sigField);

#ifdef __cplusplus
};
#endif

#endif 
// _FPDF_SIGNATURE_H_
/**@}*/