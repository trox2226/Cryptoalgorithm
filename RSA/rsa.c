#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// �������Լ��
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// ����ģ������
unsigned long long mod_pow(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Miller-Rabin���Բ���
int miller_rabin(unsigned long long n, int k) {
    if (n == 2 || n == 3) return 1;
    if (n <= 1 || n % 2 == 0) return 0;

    unsigned long long d = n - 1;
    int r = 0;
    while (d % 2 == 0) {
        d /= 2;
        r++;
    }

    for (int i = 0; i < k; i++) {
        unsigned long long a = rand() % (n - 3) + 2;
        unsigned long long x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        int is_composite = 1;
        for (int j = 0; j < r - 1; j++) {
            x = mod_pow(x, 2, n);
            if (x == n - 1) {
                is_composite = 0;
                break;
            }
        }
        if (is_composite) return 0;
    }
    return 1;
}

// ����ָ��λ���Ĵ�����
unsigned long long generate_large_prime(int bits) {
    unsigned long long min = (unsigned long long)1 << (bits - 1);
    unsigned long long max = (unsigned long long)1 << bits;

    while (1) {
        unsigned long long num = rand() % (max - min) + min;
        if (num % 2 == 0) num++;
        if (miller_rabin(num, 5)) return num;
    }
}

// ��չŷ������㷨��ģ��Ԫ
unsigned long long mod_inverse(unsigned long long a, unsigned long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;
    return x1;
}

// ����RSA��Կ��
void generate_keys(unsigned long long* e, unsigned long long* d, unsigned long long* n, int bits) {
    unsigned long long p = generate_large_prime(bits);
    unsigned long long q;
    do {
        q = generate_large_prime(bits);
    } while (q == p);

    *n = p * q;
    unsigned long long phi_n = (p - 1) * (q - 1);

    // ����ʹ�ó�����Կָ��65537
    *e = 65537;
    if (*e >= phi_n || gcd(*e, phi_n) != 1) {
        do {
            *e = rand() % (phi_n - 3) + 2;
        } while (gcd(*e, phi_n) != 1);
    }

    *d = mod_inverse(*e, phi_n);
}

// ���ܺ���
unsigned long long encrypt(unsigned long long plaintext, unsigned long long e, unsigned long long n) {
    return mod_pow(plaintext, e, n);
}

// ���ܺ���
unsigned long long decrypt(unsigned long long ciphertext, unsigned long long d, unsigned long long n) {
    return mod_pow(ciphertext, d, n);
}

int main() {
    srand(time(NULL)); // ��ʼ�������������

    int bits;
    printf("�������������λ��: ");
    scanf("%d", &bits);

    unsigned long long e, d, n;
    generate_keys(&e, &d, &n, bits);

    printf("��Կ (e, n): (%llu, %llu)\n", e, n);
    printf("˽Կ (d, n): (%llu, %llu)\n", d, n);

    unsigned long long plaintext = 123456; // �����������
    printf("\nԭʼ����: %llu\n", plaintext);

    if (plaintext >= n) {
        printf("�������Ĳ��ܳ���ģ��n\n");
        return 1;
    }

    unsigned long long ciphertext = encrypt(plaintext, e, n);
    printf("���ܽ��: %llu\n", ciphertext);

    unsigned long long decrypted = decrypt(ciphertext, d, n);
    printf("���ܽ��: %llu\n", decrypted);

    printf("\n��֤���: %s\n",
        (decrypted == plaintext) ? "�ɹ�" : "ʧ��");

    return 0;
}