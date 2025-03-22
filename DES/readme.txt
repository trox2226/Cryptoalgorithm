一、核心数据结构定义
置换表组
IP/IP_INV：初始置换和逆置换，打乱 64 位数据顺序
PC_1/PC_2：密钥置换表（PC_1 将 64 位密钥压缩为 56 位，PC_2 将 56 位压缩为 48 位）
E 扩展表：将 32 位数据扩展为 48 位
P 盒：32 位置换
SBOX：8 个 S 盒，实现非线性替换
密钥结构体
typedef struct {
uint64_t subkeys [16]; // 存储 16 轮子密钥
} DES_KEY;
二、核心算法实现
初始置换（IP 置换）
static uint64_t initial_permutation (uint64_t input) {
// 根据 IP 表重排 64 位数据的每一位
// 作用：打乱数据顺序，增强扩散性
}
逆初始置换（IP^{-1} 置换）
static uint64_t inverse_initial_permutation (uint64_t input) {
// 加密 / 解密最后一步恢复数据顺序
}
子密钥生成
static void generate_subkeys (const unsigned char* key, DES_KEY* des_key) {
// 1. 64 位密钥 → 56 位（PC_1 置换）
// 2. 分成 C0/D0 两部分（各 28 位）
// 3. 16 轮循环左移（根据 key_shifts 表）
// 4. 每次循环后生成 48 位子密钥（PC_2 置换）
// 生成的 subkeys [0]~subkeys [15] 用于加密轮次
}
Feistel 网络轮函数
加密核心处理：
扩展置换 E → 32→48 位
与子密钥异或
S 盒替换 → 48→32 位
P 盒置换 → 32 位重新排列
加密块函数
void des_encrypt_block (unsigned char* block, const DES_KEY* key) {
// 1. 数据分块（8 字节）
// 2. IP 置换
// 3. 16 轮 Feistel 迭代（每轮使用 subkeys [i]）
// 4. 交换左右半区（最后一轮不交换）
// 5. IP^{-1} 置换
}
解密块函数
void des_decrypt_block (unsigned char* block, const DES_KEY* key) {
// 与加密流程相同，但子密钥使用顺序相反（subkeys [15]~subkeys [0]）
}
三、DES 核心原理
整体结构
分组密码：64 位明文→64 位密文
16 轮 Feistel 网络
密钥长度：56 位（实际输入 64 位，含 8 位校验）
Feistel 网络流程
每轮处理步骤：
右半区扩展（E 表）
与子密钥异或
S 盒替换（非线性变换）
P 盒置换
与左半区异或，交换左右半区
S 盒的作用
唯一非线性部件，提供混淆
6 位输入→4 位输出
每个 S 盒是 4x16 的替换表，输入高 / 低两位确定行 / 列
四、代码关键流程
填充处理
main 函数中：
明文长度调整为 8 的倍数
填充值为缺少的字节数（如缺 3 字节则填 0x03）
解密后验证填充有效性
加解密流程
输入明文 → 填充 → 分块加密 → 输出密文 → 分块解密 → 去填充 → 输出明文
五、代码安全注意事项
密钥管理
示例使用固定密钥 "SecretKa"，实际应安全存储密钥
密钥应随机生成，避免弱密钥
填充模式
当前实现为 PKCS#7 填充
需注意填充攻击防护
分组模式
当前为 ECB 模式（不安全）
实际应用应使用 CBC/CFB 等模式
六、典型运行示例
输入：
请输入要加密的明文: HelloDES
输出：
原始文本: HelloDES
十六进制: 48 65 6C 6C 6F 44 45 53
加密后: 5A 8D E5 7A 84 3D 2E 2D
解密后: HelloDES