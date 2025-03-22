#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DES.h"

int main() {
    char input[1024];
    char key_input[9];

    // 获取用户输入的明文
    printf("请输入要加密的明文: ");
    fgets(input, sizeof(input), stdin);
    // 去除 fgets 可能读取的换行符
    input[strcspn(input, "\n")] = 0;

    // 获取用户输入的密钥
    printf("请输入 8 字节的密钥: ");
    fgets(key_input, sizeof(key_input), stdin);
    // 去除 fgets 可能读取的换行符
    key_input[strcspn(key_input, "\n")] = 0;

    unsigned char* plaintext = (unsigned char*)input;
    size_t text_len = strlen((char*)plaintext);
    size_t block_size = 8;
    size_t pad = block_size - (text_len % block_size);
    pad = pad ? pad : block_size;
    size_t padded_len = text_len + pad;

    unsigned char* buffer = malloc(padded_len);
    memcpy(buffer, plaintext, text_len);
    memset(buffer + text_len, pad, pad);

    unsigned char key[8];
    memcpy(key, key_input, sizeof(key));

    DES_KEY des_key;
    generate_subkeys(key, &des_key);

    // 加密
    for (size_t i = 0; i < padded_len; i += block_size) {
        des_encrypt_block(buffer + i, &des_key);
    }

    // 将加密后的内容追加保存到 content.txt 文件
    FILE* file = fopen("content.txt", "a");
    if (file == NULL) {
        perror("无法打开文件");
        free(buffer);
        return 1;
    }
    for (size_t i = 0; i < padded_len; i++) {
        fprintf(file, "%02X ", buffer[i]);
    }
    fprintf(file, "\n");
    fclose(file);

    // 在控制台输出加密后的内容
    printf("加密后: ");
    for (size_t i = 0; i < padded_len; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    // 解密
    for (size_t i = 0; i < padded_len; i += block_size) {
        des_decrypt_block(buffer + i, &des_key);
    }

    // 去除填充
    size_t pad_value = buffer[padded_len - 1];
    if (pad_value > 0 && pad_value <= block_size) {
        int valid = 1;
        for (size_t i = padded_len - pad_value; i < padded_len; i++) {
            if (buffer[i] != pad_value) valid = 0;
        }
        if (valid) padded_len -= pad_value;
    }

    buffer[padded_len] = '\0';

    // 在控制台输出解密后的内容
    printf("解密后: %s\n", buffer);

    free(buffer);
    return 0;
}