/** Copyright (C) 2012 Foxit Corporation. All Rights Reserved.
  * The following code is copyrighted and contains proprietary information and trade secrets of Foxit Corporation. 
  * You can only redistribute files listed below to customers of your application, under a written SDK license agreement with Foxit. 
  * You cannot distribute any part of the SDK to general public, even with a SDK license agreement. 
  * Without SDK license agreement, you cannot redistribute anything.
  * Functions in this header file, require "FPDFSECURITY" module to be enabled in your SDK license.
  * @file	fpdf_security.h
  * @author	Foxit Corporation
  * @brief	Header file for the Security module.<br>
  *			The Security Module is used for encrypting and decrypting PDF. 
  *			Currently, this module supports custom encryption and decryption, and public-key encryption and decryption.
  *			The callback mechanism provided in this module allow users to operate the PDF file.  
  * @note	If you want to purchase Foxit PDF SDK license and use ANY of the following functions, please
  *			request for enabling Security module explicitly. 
  * @version 2.0
  */

/** 
 * @addtogroup FPDFSECURITY Security
 * @brief Methods in this module are included in fpdf_security.h
 */
/**@{*/

#ifndef _FPDF_SECURITY_H_
#define _FPDF_SECURITY_H_

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
* @name Macro Definitions for Encryption Algorithm.
*/
/**@{*/
/** @brief Not use encryption algorithm. */
#define FPDF_CIPHER_NONE	0
/** @brief Use RC4 encryption algorithm. */
#define FPDF_CIPHER_RC4		1
/** @brief Use AES encryption algorithm. */
#define FPDF_CIPHER_AES		2
/**@}*/

