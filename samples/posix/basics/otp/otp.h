
#ifndef __OPT_H
#define __OPT_H

#include <stdio.h>
#include "mbedtls/md.h"

void hotp_set_num(uint8_t num);

void hotp_md_set(mbedtls_md_type_t type);

mbedtls_md_type_t hotp_md_get(void);

/*
 * @brief: 生成HOTP
 */
int32_t hotp_gen(uint8_t *key, uint8_t key_len, 
				 int64_t counter);

#endif