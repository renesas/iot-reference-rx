/*
 * FreeRTOS V202107.00
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

#ifndef AWS_TEST_PAL_ECDSA_SHA256_SIGNATURE_H_
#define AWS_TEST_PAL_ECDSA_SHA256_SIGNATURE_H_
#include <stdint.h>

/**
 * @brief Invalid signature for OTA PAL testing.
 */
static const uint8_t ucInvalidSignature[] =
{
    0x30, 0x44, 0x02, 0x20, 0x75, 0xde, 0xa8, 0x1f, 0xca, 0xec, 0xff, 0x16,
    0xbb, 0x38, 0x4b, 0xe3, 0x14, 0xe7, 0xfb, 0x68, 0xf5, 0x3e, 0x86, 0xa2,
    0x71, 0xba, 0x9e, 0x5e, 0x50, 0xbf, 0xb2, 0x7a, 0x9e, 0x00, 0xc6, 0x4d,
    0x02, 0x20, 0x19, 0x72, 0x42, 0x85, 0x2a, 0xac, 0xdf, 0x5a, 0x5e, 0xfa,
    0xad, 0x49, 0x17, 0x5b, 0xce, 0x5b, 0x65, 0x75, 0x08, 0x47, 0x3e, 0x55,
    0xf9, 0x0e, 0xdf, 0x9e, 0x8c, 0xdc, 0x95, 0xdf, 0x63, 0xd2
};
static const int ucInvalidSignatureLength = 70;

/**
 * @brief Valid signature matching the test block in the OTA PAL tests.
 */
static const uint8_t ucValidSignature[] =
{
	0x30, 0x44,
	0x02, 0x20,
	0x75, 0x6D, 0x95, 0x52, 0x2A, 0x61, 0x8B, 0xAE, 0x24, 0x94, 0xA2, 0x21, 0x75, 0xED, 0x86, 0x52,
	0xB7, 0xA1, 0x68, 0x9E, 0x9B, 0x38, 0x3C, 0x43, 0xF9, 0xB2, 0x3D, 0xC5, 0x77, 0xA1, 0x74, 0x47,
	0x02, 0x20,
	0x13, 0x64, 0xA6, 0x76, 0x29, 0x84, 0xEF, 0xC0, 0x51, 0xAE, 0x78, 0xF2, 0xD4, 0x2B, 0xC5, 0x87,
	0x8D, 0x92, 0xBE, 0x36, 0x44, 0x28, 0xB7, 0xF6, 0xA5, 0x51, 0x67, 0x00, 0xA3, 0x51, 0x19, 0xFF
};
static const int ucValidSignatureLength = 70;

/**
 * @brief Valid certificate matching the signature in the OTA PAL tests.
 * This certificate should be written to DF before running OTA PAL tests.
 */
#define OTA_PAL_CODE_SIGNING_CERTIFICATE                             \
"-----BEGIN CERTIFICATE-----\n"                                      \
"MIICYDCCAgUCFGdNeqMmwPw49IJ6t2vZAwTnbUt3MAoGCCqGSM49BAMCMIGxMQsw\n" \
"CQYDVQQGEwJKUDEOMAwGA1UECAwFVG9reW8xEDAOBgNVBAcMB0tvZGFpcmExHDAa\n" \
"BgNVBAoME1JlbmVzYXMgRWxlY3Ryb25pY3MxJjAkBgNVBAsMHVNvZnR3YXJlIERl\n" \
"dmVsb3BtZW50IERpdmlzaW9uMRMwEQYDVQQDDAp0ZXN0X3NpZ2VyMSUwIwYJKoZI\n" \
"hvcNAQkBFhZ0ZXN0X3NpZ2VyQHJlbmVzYXMuY29tMB4XDTIzMTEwNjA3NTExMloX\n" \
"DTMzMTEwMzA3NTExMlowgbExCzAJBgNVBAYTAkpQMQ4wDAYDVQQIDAVUb2t5bzEQ\n" \
"MA4GA1UEBwwHS29kYWlyYTEcMBoGA1UECgwTUmVuZXNhcyBFbGVjdHJvbmljczEm\n" \
"MCQGA1UECwwdU29mdHdhcmUgRGV2ZWxvcG1lbnQgRGl2aXNpb24xEzARBgNVBAMM\n" \
"CnRlc3Rfc2lnZXIxJTAjBgkqhkiG9w0BCQEWFnRlc3Rfc2lnZXJAcmVuZXNhcy5j\n" \
"b20wWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQrBsSyEpoceG3GaiMmJ0n8Zf7V\n" \
"L97m7AfWMyrfaiktNfq99yQpcbj9a8H0ipkH307oFDjBaPap/BatX1hdXK1IMAoG\n" \
"CCqGSM49BAMCA0kAMEYCIQCIP/3HOXzlvxsXJp4XOpor2jhh67mXOAl4Nwt4YVPr\n" \
"vgIhANm+vutywnRvuQ2SDdgG1ibz34I++Z8h3gLz/YiT2t3k\n"                 \
"-----END CERTIFICATE-----\n"
#endif /* ifndef AWS_TEST_PAL_ECDSA_SHA256_SIGNATURE_H_ */
