#include <stdio.h>
#include <string.h>

// ��ʼ��S��
void rc4_init(unsigned char* s, unsigned char* key, unsigned long key_length) {
    int i, j = 0;
    unsigned char temp;

    for (i = 0; i < 256; i++) {
        s[i] = i;
    }

    for (i = 0; i < 256; i++) {
        j = (j + s[i] + key[i % key_length]) % 256;
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

// RC4�ӽ��ܺ���
void rc4_crypt(unsigned char* s, unsigned char* data, unsigned long data_length) {
    int i = 0, j = 0, k;
    unsigned char temp;

    for (k = 0; k < data_length; k++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
        data[k] ^= s[(s[i] + s[j]) % 256];
    }
}

int main() {
    unsigned char s[256];
    unsigned char key[] = "SecretKey";
    unsigned char data[] = "Hello, World!";
    unsigned long key_length = strlen((char*)key);
    unsigned long data_length = strlen((char*)data);

    // ��ʼ��S��
    rc4_init(s, key, key_length);

    // ��������
    rc4_crypt(s, data, data_length);
    printf("Encrypted data: %s\n", data);

    // ���³�ʼ��S��
    rc4_init(s, key, key_length);

    // ��������
    rc4_crypt(s, data, data_length);
    printf("Decrypted data: %s\n", data);

    return 0;
}