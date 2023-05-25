/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Modifications Copyright (C) 2023 Renesas Electronics Corporation. or its affiliates.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef PKCS11_OPERATIONS_H_
#define PKCS11_OPERATIONS_H_

/* Standard includes. */
#include <stdlib.h>
#include <stdbool.h>

/* corePKCS11 include. */
#include "core_pkcs11.h"

/**
 * @brief Determine which required client crypto objects are already present in storage.
 *
 * @param[in] p11Session The PKCS #11 session to use.
 * @param[out] object handle for device certificate.
 * @param[out] object handle for device private key.
 *
 * @return CKR_OK if certificate import succeeded. Otherwise, a positive PKCS #11 error code.
 */
CK_RV xGetCertificateAndKeyState( CK_SESSION_HANDLE xP11Session,
                                  CK_OBJECT_HANDLE_PTR pxClientCertificate,
                                  CK_OBJECT_HANDLE_PTR pxPrivateKey );

/**
 * @brief Loads the claim credentials into the PKCS #11 module. Claim
 * credentials are used in "Provisioning by Claim" workflow of Fleet
 * Provisioning feature of AWS IoT Core. For more information, refer to the
 * [AWS documentation](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html#claim-based)
 *
 * Note: This function is for demonstration purposes, and the claim credentials
 * should be securely stored in production devices. For example, the
 * shared claim credentials could be loaded into a secure element on the devices
 * in your fleet at the time of manufacturing.
 *
 * @param[in] p11Session The PKCS #11 session to use.
 * @param[in] pClaimCertPath Path to the claim certificate.
 * @param[in] pClaimCertLabel PKCS #11 label for the claim certificate.
 * @param[in] pClaimPrivKeyPath Path to the claim private key.
 * @param[in] pClaimPrivKeyLabel PKCS #11 label for the claim private key.
 *
 * @return True on success.
 */
bool xLoadClaimCredentials( CK_SESSION_HANDLE xP11Session,
                           const char * pClaimCert,
                           size_t       ClaimCertLength,
                           const char * pClaimPrivKey,
                           size_t       ClaimPrivKeyLength );

/**
 * @brief Generate a new public-private key pair in the PKCS #11 module, and
 * generate a certificate signing request (CSR) for them.
 *
 * This device-generated private key and CSR can be used with the
 * CreateCertificateFromCsr API of the the Fleet Provisioning feature of AWS IoT
 * Core in order to provision a unique client certificate.
 *
 * @param[in] xP11Session The PKCS #11 session to use.
 * @param[in] pcPrivKeyLabel PKCS #11 label for the private key.
 * @param[in] pcPubKeyLabel PKCS #11 label for the public key.
 * @param[out] pcCsrBuffer The buffer to write the CSR to.
 * @param[in] xCsrBufferLength Length of #pcCsrBuffer.
 * @param[out] pcOutCsrLength The length of the written CSR.
 *
 * @return True on success.
 */
bool xGenerateKeyAndCsr( CK_SESSION_HANDLE xP11Session,
                         const char * pcPrivKeyLabel,
                         const char * pcPubKeyLabel,
                         char * pcCsrBuffer,
                         size_t xCsrBufferLength,
                         size_t * pcOutCsrLength,
                         const char * pcCsrsubjectname );

/**
 * @brief Save the device client certificate into the PKCS #11 module.
 *
 * @param[in] xP11Session The PKCS #11 session to use.
 * @param[in] pcCertificate The certificate to save.
 * @param[in] pcLabel PKCS #11 label for the certificate.
 * @param[in] xCertificateLength Length of #pcCertificate.
 *
 * @return True on success.
 */
bool xLoadCertificate( CK_SESSION_HANDLE xP11Session,
                       const char * pcCertificate,
                       const char * pcLabel,
                       size_t xCertificateLength );


/**
 * @brief Import the specified private key into storage.
 *
 * @param[in] session The PKCS #11 session.
 * @param[in] privateKey The private key to store, in PEM format.
 * @param[in] privateKeyLength The length of the key, including null terminator.
 * @param[in] label The label to store the key.
 */
CK_RV provisionPrivateKey( CK_SESSION_HANDLE session,
                                  const char * privateKey,
                                  size_t privateKeyLength,
                                  const char * label );

/**
 * @brief Import the specified X.509 client certificate into storage.
 *
 * @param[in] session The PKCS #11 session.
 * @param[in] certificate The certificate to store, in PEM format.
 * @param[in] certificateLength The length of the certificate, including the null terminator.
 * @param[in] label The label to store the certificate.
 */
CK_RV provisionCertificate( CK_SESSION_HANDLE session,
                                   const char * certificate,
                                   size_t certificateLength,
                                   const char * label );

/**
 * @brief Close the PKCS #11 session.
 *
 * @param[in] xP11Session The PKCS #11 session to use.
 *
 * @return True on success.
 */
bool xPkcs11CloseSession( CK_SESSION_HANDLE xP11Session );

#endif /* ifndef PKCS11_OPERATIONS_H_ */
