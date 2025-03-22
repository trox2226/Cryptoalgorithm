#ifndef DES_H
#define DES_H

#include <stdint.h>

// 结构体定义
typedef struct {
    uint64_t subkeys[16];
} DES_KEY;

// 函数声明
void generate_subkeys(const unsigned char* key, DES_KEY* des_key);
void des_encrypt_block(unsigned char* block, const DES_KEY* key);
void des_decrypt_block(unsigned char* block, const DES_KEY* key);

#endif