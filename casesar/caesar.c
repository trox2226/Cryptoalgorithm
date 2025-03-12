#include <stdio.h>
#include <wchar.h>
#include <locale.h>

// 判断是否为大写字母
int is_upper(wchar_t c) {
    return c >= L'A' && c <= L'Z';
}

// 判断是否为小写字母
int is_lower(wchar_t c) {
    return c >= L'a' && c <= L'z';
}

// 判断是否为字母
int is_alpha(wchar_t c) {
    return is_upper(c) || is_lower(c);
}

// 加密函数
wchar_t encrypt(wchar_t c, int shift) {
    if (is_upper(c)) {
        return L'A' + (c - L'A' + shift + 26) % 26;
    }
    else if (is_lower(c)) {
        return L'a' + (c - L'a' + shift + 26) % 26;
    }
    // 对其他字符（包括中文和符号）进行 Unicode 移位
    return c + shift;
}

// 解密函数
wchar_t decrypt(wchar_t c, int shift) {
    if (is_upper(c)) {
        return L'A' + (c - L'A' - shift + 26) % 26;
    }
    else if (is_lower(c)) {
        return L'a' + (c - L'a' - shift + 26) % 26;
    }
    // 对其他字符（包括中文和符号）进行 Unicode 反向移位
    return c - shift;
}