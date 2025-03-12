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

    // ���ñ��ػ�������֧�ֿ��ַ�
    setlocale(LC_ALL, "");

    // ��ȡƫ����
    printf("set your offset(please remember your enter): ");
    if (scanf("%d", &offset) != 1) {
        fprintf(stderr, "����ƫ����ʱ����������������Ч��������\n");
        while (getchar() != '\n'); // ������뻺����
        return;
    }
    while (getchar() != '\n'); // ���ĵ� scanf �����Ļ��з�

    // ��ȡ����
    wprintf(L"please enter your message:\n");
    if (fgetws(message, sizeof(message) / sizeof(wchar_t), stdin) == NULL) {
        fprintf(stderr, "��������ʱ��������\n");
        return;
    }
    // ȥ�� fgetws ��ȡ�Ļ��з�
    size_t len = wcslen(message);
    if (len > 0 && message[len - 1] == L'\n') {
        message[len - 1] = L'\0';
    }

    // ��������
    for (int i = 0; message[i] != L'\0'; i++) {
        enmessage[i] = encrypt(message[i], offset);
    }
    enmessage[wcslen(message)] = L'\0'; // ȷ�����ܺ���ַ����� L'\0' ��β

    // ��������
    for (int i = 0; enmessage[i] != L'\0'; i++) {
        decmessage[i] = decrypt(enmessage[i], offset);
    }
    decmessage[wcslen(enmessage)] = L'\0'; // ȷ�����ܺ���ַ����� L'\0' ��β

    // ���ļ���׷��ģʽд��
    fp = _wfopen(L"message.txt", L"a,ccs=UTF-8");
    if (fp == NULL) {
        perror("�޷����ļ�");
        return;
    }

    // ������д���ļ�
    fwprintf(fp, L"����: %ls\n", message);
    // ������д���ļ�
    fwprintf(fp, L"����: %ls\n", enmessage);
    // �����ܺ������д���ļ�
    fwprintf(fp, L"���ܺ������: %ls\n", decmessage);

    // �ر��ļ�
    fclose(fp);

    wprintf(L"���ġ����ĺͽ��ܺ�������ѳɹ�д�� message.txt �ļ���\n");
}