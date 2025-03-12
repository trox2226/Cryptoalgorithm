#include <stdio.h>
#include <wchar.h>
#include <locale.h>

// �ж��Ƿ�Ϊ��д��ĸ
int is_upper(wchar_t c) {
    return c >= L'A' && c <= L'Z';
}

// �ж��Ƿ�ΪСд��ĸ
int is_lower(wchar_t c) {
    return c >= L'a' && c <= L'z';
}

// �ж��Ƿ�Ϊ��ĸ
int is_alpha(wchar_t c) {
    return is_upper(c) || is_lower(c);
}

// ���ܺ���
wchar_t encrypt(wchar_t c, int shift) {
    if (is_upper(c)) {
        return L'A' + (c - L'A' + shift + 26) % 26;
    }
    else if (is_lower(c)) {
        return L'a' + (c - L'a' + shift + 26) % 26;
    }
    // �������ַ����������ĺͷ��ţ����� Unicode ��λ
    return c + shift;
}

// ���ܺ���
wchar_t decrypt(wchar_t c, int shift) {
    if (is_upper(c)) {
        return L'A' + (c - L'A' - shift + 26) % 26;
    }
    else if (is_lower(c)) {
        return L'a' + (c - L'a' - shift + 26) % 26;
    }
    // �������ַ����������ĺͷ��ţ����� Unicode ������λ
    return c - shift;
}