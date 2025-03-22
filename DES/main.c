#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DES.h"

int main() {
    char input[1024];
    char key_input[9];

    // ��ȡ�û����������
    printf("������Ҫ���ܵ�����: ");
    fgets(input, sizeof(input), stdin);
    // ȥ�� fgets ���ܶ�ȡ�Ļ��з�
    input[strcspn(input, "\n")] = 0;

    // ��ȡ�û��������Կ
    printf("������ 8 �ֽڵ���Կ: ");
    fgets(key_input, sizeof(key_input), stdin);
    // ȥ�� fgets ���ܶ�ȡ�Ļ��з�
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

    // ����
    for (size_t i = 0; i < padded_len; i += block_size) {
        des_encrypt_block(buffer + i, &des_key);
    }

    // �����ܺ������׷�ӱ��浽 content.txt �ļ�
    FILE* file = fopen("content.txt", "a");
    if (file == NULL) {
        perror("�޷����ļ�");
        free(buffer);
        return 1;
    }
    for (size_t i = 0; i < padded_len; i++) {
        fprintf(file, "%02X ", buffer[i]);
    }
    fprintf(file, "\n");
    fclose(file);

    // �ڿ���̨������ܺ������
    printf("���ܺ�: ");
    for (size_t i = 0; i < padded_len; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    // ����
    for (size_t i = 0; i < padded_len; i += block_size) {
        des_decrypt_block(buffer + i, &des_key);
    }

    // ȥ�����
    size_t pad_value = buffer[padded_len - 1];
    if (pad_value > 0 && pad_value <= block_size) {
        int valid = 1;
        for (size_t i = padded_len - pad_value; i < padded_len; i++) {
            if (buffer[i] != pad_value) valid = 0;
        }
        if (valid) padded_len -= pad_value;
    }

    buffer[padded_len] = '\0';

    // �ڿ���̨������ܺ������
    printf("���ܺ�: %s\n", buffer);

    free(buffer);
    return 0;
}