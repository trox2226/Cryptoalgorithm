#define _CRT_SECURE_NO_WARNINGS
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include"caesar.h"
void rwfile() {
    int offset;
    wchar_t message[800];
    wchar_t enmessage[800];
    wchar_t decmessage[800];
    FILE* fp;

    // 设置本地化环境以支持宽字符
    setlocale(LC_ALL, "");

    // 获取偏移量
    printf("set your offset(please remember your enter): ");
    if (scanf("%d", &offset) != 1) {
        fprintf(stderr, "输入偏移量时发生错误，请输入有效的整数。\n");
        while (getchar() != '\n'); // 清空输入缓冲区
        return;
    }
    while (getchar() != '\n'); // 消耗掉 scanf 残留的换行符

    // 获取明文
    wprintf(L"please enter your message:\n");
    if (fgetws(message, sizeof(message) / sizeof(wchar_t), stdin) == NULL) {
        fprintf(stderr, "输入明文时发生错误。\n");
        return;
    }
    // 去除 fgetws 读取的换行符
    size_t len = wcslen(message);
    if (len > 0 && message[len - 1] == L'\n') {
        message[len - 1] = L'\0';
    }

    // 加密明文
    for (int i = 0; message[i] != L'\0'; i++) {
        enmessage[i] = encrypt(message[i], offset);
    }
    enmessage[wcslen(message)] = L'\0'; // 确保加密后的字符串以 L'\0' 结尾

    // 解密密文
    for (int i = 0; enmessage[i] != L'\0'; i++) {
        decmessage[i] = decrypt(enmessage[i], offset);
    }
    decmessage[wcslen(enmessage)] = L'\0'; // 确保解密后的字符串以 L'\0' 结尾

    // 打开文件以追加模式写入
    fp = _wfopen(L"message.txt", L"a,ccs=UTF-8");
    if (fp == NULL) {
        perror("无法打开文件");
        return;
    }

    // 将明文写入文件
    fwprintf(fp, L"明文: %ls\n", message);
    // 将密文写入文件
    fwprintf(fp, L"密文: %ls\n", enmessage);
    // 将解密后的明文写入文件
    fwprintf(fp, L"解密后的明文: %ls\n", decmessage);

    // 关闭文件
    fclose(fp);

    wprintf(L"明文、密文和解密后的明文已成功写入 message.txt 文件。\n");
}