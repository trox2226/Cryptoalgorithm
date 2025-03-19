#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 2
#define LENGTH 800

// �ж��Ƿ���
int gcd(int a, int b) {
    int temp = 0;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// ������,�������
void algofun(int key[MAX][MAX], int plaintext[MAX], int ciphertext[MAX]) {
    for (int i = 0; i < MAX; i++) {
        ciphertext[i] = 0;
        for (int j = 0; j < MAX; j++) {
            ciphertext[i] += key[i][j] * plaintext[j];
        }
        ciphertext[i] %= 26;
    }
}

// ��������
int inverseMatrix(int key[MAX][MAX], int inverse[MAX][MAX]) {
    int det = key[0][0] * key[1][1] - key[0][1] * key[1][0];
    det = (det % 26 + 26) % 26;

    if (gcd(det, 26) != 1) {
        return 0; // ���󲻿���
    }

    int det_inverse;
    for (int i = 0; i < 26; i++) {
        if ((det * i) % 26 == 1) {
            det_inverse = i;
            break;
        }
    }

    inverse[0][0] = (key[1][1] * det_inverse) % 26;
    inverse[0][1] = (-key[0][1] * det_inverse) % 26;
    inverse[1][0] = (-key[1][0] * det_inverse) % 26;
    inverse[1][1] = (key[0][0] * det_inverse) % 26;

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            inverse[i][j] = (inverse[i][j] + 26) % 26;
        }
    }
    return 1; // �������
}

// ���ܺ���
void encrypt(char plaintext[], int key[MAX][MAX], char ciphertext[]) {
    char filter_plain[LENGTH];// ȥ��������ź�
    int filter_index = 0;// ����

    int case_info[LENGTH];// ��¼�ַ���Сд

    int special_pos[LENGTH];// �������λ��
    int special_count = 0;
    // �����Ĺ��ˣ�����д
    for (int i = 0; i < strlen(plaintext); i++) {
        unsigned char c = (unsigned char)plaintext[i];
        if (isalpha(c)) {
            case_info[filter_index] = islower(c);
            filter_plain[filter_index++] = toupper(c);

        }
        else {
            special_pos[special_count++] = i;
        }
    }
    filter_plain[filter_index] = '\0';

    // �����˺���ַ�������
    if (filter_index == 0) {
        printf("������û����Ч����ĸ�ַ����޷����м��ܡ�\n");
        ciphertext[0] = '\0';
        return;
    }

    int length = strlen(filter_plain);
    int padding = MAX - length % MAX;
    if (padding < MAX) {
        for (int i = 0; i < padding; i++) {
            filter_plain[length + i] = 'X';
            case_info[length + i] = 0;

        }
        filter_plain[length + padding] = '\0';
    }

    // ת��Ϊ����
    int plain_vector[MAX];
    int cipher_vector[MAX];
    int index = 0;
    char temp_cipher[LENGTH];
    for (int i = 0; i < strlen(filter_plain); i += MAX) {
        for (int j = 0; j < MAX; j++) {
            plain_vector[j] = filter_plain[i + j] - 'A';
        }
        algofun(key, plain_vector, cipher_vector);
        for (int j = 0; j < MAX; j++) {
            temp_cipher[index] = cipher_vector[j] + 'A';
            if (case_info[index]) {
                temp_cipher[index] = tolower(temp_cipher[index]);
            }
            index++;
        }
    }
    temp_cipher[index] = '\0';

    // ��������Ų������
    index = 0;
    int cipher_index = 0;
    for (int i = 0; i < strlen(plaintext); i++) {
        if (cipher_index < special_count && i == special_pos[cipher_index]) {
            ciphertext[index++] = plaintext[i];
            cipher_index++;
        }
        else {
            ciphertext[index++] = temp_cipher[i - cipher_index];
        }
    }
    ciphertext[index] = '\0';

}

// ���ܺ���
void decrypt(char ciphertext[], int key[MAX][MAX], char plaintext[]) {
    char filter_cipher[LENGTH];// ȥ��������ź�
    int filter_index = 0;// ����

    int case_info[LENGTH];// ��¼�ַ���Сд

    int special_pos[LENGTH];// �������λ��
    int special_count = 0;
    // �����Ĺ��ˣ�����д
    for (int i = 0; i < strlen(ciphertext); i++) {
        unsigned char c = (unsigned char)ciphertext[i];
        if (isalpha(c)) {
            case_info[filter_index] = islower(c);
            filter_cipher[filter_index++] = toupper(c);

        }
        else {
            special_pos[special_count++] = i;
        }
    }
    filter_cipher[filter_index] = '\0';

    int inverse[MAX][MAX];
    if (!inverseMatrix(key, inverse)) {
        printf("���󲻿��棬�޷����ܡ�\n");
        plaintext[0] = '\0';
        return;
    }

    int length = strlen(filter_cipher);
    int cipher_vector[MAX];
    int plain_vector[MAX];
    int index = 0;
    char temp_plain[LENGTH];
    for (int i = 0; i < strlen(filter_cipher); i += MAX) {
        for (int j = 0; j < MAX; j++) {
            cipher_vector[j] = filter_cipher[i + j] - 'A';
        }
        algofun(inverse, cipher_vector, plain_vector);
        for (int j = 0; j < MAX; j++) {
            temp_plain[index] = plain_vector[j] + 'A';
            if (case_info[index]) {
                temp_plain[index] = tolower(temp_plain[index]);
            }
            index++;
        }
    }
    temp_plain[index] = '\0';

    // ��������Ų������
    index = 0;
    int plain_index = 0;
    for (int i = 0; i < strlen(ciphertext); i++) {
        if (plain_index < special_count && i == special_pos[plain_index]) {
            plaintext[index++] = ciphertext[i];
            plain_index++;
        }
        else {
            plaintext[index++] = temp_plain[i - plain_index];
        }
    }
    plaintext[index] = '\0';

}

int main() {
    char plaintext[LENGTH];
    char ciphertext[LENGTH];
    char decryptedtext[LENGTH];
    int key[MAX][MAX];

    // ���û�������Կ����
    while (1) {
        printf("������һ�� 2x2 ����Կ���󣨰������룬ÿ�������������ÿո�ָ�����\n");
        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {
                if (scanf("%d", &key[i][j]) != 1) {
                    printf("������Ч��������������\n");
                    while (getchar() != '\n'); // ������뻺����
                    return 1;
                }
            }
        }

        // �������Ƿ���棨����ʽ�� 26 ���ʣ�
        int det = key[0][0] * key[1][1] - key[0][1] * key[1][0];
        det = (det % 26 + 26) % 26;
        if (gcd(det, 26) == 1) {
            break;
        }
        printf("����ľ��󲻿��棬����������һ������ʽ�� 26 ���ʵľ���\n");
    }

    // ���ĵ� scanf ���µĻ��з�
    while (getchar() != '\n');

    printf("����������: ");
    if (fgets(plaintext, LENGTH, stdin) == NULL) {
        printf("�������\n");
        return 1;
    }
    // ȥ�� fgets ��ȡ�Ļ��з�
    plaintext[strcspn(plaintext, "\n")] = 0;

    encrypt(plaintext, key, ciphertext);
    if (strlen(ciphertext) > 0) {
        printf("���ܺ������: %s\n", ciphertext);
        decrypt(ciphertext, key, decryptedtext);
        printf("���ܺ������: %s\n", decryptedtext);
    }

    return 0;
}