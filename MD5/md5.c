#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义常量
#define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~(z))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~(z))))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// 四轮循环的常量
static unsigned int T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// 四轮循环的位移量
static unsigned int s[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

// 初始化MD5缓冲区
void md5_init(unsigned int state[4]) {
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}

// 处理512位数据块
void md5_transform(unsigned int state[4], const unsigned char block[64]) {
    unsigned int a = state[0], b = state[1], c = state[2], d = state[3];
    unsigned int x[16];
    int i;

    // 将64字节的数据块转换为16个32位整数
    for (i = 0; i < 16; i++) {
        x[i] = (unsigned int)block[i * 4] |
            ((unsigned int)block[i * 4 + 1] << 8) |
            ((unsigned int)block[i * 4 + 2] << 16) |
            ((unsigned int)block[i * 4 + 3] << 24);
    }

    // 四轮循环
    for (i = 0; i < 64; i++) {
        unsigned int f, g;
        if (i < 16) {
            f = F(b, c, d);
            g = i;
        }
        else if (i < 32) {
            f = G(b, c, d);
            g = (5 * i + 1) % 16;
        }
        else if (i < 48) {
            f = H(b, c, d);
            g = (3 * i + 5) % 16;
        }
        else {
            f = I(b, c, d);
            g = (7 * i) % 16;
        }

        unsigned int temp = d;
        d = c;
        c = b;
        b = b + ROTATE_LEFT((a + f + T[i] + x[g]), s[i]);
        a = temp;
    }

    // 更新状态
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

// 计算MD5哈希值
void md5(const unsigned char* input, size_t length, unsigned char digest[16]) {
    unsigned int state[4];
    md5_init(state);

    size_t block_count = length / 64;
    for (size_t i = 0; i < block_count; i++) {
        md5_transform(state, input + i * 64);
    }

    // 处理剩余数据
    unsigned char buffer[64];
    size_t remaining = length % 64;
    memcpy(buffer, input + block_count * 64, remaining);
    buffer[remaining] = 0x80;
    remaining++;
    if (remaining > 56) {
        memset(buffer + remaining, 0, 64 - remaining);
        md5_transform(state, buffer);
        memset(buffer, 0, 56);
    }
    else {
        memset(buffer + remaining, 0, 56 - remaining);
    }

    // 添加原始数据长度
    unsigned long long bit_length = length * 8;
    for (int i = 0; i < 8; i++) {
        buffer[56 + i] = (unsigned char)(bit_length >> (i * 8));
    }
    md5_transform(state, buffer);

    // 输出结果
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            digest[i * 4 + j] = (unsigned char)(state[i] >> (j * 8));
        }
    }
}

// 打印MD5哈希值
void print_md5(const unsigned char digest[16]) {
    for (int i = 0; i < 16; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");
}

int main() {
    char input[1024];
    unsigned char digest[16];

    // 提示用户输入内容
    printf("请输入要加密的内容：");
    fgets(input, sizeof(input), stdin);

    // 去除fgets可能读取的换行符
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    // 显示原文
    printf("原文: %s\n", input);

    // 计算MD5哈希值
    md5((const unsigned char*)input, strlen(input), digest);

    // 显示加密后的数据
    printf("加密后的数据 (MD5): ");
    print_md5(digest);

    return 0;
}