#ifndef _FS_DEF_SECURITYHANDLER_
#define _FS_DEF_SECURITYHANDLER_
/** 
* @brief Structure for Security Handler. 
*
* @details	This structure holds a number of interfaces allowing application to use customized security handlers.<br>
*			A security handler is responsible for decrypting data and enforce document permissions.
*/
typedef struct _FPDF_SECURITY_HANDLER_
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FPDF_SECURITY_HANDLER). */
	FS_DWORD	lStructSize;

	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	 * @brief SDK calls this interface to initialize the custom handler
	 *
	 * @return	A pointer to the internal data structure of the custom handler, NULL for error.
	 *			This pointer is supplied when SDK calls all other interfaces.
 	 * 
	 */
	FS_LPVOID	(*CreateHandler)();

	/**
	 * @brief Get permission settings of the document.
	 * 
	 * @param[in]	handler			Internal handler pointer, returned by CreateHandler interface.
	 * @param[in]	permission		The original permission settings of the document.
	 *
	 * @return  A combination of permission flags. See macro definitions <b>FPDF_PERMISSION_XXX</b>
	 *
	 */
	FS_DWORD	(*GetPermissions)(FS_LPVOID handler,FS_DWORD permission);
	
	/**
	 * @brief Setup a decryption filter, for decrypting a data stream.
	 * 
	 * @param[in]	handler			Internal handler pointer, returned by CreateHandler interface
	 * @param[in]	objnum			PDF object number for the object we are decrypting.
	 * @param[in]	version			PDF version number for the object we are decrypting.
	 *
	 * @return	Implementation should return a pointer to internal structure of the created filter.<br>
	 *			Return <b>NULL</b> for failure.
	 *
	 */
	FS_LPVOID	(*CreateFilter)(FS_LPVOID handler, FS_INT32 objnum, FS_INT32 version);
	
	/**
	 * @brief Input encrypted data to the filter.
	 *
	 * @param[in]	filter			Internal filter pointer, returned by CreateFilter interface
	 * @param[in]	output			An output buffer handle supplied by SDK.
	 *								Can be used when calling ::FPDF_Security_OutputDecrypted to output decrypted data.
	 * @param[in]	src_data		Source (encrypted) data.
	 * @param[in]	src_len			Length (number of bytes) of the source data.
	 *
	 * @return	Non-zero for success, 0 for failure.
	 *
	 * @note	Implementation of this interface should be prepared to be called several times,
	 *			for the same data stream to be decrypted. Each time, SDK supplies part of the source
	 *			data. Whenever the implementation is ready to output some decrypted data, it should
	 *			do so by calling ::FPDF_Security_OutputDecrypted function.
	 */
	FS_BOOL		(*FilterInput)(FS_LPVOID filter, FPDF_DECRYPT_OUTPUT output, FS_LPCBYTE src_data, FS_DWORD src_len);

	/**
	 * @brief Finish a decrypt filter.
	 *
	 * @param[in]	filter			Internal filter pointer, returned by CreateFilter interface
	 * @param[in]	output			An output buffer handle supplied by SDK.
	 *								Can be used when calling ::FPDF_Security_OutputDecrypted to output decrypted data.
	 *
	 * @return	Non-zero for success, 0 for failure.
	 *
	 * @note	FinishFilter implementation can also call ::FPDF_Security_OutputDecrypted function to output 
	 *			any left-over decrypted data.<br>
	 *			It's also OK for the filter implementation to cache all the input data in FilterInput
	 *			interface, and decrypt the whole data stream when FinishFilter is called. 
	 *			If an internal filter structure is created during CreateFilter interface, then this is
	 *			the time to free that structure and any associated resources.
	 */
	FS_BOOL		(*FinishFilter)(FS_LPVOID filter, FPDF_DECRYPT_OUTPUT output);

	/**
	 * @brief Finish the custom security handler.
	 * 
	 * @param[in]	handler			Internal handler pointer, returned by CreateHandler interface.
	 *
	 * @note		It's time to free the internal handler structure and all associated resources.
	 */
	void		(*FinishHandler)(FS_LPVOID handler);
	
	/**
	 * @brief Get encryption information including standard algorithm and key. 
	 *
	 * @details	SDK will call the function twice:
	 *			first to receive the required length of the buffer, and second to receive the buffer of key content.
	 *
	 * @param[in]	handler			Internal handler pointer, returned by CreateHandler interface
	 * @param[in]	cipher			Cipher identifier. It can be one of the below:<br>
	 *									<ul>
	 *									<li>::FPDF_CIPHER_NONE</li>
	 *									<li>::FPDF_CIPHER_RC4</li>
	 *									<li>::FPDF_CIPHER_AES</li>
	 *									</ul>
	 * @param[in]	buffer			Pointer to a key.If it is NULL, then the parameter <i>keylen</i> should received the required length of the buffer.
	 * @param[in]	keylen			Length (number of bytes) of the buffer.
	 *
	 * @return <b>TRUE</b> for success, otherwise failure.
	 *
	 * @note	As long as one of CreateFilter,FilterInput and FinishFilte is set to NULL, 
	 *			we think users are using standard encryption for encryption and decryption.
	 *			The GetCryptInfo method must be set.
	 */
	FS_BOOL		(*GetCryptInfo)(FS_LPVOID handler, FS_INT32* cipher, FS_LPBYTE buffer, FS_LPDWORD  keylen);

	 /**
	  * @brief Get encrypted data size for a source data block.
	  *
	  * @param[in]	handler			Internal handler pointer, returned by CreateHandler interface
	  * @param[in]	objnum			PDF object number for the object we are encrypting.
	  * @param[in]	version			PDF version number for the object we are encrypting.
	  * @param[in]	src_buf			The source data block.
	  * @param[in]	src_size		The size in bytes of the source data.
	  *
	  * @return The encrypted data size. It should equal to or be larger than the final encrypted data block.
	  *
	  */
	 FS_DWORD	(*EncryptGetSize)(FS_LPVOID handler, FS_INT32 objnum, FS_INT32 version, FS_LPCBYTE src_buf, FS_DWORD src_size);

	/**
	 * @brief Do the encryption process.
	 *
 	 * @details Final encrypted data block should be output in the parameter <i>dest_size</i>.
	 *
	 * @param[in]	handler			Internal handler pointer, returned by CreateHandler interface
	 * @param[in]	objnum			PDF object number for the object we are encrypting.
	 * @param[in]	version			PDF version number for the object we are encrypting.
	 * @param[in]	src_buf			The source data block.
	 * @param[in]	src_size		The size in bytes of the source data.
	 * @param[in]	dest_buf		It receives the encrypted data.
	 * @param[in]	dest_size		It receives the size in bytes of the encrypted data.
	 *
	 * @return <b>TRUE</b> for success, otherwise failure.
	 *
	*/
	 FS_BOOL	(*EncryptContent)(FS_LPVOID handler, FS_INT32 objnum, FS_INT32 version, FS_LPCBYTE src_buf, FS_DWORD src_size,
								  FS_LPBYTE dest_buf, FS_LPDWORD dest_size);

}FPDF_SECURITY_HANDLER;
#endif

#ifndef _FS_DEF_PKISECURITYHANDLER_
#define _FS_DEF_PKISECURITYHANDLER_
/** @brief Structure for Security Handler. */
typedef struct _FPDF_PKI_SECURITY_HANDLER_
{
	/** @brief The size of the data structure. It must be set to <I>sizeof</I>(::FPDF_PKI_SECURITY_HANDLER). */
	FS_DWORD	lStructSize;
	
	/** @brief The user-supplied data. */
	FS_LPVOID	clientData;

	/**
	 * @brief Get public key verify.
	 *
	 * @param[out] clientData		Pointer to the interface structure itself.
	 * @param[in] envelop			A handle to the certificate Enveloper
	 * @param[in] len				Length of the certificate file data buffer, in bytes.
	 * @param[in] seed_buf			The source seed buf block.
	 * @param[in] buf_len			The size in bytes of the seed_buf data.
	 *
	 * @return <b>TRUE</b> for success, otherwise failure.
	 *
	 */
	FS_BOOL    (*GetPubKeyVerify)(FS_LPVOID clientData,FS_LPCSTR envelope,FS_DWORD len, FS_LPBYTE seed_buf, FS_LPDWORD buf_len);
	
}FPDF_PKI_SECURITY_HANDLER;
#endif

