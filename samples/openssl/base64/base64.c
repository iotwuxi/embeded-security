#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

int base64_encode(char *input, int len, char *output)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    size_t size = 0;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, input, len);
    BIO_flush(bio);

    BIO_get_mem_ptr(bio, &bptr);
    memcpy(output, bptr->data, bptr->length);
    output[bptr->length] = '\0';
    size = bptr->length;

    BIO_free_all(bio);
    return size;
}

int base64_decode(char *input, int len, char *output)
{
    BIO *b64, *bio;
    BUF_MEM *bptr = NULL;
    int counts;
    int size = 0;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    bio = BIO_new_mem_buf(input, len);
    bio = BIO_push(b64, bio);

    size = BIO_read(bio, output, len);
    output[size] = '\0';

    BIO_free_all(bio);
    return size;
}

int main(void)
{
	char message[] = "hello";
	char str_encode[256] = {0x00, };
	base64_encode(message, strlen(message), str_encode);
	printf("base64 encode: %s\n", str_encode);

	char str_decode[256] = {0};
	base64_decode(str_encode, strlen(str_encode), str_decode);
	printf("base64 decode: %s\n", str_decode);
	return 0;
}