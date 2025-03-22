#ifndef DES_H
#define DES_H

#include <stdint.h>

// DES��Կ�ṹ�壨�洢16������Կ��
typedef struct {
    uint64_t subkeys[16];
} DES_KEY;

// �洢����DES��Կ�Ľṹ��
typedef struct {
    DES_KEY key1;
    DES_KEY key2;
    DES_KEY key3;
} TRIPLE_DES_KEY;

// DES���ܿ麯��
void des_encrypt_block(unsigned char* block, const DES_KEY* key);

// DES���ܿ麯��
void des_decrypt_block(unsigned char* block, const DES_KEY* key);

// ��������Կ����
void generate_subkeys(const unsigned char* key, DES_KEY* des_key);

// ����DES���ܿ麯��
void triple_des_encrypt_block(unsigned char* block, const TRIPLE_DES_KEY* key);

// ����DES���ܿ麯��
void triple_des_decrypt_block(unsigned char* block, const TRIPLE_DES_KEY* key);

#endif