/** 
 * @brief 	Register a PDF security handler to PDF SDK, enabling access to a PDF document which
 *			protected by PKI or non-standard security handler.
 *
 * @param[in]	handler_name		Name of the handler. It must be "Adobe.PubSec" for public-key decryption.
 * @param[in]	pHandler			Pointer	to a structure containing callback functions for the handler.
 *									Must point to a ::FPDF_PKI_SECURITY_HANDLER structure if do the public-key decryption, 
 *									otherwise point to a ::FPDF_SECURITY_HANDLER structure.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Security_RegisterHandler(FS_LPCSTR handler_name, FS_LPVOID pHandler);

/** 
 * @brief 	UnRegister a PDF security handler to PDF SDK.
 *
 * @param[in]	handler_name		Name of the handler. It must be "Adobe.PubSec" for public-key decryption.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT  FPDF_Security_UnRegisterHandler(FS_LPCSTR handler_name);

/** 
 * @brief Output data decrypted by a custom security handler.
 *
 * @param[in]	output				Handler to the output buffer.
 * @param[in]	data				Pointer to decrypted data.
 * @param[in]	len					Number of bytes for the decrypted data.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Security_OutputDecrypted(FPDF_DECRYPT_OUTPUT output, FS_LPCSTR data, FS_DWORD len);

/** 
 * @brief 	Encrypt the PDF in custom way,the PDF document will be protected by no-standard security handler.
 *
 * @param[in]	doc					Handler to the document.
 * @param[in]	handler_name		The name of the handler.
 * @param[in]	pHandler			Pointer	to a structure containing callback functions for the handler.
 * @param[in]	fileWrite			A pointer to a custom file write structure.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Security_CustomEncrypt(FPDF_DOCUMENT doc, FS_LPCSTR handler_name, FPDF_SECURITY_HANDLER* pHandler, FS_FILEWRITE* fileWrite);

/** 
 * @brief 	Remove all the security setting of the PDF document, the output PDF will be decrypted.
 *
 * @param[in]	doc					Handler to the document.
 * @param[in]	fileWrite			A pointer to a custom file write structure.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Security_Remove(FPDF_DOCUMENT doc, FS_FILEWRITE* fileWrite);

/** 
 * @brief 	Get level of password: no password, user password, or owner password.
 *
 * @param[in]	doc					Handler to the document.
 * @param[out]	level				Used to receive the password level.<br>
 *									<ul>
 *									<li>0 : no password</li>
 *									<li>1 : user password</li>
 *									<li>2 : owner password</li>
 *									</ul>
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Security_GetPasswordLevel(FPDF_DOCUMENT doc, FS_INT32* level);

/**
 * @brief Create a certificate envelopes.
 *
 * @param[out]	envelopes            A handle to the certificate envelopes.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 */
FS_RESULT FPDF_Security_CreateEnvelopes(FPDF_ENVELOPES* envelopes);

/**
 * @brief Add recipient from the certificate to the certificate recipients handle.
 *
 * @param[in]	envelopes			A handle to the certificate Enveloper array.
 * @param[in]   envelopeData		A handle to the certificate Enveloper.
 * @param[in]   dataLen				Length of the certificate file data buffer, in bytes.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Security_AddEnvelope(FPDF_ENVELOPES envelopes, FS_LPVOID envelopeData, FS_DWORD dataLen);

/**
 * @brief Encrypt the document. 
 /
 * @param[in]      document           A handle to the PDF document.
 * @param[in]      nCipher            Encryption algorithm, as defined above.
 * @param[in]      bEncryptMetaData   Whether to encrypt the meta data or not.
 * @param[in]      envelops           Envelop array.
 * @param[in]      key                After envelops calculate to get key information
 * @param[in]      nKeyBytes          pecified encryption algorithm length
 * @param[in]      fileWrite          A pointer to a custom file write structure.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>. 
 *			
 */
FS_RESULT FPDF_Security_CertEncrypt(FPDF_DOCUMENT document, FS_INT32 nCipher, FS_BOOL bEncryptMetaData,
									FPDF_ENVELOPES envelops, FS_LPCBYTE key, FS_DWORD nKeyBytes, 
									FS_FILEWRITE* fileWrite);

/**
 * @brief Destroy the certificate recipients handle. 
 *
 * @param[in]      envelops				A handle to the certificate envelopes.
 *
 * @return	::FS_ERR_SUCCESS means success.<br>
 *			 For more definitions please see macro definitions <b>FS_ERR_XXX</b>.
 *
 */
FS_RESULT FPDF_Security_DestroyEnvelopes(FPDF_ENVELOPES envelops);

#ifdef __cplusplus
};
#endif

#endif
//_FPDF_SECURITY_H_
/**@}*/