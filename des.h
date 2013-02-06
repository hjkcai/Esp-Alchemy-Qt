#ifndef __Alchemy__des__
#define __Alchemy__des__

#include <cstring>
#include <string>
using namespace std;

class CConvert
{
public:
    static void _enBase64Help(unsigned char chasc[3], unsigned char chuue[4]);
    static void _deBase64Help(unsigned char chuue[4], unsigned char chasc[3]);
    static string enBase64(const char *inbuf, size_t inbufLen);
    static string enBase64(const string &inbuf);
    static int deBase64(string src, char *outbuf);
    static string deBase64(string src);
};

struct MD5_DATA
{
    unsigned long data[4];
    bool operator < (const MD5_DATA &p) const
    {
        return memcmp(data, p.data, 4 * sizeof(long)) > 0;
    }
};

class CMD5
{
private:
#define uint8  unsigned char
#define uint32 unsigned long int
    struct md5_context
    {
        uint32 total[2];
        uint32 state[4];
        uint8 buffer[64];
    };
    void md5_starts(struct md5_context *ctx);
    void md5_process(struct md5_context *ctx, uint8 data[64]);
    void md5_update(struct md5_context *ctx, uint8 *input, size_t length);
    void md5_finish(struct md5_context *ctx, uint8 digest[16]);
public:
    // construct a CMD5 from any buffer
    void GenerateMD5(unsigned char *buffer, size_t bufferlen);
    // construct a CMD5
    CMD5();
    // construct a md5src from char *
    CMD5(const char *md5src);
    // construct a CMD5 from a 16 bytes md5
    CMD5(unsigned long *md5src);
    // add a other md5
    CMD5 operator +(CMD5 adder);
    // just if equal
    bool operator ==(CMD5 cmper);
    // to a string
    string ToString();
    unsigned long m_data[4];
};

//////////////////////////////////////////////////////////////////////////
class CDes
{
protected:
    bool SubKey[16][48];
    void F_func(bool In[32], const bool Ki[48]);// f 函数
    void S_func(bool Out[32], bool In[48]);// S 盒代替
    void Transform(bool *Out, bool *In, const char *Table, int len);// 变换
    void Xor(bool *InA, const bool *InB, int len);// 异或
    void RotateL(bool *In, int len, int loop);// 循环左移
    void ByteToBit(bool *Out, const char *In, int bits);// 字节组转换成位组
    void BitToByte(char *Out, const bool *In, int bits);// 位组转换成字节组
public:
    //加密文本,产生base64加密结果
    string Des_EncryptText(string text, string key);
    //解密文本，由base64解密成原始字符串
    string Des_DecryptText(string text, string key);
    // 加/解密 一个连续内存块，要求len必须8倍数
    void Des_Full(unsigned char *pdata, size_t len, bool bENCRYPT);
    // 加/解密 一个8字节块
    void Des_Block(char *Out, char *In, bool bENCRYPT);
    // 设置密钥
    void Des_SetKey(const char Key[8]);
    void Des_SetKey(const string &Key);
    //当前使用的key,即常见的56bit密码
    char FullKey[8];
};

#endif // __Alchemy__des__
