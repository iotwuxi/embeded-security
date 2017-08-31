#include <stdio.h>
#include <string.h>
#include "tinycbor/cbor.h"
#include "tinycbor/cborjson.h"

void print_hex(uint8_t *buf, uint8_t len)
{   
    for(int i = 0; i < len; i++)
    {
        printf("%02X ", *buf++);
    }
    printf("\n");
}
// a3 63 73 65 71 01 63 63 74 78 83 18 33 18 33 18 33 62 69 76 83 18 42 18 42 18 42

int main(int argc, char **argv)
{
    uint8_t buf[64] = {0x00, };
    int some_value = 0x10;

    CborEncoder encoder;
    CborEncoder mapEncoder;
    CborEncoder arrayEncoder;
    cbor_encoder_init(&encoder, buf, sizeof(buf), 0);

    cbor_encoder_create_map(&encoder, &mapEncoder, 3);

    // {"seq: 1"}
    cbor_encode_text_string(&mapEncoder, "seq", strlen("seq"));
    cbor_encode_int(&mapEncoder, 1);

    // {"iv": [0x33, 0x33, 0x33]}
    cbor_encode_text_string(&mapEncoder, "ctx", strlen("ctx"));
    cbor_encoder_create_array(&mapEncoder, &arrayEncoder, 3);
    cbor_encode_int(&arrayEncoder, 0x33);
    cbor_encode_int(&arrayEncoder, 0x33);
    cbor_encode_int(&arrayEncoder, 0x33);
    cbor_encoder_close_container_checked(&mapEncoder, &arrayEncoder);

    // {"iv": [0x42, 0x42, 0x42]}
    cbor_encode_text_string(&mapEncoder, "iv", strlen("iv"));
    cbor_encoder_create_array(&mapEncoder, &arrayEncoder, 3);
    cbor_encode_int(&arrayEncoder, 0x42);
    cbor_encode_int(&arrayEncoder, 0x42);
    cbor_encode_int(&arrayEncoder, 0x42);
    cbor_encoder_close_container_checked(&mapEncoder, &arrayEncoder);


    cbor_encoder_close_container_checked(&encoder, &mapEncoder);

    print_hex(buf, cbor_encoder_get_buffer_size(&encoder, buf));
    return 0;
}