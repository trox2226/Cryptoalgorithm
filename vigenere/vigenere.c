#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// 维吉尼亚密码加密解密函数
// input: 输入的字符串
// key: 加密解密的密钥
// output: 输出的字符串
// encrypt: 加密标志，1 表示加密，0 表示解密
void vigenere_cipher(const char* input, const char* key, char* output, int encrypt) {
    int key_len = strlen(key);  // 计算密钥的长度
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        char output_char = c;

        // 检查字符是否为大写字母
        int is_upper = (c >= 'A' && c <= 'Z');
        // 检查字符是否为小写字母
        int is_lower = (c >= 'a' && c <= 'z');

        // 如果不是字母，则直接保留该字符
        if (!is_upper && !is_lower) {
            output[i] = c;
            continue;
        }

        // 获取当前密钥字符
        char k = key[j % key_len];
        j++;

        // 确定字符的基准值
        int base = is_upper ? 'A' : 'a';
        // 将字符转换为 0 - 25 的数字
        int num = (is_upper ? c : c - 32) - 'A';
        // 将密钥字符转换为 0 - 25 的数字
        int key_num = (k >= 'A' && k <= 'Z') ? k - 'A' : (k - 'a');

        // 根据加密标志进行加密或解密操作
        int result = encrypt ? (num + key_num) : (num - key_num);
        // 确保结果在 0 - 25 的范围内
        result = (result + 26) % 26;

        // 计算输出字符
        output_char = result + base;
        output[i] = output_char;
    }

    // 在输出字符串末尾添加结束符
    output[i] = '\0';
}

int main() {
    char plaintext[800];  // 明文
    char ciphertext[800]; // 密文
    char key[800];        // 密钥
    char decryptedtext[800]; // 解密后的文本

    printf("请输入明文：\n");
    // 使用 fgets 函数读取包含空格的输入
    fgets(plaintext, sizeof(plaintext), stdin);
    // 去除 fgets 可能读取的换行符
    plaintext[strcspn(plaintext, "\n")] = 0;

    printf("输入密钥：\n");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    // 加密明文
    vigenere_cipher(plaintext, key, ciphertext, 1);
    // 解密密文
    vigenere_cipher(ciphertext, key, decryptedtext, 0);

    // 输出密文
    printf("加密后的文本：%s\n", ciphertext);
    // 输出解密后的文本
    printf("解密后的文本：%s\n", decryptedtext);

    return 0;
}