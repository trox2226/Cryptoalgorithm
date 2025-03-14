#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

// ά������������ܽ��ܺ���
// input: ������ַ���
// key: ���ܽ��ܵ���Կ
// output: ������ַ���
// encrypt: ���ܱ�־��1 ��ʾ���ܣ�0 ��ʾ����
void vigenere_cipher(const char* input, const char* key, char* output, int encrypt) {
    int key_len = strlen(key);  // ������Կ�ĳ���
    int i, j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        char output_char = c;

        // ����ַ��Ƿ�Ϊ��д��ĸ
        int is_upper = (c >= 'A' && c <= 'Z');
        // ����ַ��Ƿ�ΪСд��ĸ
        int is_lower = (c >= 'a' && c <= 'z');

        // ���������ĸ����ֱ�ӱ������ַ�
        if (!is_upper && !is_lower) {
            output[i] = c;
            continue;
        }

        // ��ȡ��ǰ��Կ�ַ�
        char k = key[j % key_len];
        j++;

        // ȷ���ַ��Ļ�׼ֵ
        int base = is_upper ? 'A' : 'a';
        // ���ַ�ת��Ϊ 0 - 25 ������
        int num = (is_upper ? c : c - 32) - 'A';
        // ����Կ�ַ�ת��Ϊ 0 - 25 ������
        int key_num = (k >= 'A' && k <= 'Z') ? k - 'A' : (k - 'a');

        // ���ݼ��ܱ�־���м��ܻ���ܲ���
        int result = encrypt ? (num + key_num) : (num - key_num);
        // ȷ������� 0 - 25 �ķ�Χ��
        result = (result + 26) % 26;

        // ��������ַ�
        output_char = result + base;
        output[i] = output_char;
    }

    // ������ַ���ĩβ��ӽ�����
    output[i] = '\0';
}

int main() {
    char plaintext[800];  // ����
    char ciphertext[800]; // ����
    char key[800];        // ��Կ
    char decryptedtext[800]; // ���ܺ���ı�

    printf("���������ģ�\n");
    // ʹ�� fgets ������ȡ�����ո������
    fgets(plaintext, sizeof(plaintext), stdin);
    // ȥ�� fgets ���ܶ�ȡ�Ļ��з�
    plaintext[strcspn(plaintext, "\n")] = 0;

    printf("������Կ��\n");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    // ��������
    vigenere_cipher(plaintext, key, ciphertext, 1);
    // ��������
    vigenere_cipher(ciphertext, key, decryptedtext, 0);

    // �������
    printf("���ܺ���ı���%s\n", ciphertext);
    // ������ܺ���ı�
    printf("���ܺ���ı���%s\n", decryptedtext);

    return 0;
}