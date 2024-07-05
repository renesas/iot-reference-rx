#!/bin/sh

# If the format of client certificate is RSA, this project utilize RSA root CA certificate.

INPUT_CRT_ROOT_PEM="./ca/AmazonRootCA1.pem"
OUTPUT_CRT_ROOT_DER="./output/AmazonRootCA1_crt.der"
OUTPUT_CRT_ROOT_TXT="./output/AmazonRootCA1_crt_array.txt"

INPUT_KEY_PRIVATE_PEM="./ca-sign-keypair-rsa2048/rsa2048-private.pem"
INPUT_KEY_PUBLIC_PEM="./ca-sign-keypair-rsa2048/rsa2048-public.pem"

OUTPUT_CRT_ROOT_SIG_BIN="./output/AmazonRootCA1_sig.sig"
OUTPUT_CRT_ROOT_SIG_TXT="./output/AmazonRootCA1_sig_array.txt"

. ./convertCrt.sh
