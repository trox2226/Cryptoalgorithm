#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <cstring>  // 添加strlen用于处理特殊字符
#include <cmath>    // 添加sqrt用于Hill密钥长度处理

enum class Algorithm {
    caesar,       // 凯撒密码：1-25的位移值
    vigenere,     // 维吉尼亚密码：任意长度字母串（通常1-20）
    hill,         // 希尔密码：数字矩阵（需为平方数长度）
    DES,          // DES：8字节（64位，含8位校验位，有效56位）
    tripleDES,    // 3DES：24字节（192位）
    AES,          // AES：16/24/32字节（128/192/256位）
    RSA           // RSA：密钥对，此处生成模数长度（1024/2048/4096位）
};

const char upper[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char lower[27] = "abcdefghijklmnopqrstuvwxyz";
const char digits[11] = "0123456789";  // 修正数组大小为11以包含结尾\0
const char special_chars[] = "!@#$%^&*()_+";

// 提前声明函数
Algorithm select_algorithm();
std::vector<char> get_charset(Algorithm algo);
int generate_caesar_key();
std::string generate_vigenere_key(int length);
std::string generate_fixed_length_key(Algorithm algo, int length);
int get_valid_length(Algorithm algo);
bool is_perfect_square(int n);

// 使用线程安全的随机数生成器（C++11起）
std::mt19937& get_generator() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    return gen;
}

// 获取算法对应的字符集
std::vector<char> get_charset(Algorithm algo) {
    std::vector<char> chars;
    switch (algo) {
    case Algorithm::caesar:
        chars.assign(digits, digits + 10);
        break;
    case Algorithm::vigenere:
        chars.assign(upper, upper + 26);
        break;
    case Algorithm::hill:
        chars.assign(upper, upper + 26);
        chars.insert(chars.end(), digits, digits + 10);
        break;
    case Algorithm::DES:
    case Algorithm::tripleDES:
    case Algorithm::AES:
        chars.assign(upper, upper + 26);
        chars.insert(chars.end(), lower, lower + 26);
        chars.insert(chars.end(), digits, digits + 10);
        chars.insert(chars.end(), special_chars, special_chars + strlen(special_chars));
        break;
    case Algorithm::RSA:
        return {};
    }
    return chars;
}

int generate_caesar_key() {
    std::uniform_int_distribution<> dis(1, 25);
    return dis(get_generator());
}

std::string generate_vigenere_key(int length) {
    auto chars = std::vector<char>(upper, upper + 26);
    std::uniform_int_distribution<> dis(0, chars.size() - 1);

    std::string key;
    for (int i = 0; i < length; i++) {
        key += chars[dis(get_generator())];
    }
    return key;
}

std::string generate_fixed_length_key(Algorithm algo, int length) {
    auto chars = get_charset(algo);
    std::uniform_int_distribution<> dis(0, chars.size() - 1);

    std::string key;
    for (int i = 0; i < length; i++) {
        key += chars[dis(get_generator())];
    }

    // Hill密码补充校验：确保密钥长度是平方数
    if (algo == Algorithm::hill) {
        int n = static_cast<int>(sqrt(length));
        if (n * n != length) {
            std::cerr << "警告：希尔密码密钥长度应为平方数，自动补充到下一个平方数\n";
            int new_len = (n + 1) * (n + 1);
            for (int i = length; i < new_len; i++) {
                key += chars[dis(get_generator())];
            }
        }
    }
    return key;
}

int get_valid_length(Algorithm algo) {
    int length;
    switch (algo) {
    case Algorithm::caesar:
        return 1;

    case Algorithm::vigenere: {
        std::cout << "请输入维吉尼亚密钥长度（1-20）: ";
        while (!(std::cin >> length) || length < 1 || length > 20) {
            std::cout << "无效长度，请重新输入: ";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
        return length;
    }

    case Algorithm::DES:
        return 8;

    case Algorithm::tripleDES:
        return 24;

    case Algorithm::AES: {
        std::cout << "请输入AES密钥长度（16/24/32字节）: ";
        while (!(std::cin >> length) || (length != 16 && length != 24 && length != 32)) {
            std::cout << "无效长度，请重新输入: ";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
        return length;
    }

    case Algorithm::RSA: {
        std::cout << "请输入RSA密钥长度（1024/2048/4096位）: ";
        while (!(std::cin >> length) || (length != 1024 && length != 2048 && length != 4096)) {
            std::cout << "无效长度，请重新输入: ";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
        return length;
    }

    case Algorithm::hill: {
        std::cout << "请输入希尔密码密钥长度（建议平方数如4/9/16）: ";
        while (!(std::cin >> length) || length < 4 || length > 64) {  // 扩大上限方便处理
            std::cout << "无效长度，请重新输入: ";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }
        return length;
    }

    default:
        return 0;
    }
}

int main() {
    Algorithm algo = select_algorithm();
    std::string key;

    switch (algo) {
    case Algorithm::caesar: {
        int shift = generate_caesar_key();
        key = std::to_string(shift);
        std::cout << "凯撒密码位移密钥: " << key << " (1-25)\n";
        break;
    }
    case Algorithm::vigenere: {
        int length = get_valid_length(algo);
        key = generate_vigenere_key(length);
        std::cout << "维吉尼亚密钥（大写字母）: " << key << "\n";
        break;
    }
    case Algorithm::hill: {
        int length = get_valid_length(algo);
        key = generate_fixed_length_key(algo, length);
        int actual_len = key.length();
        int n = static_cast<int>(sqrt(actual_len));
        std::cout << "希尔密码密钥（" << n << "x" << n << "矩阵）: " << key << "\n";
        std::cout << "注意：实际使用时需要转换为数值矩阵并验证可逆性\n";
        break;
    }
    case Algorithm::DES: {
        key = generate_fixed_length_key(algo, 8);
        std::cout << "DES密钥（8字节）: " << key << "\n";
        break;
    }
    case Algorithm::tripleDES: {
        key = generate_fixed_length_key(algo, 24);
        std::cout << "3DES密钥（24字节）: " << key << "\n";
        break;
    }
    case Algorithm::AES: {
        int length = get_valid_length(algo);
        key = generate_fixed_length_key(algo, length);
        std::cout << "AES密钥（" << length << "字节）: " << key << "\n";
        break;
    }
    case Algorithm::RSA: {
        int length = get_valid_length(algo);
        std::cout << "RSA密钥长度设置为: " << length << "位（需使用专用库生成密钥对）\n";
        break;
    }
    default:
        std::cout << "不支持的算法\n";
    }

    return 0;
}

Algorithm select_algorithm() {
    std::cout << "请选择算法（输入对应数字）：\n"
        << "0: Caesar (1-25数字位移)\n"
        << "1: Vigenere (1-20字母密钥)\n"
        << "2: Hill (建议平方数长度如4/9/16)\n"
        << "3: DES (固定8字节)\n"
        << "4: TripleDES (固定24字节)\n"
        << "5: AES (16/24/32字节)\n"
        << "6: RSA (1024/2048/4096位)\n";

    int choice;
    while (!(std::cin >> choice) || choice < 0 || choice > 6) {
        std::cout << "输入无效，请重新选择（0-6）：";
        std::cin.clear();
        std::cin.ignore(1000, '\n');
    }
    return static_cast<Algorithm>(choice);
}