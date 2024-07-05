#!/bin/sh

# 1.Convert PEM format root CA certicicate to DER
if [ -e $INPUT_CRT_ROOT_PEM ]
then
    openssl x509 -in $INPUT_CRT_ROOT_PEM -out $OUTPUT_CRT_ROOT_DER -outform der
    hexdump -v -e '/1 "0x%02x, "' $OUTPUT_CRT_ROOT_DER > $OUTPUT_CRT_ROOT_TXT
else
    echo "Root CA certificate is not found"
    exit 1
fi

# 2.Generate RSA-2048 key pair for signature if it is not exist
if [ ! -e $INPUT_KEY_PRIVATE_PEM ] || [ ! -e $INPUT_KEY_PUBLIC_PEM ]
then
    openssl genrsa -out $INPUT_KEY_PRIVATE_PEM 2048
    openssl rsa -in $INPUT_KEY_PRIVATE_PEM -pubout -out $INPUT_KEY_PUBLIC_PEM
fi

# 3.Create a signature of the Root CA certificate
if [ -e $INPUT_KEY_PRIVATE_PEM ] && [ -e $OUTPUT_CRT_ROOT_DER ]
then
    openssl dgst -sha256 -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-1 -sign $INPUT_KEY_PRIVATE_PEM -out $OUTPUT_CRT_ROOT_SIG_BIN $OUTPUT_CRT_ROOT_DER
    openssl dgst -sha256 -sigopt rsa_padding_mode:pss -verify $INPUT_KEY_PUBLIC_PEM -signature $OUTPUT_CRT_ROOT_SIG_BIN $OUTPUT_CRT_ROOT_DER
    hexdump -v -e '/1 "0x%02x, "' $OUTPUT_CRT_ROOT_SIG_BIN > $OUTPUT_CRT_ROOT_SIG_TXT
fi
