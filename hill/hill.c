#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 2
#define LENGTH 800

// 判断是否互质
int gcd(int a, int b) {
    int temp = 0;
    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 求密文,矩阵相乘
void algofun(int key[MAX][MAX], int plaintext[MAX], int ciphertext[MAX]) {
    for (int i = 0; i < MAX; i++) {
        ciphertext[i] = 0;
        for (int j = 0; j < MAX; j++) {
            ciphertext[i] += key[i][j] * plaintext[j];
        }
        ciphertext[i] %= 26;
    }
}

// 求矩阵的逆
int inverseMatrix(int key[MAX][MAX], int inverse[MAX][MAX]) {
    int det = key[0][0] * key[1][1] - key[0][1] * key[1][0];
    det = (det % 26 + 26) % 26;

    if (gcd(det, 26) != 1) {
        return 0; // 矩阵不可逆
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
    return 1; // 矩阵可逆
}

// 加密函数
void encrypt(char plaintext[], int key[MAX][MAX], char ciphertext[]) {
    char filter_plain[LENGTH];// 去掉特殊符号后
    int filter_index = 0;// 索引

    int case_info[LENGTH];// 记录字符大小写

    int special_pos[LENGTH];// 特殊符号位置
    int special_count = 0;
    // 将明文过滤，纯大写
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

    // 检查过滤后的字符串长度
    if (filter_index == 0) {
        printf("输入中没有有效的字母字符，无法进行加密。\n");
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

    // 转换为数字
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

    // 将特殊符号插回密文
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

// 解密函数
void decrypt(char ciphertext[], int key[MAX][MAX], char plaintext[]) {
    char filter_cipher[LENGTH];// 去掉特殊符号后
    int filter_index = 0;// 索引

    int case_info[LENGTH];// 记录字符大小写

    int special_pos[LENGTH];// 特殊符号位置
    int special_count = 0;
    // 将密文过滤，纯大写
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
        printf("矩阵不可逆，无法解密。\n");
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

    // 将特殊符号插回明文
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

    // 让用户输入密钥矩阵
    while (1) {
        printf("请输入一个 2x2 的密钥矩阵（按行输入，每行两个整数，用空格分隔）：\n");
        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {
                if (scanf("%d", &key[i][j]) != 1) {
                    printf("输入无效，请输入整数。\n");
                    while (getchar() != '\n'); // 清空输入缓冲区
                    return 1;
                }
            }
        }

        // 检查矩阵是否可逆（行列式与 26 互质）
        int det = key[0][0] * key[1][1] - key[0][1] * key[1][0];
        det = (det % 26 + 26) % 26;
        if (gcd(det, 26) == 1) {
            break;
        }
        printf("输入的矩阵不可逆，请重新输入一个行列式与 26 互质的矩阵。\n");
    }

    // 消耗掉 scanf 留下的换行符
    while (getchar() != '\n');

    printf("请输入明文: ");
    if (fgets(plaintext, LENGTH, stdin) == NULL) {
        printf("输入错误。\n");
        return 1;
    }
    // 去除 fgets 读取的换行符
    plaintext[strcspn(plaintext, "\n")] = 0;

    encrypt(plaintext, key, ciphertext);
    if (strlen(ciphertext) > 0) {
        printf("加密后的密文: %s\n", ciphertext);
        decrypt(ciphertext, key, decryptedtext);
        printf("解密后的明文: %s\n", decryptedtext);
    }

    return 0;
}