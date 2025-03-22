#ifndef DES_H
#define DES_H

#include <stdint.h>

// �ṹ�嶨��
typedef struct {
    uint64_t subkeys[16];
} DES_KEY;

// ��������
void generate_subkeys(const unsigned char* key, DES_KEY* des_key);
void des_encrypt_block(unsigned char* block, const DES_KEY* key);
void des_decrypt_block(unsigned char* block, const DES_KEY* key);

#endif