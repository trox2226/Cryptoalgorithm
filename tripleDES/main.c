#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DES.h"

// ������뻺����
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ��ȡ�û��������Կ��ȷ������Ϊ8�ֽ�
int get_key_input(char* key_input, const char* prompt) {
    while (1) {
        printf("%s", prompt);
        if (fgets(key_input, 9, stdin) == NULL) {
            fprintf(stderr, "��ȡ��Կʱ����\n");
            return 0;
        }
        key_input[strcspn(key_input, "\n")] = 0;
        clear_input_buffer();

        if (strlen(key_input) == 8) {
            return 1;
        }
        else {
            fprintf(stderr, "��Կ���ȱ���Ϊ 8 �ֽڣ����������롣\n");
        }
    }
}

// ���ܺ���
void encrypt_buffer(unsigned char* buffer, size_t padded_len, TRIPLE_DES_KEY* triple_des_key) {
    size_t block_size = 8;
    for (size_t i = 0; i < padded_len; i += block_size) {
        triple_des_encrypt_block(buffer + i, triple_des_key);
    }
}

// ���ܺ���
void decrypt_buffer(unsigned char* buffer, size_t padded_len, TRIPLE_DES_KEY* triple_des_key) {
    size_t block_size = 8;
    for (size_t i = 0; i < padded_len; i += block_size) {
        triple_des_decrypt_block(buffer + i, triple_des_key);
    }
}

// ȥ�����
size_t remove_padding(unsigned char* buffer, size_t padded_len, size_t block_size) {
    size_t pad_value = buffer[padded_len - 1];
    if (pad_value > 0 && pad_value <= block_size) {
        int valid = 1;
        for (size_t i = padded_len - pad_value; i < padded_len; i++) {
            if (buffer[i] != pad_value) {
                valid = 0;
                break;
            }
        }
        if (valid) {
            padded_len -= pad_value;
        }
    }
    return padded_len;
}

// �����ܺ������д���ļ�
int write_encrypted_to_file(unsigned char* buffer, size_t padded_len, const char* filename) {
    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        perror("�޷����ļ�");
        return 0;
    }
    for (size_t i = 0; i < padded_len; i++) {
        if (fprintf(file, "%02X ", buffer[i]) < 0) {
            perror("д���ļ�ʱ����");
            fclose(file);
            return 0;
        }
    }
    if (fprintf(file, "\n") < 0) {
        perror("д���ļ�ʱ����");
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

int main() {
    char input[1024];
    char key_input1[9], key_input2[9], key_input3[9];

    // ��ȡ�û����������
    printf("������Ҫ���ܵ�����: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "��ȡ����ʱ����\n");
        return 1;
    }
    input[strcspn(input, "\n")] = 0;

    // ��ȡ������Կ
    if (!get_key_input(key_input1, "�������һ�� 8 �ֽڵ���Կ: ")) {
        return 1;
    }
    if (!get_key_input(key_input2, "������ڶ��� 8 �ֽڵ���Կ: ")) {
        return 1;
    }
    if (!get_key_input(key_input3, "����������� 8 �ֽڵ���Կ: ")) {
        return 1;
    }

    unsigned char* plaintext = (unsigned char*)input;
    size_t text_len = strlen((char*)plaintext);
    size_t block_size = 8;
    size_t pad = block_size - (text_len % block_size);
    pad = pad ? pad : block_size;
    size_t padded_len = text_len + pad;

    // �����ڴ沢����Ƿ�ɹ�
    unsigned char* buffer = malloc(padded_len);
    if (buffer == NULL) {
        fprintf(stderr, "�ڴ����ʧ��\n");
        return 1;
    }
    memcpy(buffer, plaintext, text_len);
    memset(buffer + text_len, pad, pad);

    unsigned char key1[8], key2[8], key3[8];
    memcpy(key1, key_input1, sizeof(key1));
    memcpy(key2, key_input2, sizeof(key2));
    memcpy(key3, key_input3, sizeof(key3));

    TRIPLE_DES_KEY triple_des_key;
    generate_subkeys(key1, &triple_des_key.key1);
    generate_subkeys(key2, &triple_des_key.key2);
    generate_subkeys(key3, &triple_des_key.key3);

    // ����
    encrypt_buffer(buffer, padded_len, &triple_des_key);

    // �����ܺ������׷�ӱ��浽 content.txt �ļ�
    if (!write_encrypted_to_file(buffer, padded_len, "content.txt")) {
        free(buffer);
        return 1;
    }

    // �ڿ���̨������ܺ������
    printf("���ܺ�: ");
    for (size_t i = 0; i < padded_len; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    // ����
    decrypt_buffer(buffer, padded_len, &triple_des_key);

    // ȥ�����
    padded_len = remove_padding(buffer, padded_len, block_size);
    buffer[padded_len] = '\0';

    // �ڿ���̨������ܺ������
    printf("���ܺ�: %s\n", buffer);

    free(buffer);
    return 0;
}