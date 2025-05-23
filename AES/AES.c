#include <stdio.h>
#include <string.h>

#define Nb 4
#define Nk 4
#define Nr 10
#define AES_BLOCK_SIZE 16

// S盒
static const unsigned char sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// 逆 S 盒
static const unsigned char inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

// 轮常量
static const unsigned char rcon[11] = { 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

// 字节替换
void sub_bytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

// 逆字节替换
void inv_sub_bytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}

// 行移位
void shift_rows(unsigned char state[4][4]) {
    unsigned char temp;

    // 第一行循环左移 1 字节
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;

    // 第二行循环左移 2 字节
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // 第三行循环左移 3 字节
    temp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp;
}

// 逆行移位
void inv_shift_rows(unsigned char state[4][4]) {
    unsigned char temp;

    // 第一行循环右移 1 字节
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;

    // 第二行循环右移 2 字节
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;

    // 第三行循环右移 3 字节
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}

// 有限域上的乘法
unsigned char gf_mul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    unsigned char counter;
    unsigned char hi_bit_set;
    for (counter = 0; counter < 8; counter++) {
        if ((b & 1) == 1) {
            p ^= a;
        }
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set == 0x80) {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return p;
}

// 列混淆
void mix_columns(unsigned char state[4][4]) {
    unsigned char temp[4];
    for (int i = 0; i < 4; i++) {
        temp[0] = gf_mul(0x02, state[0][i]) ^ gf_mul(0x03, state[1][i]) ^ state[2][i] ^ state[3][i];
        temp[1] = state[0][i] ^ gf_mul(0x02, state[1][i]) ^ gf_mul(0x03, state[2][i]) ^ state[3][i];
        temp[2] = state[0][i] ^ state[1][i] ^ gf_mul(0x02, state[2][i]) ^ gf_mul(0x03, state[3][i]);
        temp[3] = gf_mul(0x03, state[0][i]) ^ state[1][i] ^ state[2][i] ^ gf_mul(0x02, state[3][i]);
        state[0][i] = temp[0];
        state[1][i] = temp[1];
        state[2][i] = temp[2];
        state[3][i] = temp[3];
    }
}

// 逆列混淆
void inv_mix_columns(unsigned char state[4][4]) {
    unsigned char temp[4];
    for (int i = 0; i < 4; i++) {
        temp[0] = gf_mul(0x0e, state[0][i]) ^ gf_mul(0x0b, state[1][i]) ^ gf_mul(0x0d, state[2][i]) ^ gf_mul(0x09, state[3][i]);
        temp[1] = gf_mul(0x09, state[0][i]) ^ gf_mul(0x0e, state[1][i]) ^ gf_mul(0x0b, state[2][i]) ^ gf_mul(0x0d, state[3][i]);
        temp[2] = gf_mul(0x0d, state[0][i]) ^ gf_mul(0x09, state[1][i]) ^ gf_mul(0x0e, state[2][i]) ^ gf_mul(0x0b, state[3][i]);
        temp[3] = gf_mul(0x0b, state[0][i]) ^ gf_mul(0x0d, state[1][i]) ^ gf_mul(0x09, state[2][i]) ^ gf_mul(0x0e, state[3][i]);
        state[0][i] = temp[0];
        state[1][i] = temp[1];
        state[2][i] = temp[2];
        state[3][i] = temp[3];
    }
}

// 轮密钥加
void add_round_key(unsigned char state[4][4], unsigned char round_key[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] ^= round_key[i][j];
        }
    }
}

// 密钥扩展
void key_expansion(unsigned char key[4 * Nk], unsigned char round_keys[4 * Nb * (Nr + 1)]) {
    for (int i = 0; i < 4 * Nk; i++) {
        round_keys[i] = key[i];
    }

    for (int i = 4 * Nk; i < 4 * Nb * (Nr + 1); i++) {
        unsigned char temp[4];
        for (int j = 0; j < 4; j++) {
            temp[j] = round_keys[i - 4 + j];
        }
        if (i % (4 * Nk) == 0) {
            // 循环左移
            unsigned char t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;

            // 字节替换
            for (int j = 0; j < 4; j++) {
                temp[j] = sbox[temp[j]];
            }

            // 异或轮常量
            temp[0] ^= rcon[i / (4 * Nk)];
        }
        if (Nk > 6 && i % (4 * Nk) == 4 * 4) {
            for (int j = 0; j < 4; j++) {
                temp[j] = sbox[temp[j]];
            }
        }
        for (int j = 0; j < 4; j++) {
            round_keys[i + j] = round_keys[i - 4 * Nk + j] ^ temp[j];
        }
        i += 3;
    }
}

// AES 加密
void aes_encrypt(unsigned char plaintext[16], unsigned char key[16], unsigned char ciphertext[16]) {
    unsigned char state[4][4];
    unsigned char round_keys[4 * Nb * (Nr + 1)];

    // 初始状态
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = plaintext[j * 4 + i];
        }
    }

    // 密钥扩展
    key_expansion(key, round_keys);

    // 初始轮密钥加
    unsigned char round_key[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            round_key[i][j] = round_keys[j * 4 + i];
        }
    }
    add_round_key(state, round_key);

    // 9 轮迭代
    for (int round = 1; round < Nr; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                round_key[i][j] = round_keys[round * 4 * Nb + j * 4 + i];
            }
        }
        add_round_key(state, round_key);
    }

    // 最后一轮
    sub_bytes(state);
    shift_rows(state);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            round_key[i][j] = round_keys[Nr * 4 * Nb + j * 4 + i];
        }
    }
    add_round_key(state, round_key);

    // 输出密文
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ciphertext[j * 4 + i] = state[i][j];
        }
    }
}

// AES 解密
void aes_decrypt(unsigned char ciphertext[16], unsigned char key[16], unsigned char plaintext[16]) {
    unsigned char state[4][4];
    unsigned char round_keys[4 * Nb * (Nr + 1)];

    // 初始状态
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = ciphertext[j * 4 + i];
        }
    }

    // 密钥扩展
    key_expansion(key, round_keys);

    // 初始轮密钥加
    unsigned char round_key[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            round_key[i][j] = round_keys[Nr * 4 * Nb + j * 4 + i];
        }
    }
    add_round_key(state, round_key);

    // 9 轮迭代
    for (int round = Nr - 1; round > 0; round--) {
        inv_shift_rows(state);
        inv_sub_bytes(state);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                round_key[i][j] = round_keys[round * 4 * Nb + j * 4 + i];
            }
        }
        add_round_key(state, round_key);
        inv_mix_columns(state);
    }

    // 最后一轮
    inv_shift_rows(state);
    inv_sub_bytes(state);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            round_key[i][j] = round_keys[j * 4 + i];
        }
    }
    add_round_key(state, round_key);

    // 输出明文
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            plaintext[j * 4 + i] = state[i][j];
        }
    }
}

int main() {
    unsigned char plaintext[16] = "Hello, AES!";
    unsigned char key[16] = "0123456789abcdef";
    unsigned char ciphertext[16];
    unsigned char decryptedtext[16];

    // 加密
    aes_encrypt(plaintext, key, ciphertext);

    // 解密
    aes_decrypt(ciphertext, key, decryptedtext);

    // 输出结果
    printf("明文: %s\n", plaintext);
    printf("密文: ");
    for (int i = 0; i < 16; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n");
    printf("解密后的明文: %s\n", decryptedtext);

    return 0;
}