#ifndef DES_H
#define DES_H

#include <stdint.h>

// DES密钥结构体（存储16轮子密钥）
typedef struct {
    uint64_t subkeys[16];
} DES_KEY;

// 存储三个DES密钥的结构体
typedef struct {
    DES_KEY key1;
    DES_KEY key2;
    DES_KEY key3;
} TRIPLE_DES_KEY;

// DES加密块函数
void des_encrypt_block(unsigned char* block, const DES_KEY* key);

// DES解密块函数
void des_decrypt_block(unsigned char* block, const DES_KEY* key);

// 生成子密钥函数
void generate_subkeys(const unsigned char* key, DES_KEY* des_key);

// 三重DES加密块函数
void triple_des_encrypt_block(unsigned char* block, const TRIPLE_DES_KEY* key);

// 三重DES解密块函数
void triple_des_decrypt_block(unsigned char* block, const TRIPLE_DES_KEY* key);

#endif
