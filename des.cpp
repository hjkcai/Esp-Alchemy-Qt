#include "des.h"
#include <cstdio>
#include <cstdlib>
#include <string>

using namespace std;

void CConvert::_enBase64Help(unsigned char chasc[3], unsigned char chuue[4])
{
    int i, k = 2;
    unsigned char t = 0;
    for (i = 0; i < 3; i++)
    {
        *(chuue + i) = *(chasc + i) >> k;
        *(chuue + i) |= t;
        t = *(chasc + i) << (8 - k);
        t >>= 2;
        k += 2;
    }
    *(chuue + 3) = *(chasc + 2) & 63;

    for (i = 0; i < 4; i++)
    {
        if ((*(chuue + i) <= 128) && (*(chuue + i) <= 25))
        {
            *(chuue + i) += 65; // 'A'-'Z'
        }
        else if ((*(chuue + i) >= 26) && (*(chuue + i) <= 51))
        {
            *(chuue + i) += 71; // 'a'-'z'
        }
        else if ((*(chuue + i) >= 52) && (*(chuue + i) <= 61))
        {
            *(chuue + i) -= 4; // 0-9
        }
        else if (*(chuue + i) == 62)
        {
            *(chuue + i) = 43; // +
        }
        else if (*(chuue + i) == 63)
        {
            *(chuue + i) = 47; // /
        }
    }
}

void CConvert::_deBase64Help(unsigned char chuue[4], unsigned char chasc[3])
{
    int i, k = 2;
    unsigned char t = 0;

    for (i = 0; i < 4; i++)
    {
        if ((*(chuue + i) >= 65) && (*(chuue + i) <= 90))
            *(chuue + i) -= 65; // 'A'-'Z' -> 0-25
        else if ((*(chuue + i) >= 97) && (*(chuue + i) <= 122))
            *(chuue + i) -= 71; // 'a'-'z' -> 26-51
        else if ((*(chuue + i) >= 48) && (*(chuue + i) <= 57))
            *(chuue + i) += 4; // '0'-'9' -> 52-61
        else if (*(chuue + i) == 43)
            *(chuue + i) = 62; // + -> 62
        else if (*(chuue + i) == 47)
            *(chuue + i) = 63; // / -> 63
        else if (*(chuue + i) == 61)
            *(chuue + i) = 0; // = -> 0  Note: 'A'和'='都对应了0
    }

    for (i = 0; i < 3; i++)
    {
        *(chasc + i) = *(chuue + i) << k;
        k += 2;
        t = *(chuue + i + 1) >> (8 - k);
        *(chasc + i) |= t;
    }
}

string CConvert::enBase64(const char *inbuf, size_t inbufLen)
{
    string outStr;
    unsigned char in[8];
    unsigned char out[8];
    out[4] = 0;
    size_t blocks = inbufLen / 3;

    for (size_t i = 0; i < blocks; i++)
    {
        in[0] = inbuf[i * 3];
        in[1] = inbuf[i * 3 + 1];
        in[2] = inbuf[i * 3 + 2];
        _enBase64Help(in, out);
        outStr += out[0];
        outStr += out[1];
        outStr += out[2];
        outStr += out[3];
    }

    if (inbufLen % 3 == 1)
    {
        in[0] = inbuf[inbufLen - 1];
        in[1] = 0;
        in[2] = 0;
        _enBase64Help(in, out);
        outStr += out[0];
        outStr += out[1];
        outStr += '=';
        outStr += '=';
    }

    else if (inbufLen % 3 == 2)
    {
        in[0] = inbuf[inbufLen - 2];
        in[1] = inbuf[inbufLen - 1];
        in[2] = 0;
        _enBase64Help(in, out);
        outStr += out[0];
        outStr += out[1];
        outStr += out[2];
        outStr += '=';
    }
    return string(outStr);
}

string CConvert::enBase64(const string &inbuf)
{
    return CConvert::enBase64(inbuf.c_str(), inbuf.size());
}

int CConvert::deBase64(string src, char *outbuf)
{
    // Break when the incoming base64 coding is wrong
    if ((src.size() % 4) != 0)
    {
        return 0;
    }

    unsigned char in[4];
    unsigned char out[3];

    size_t blocks = src.size() / 4;
    for (size_t i = 0; i < blocks; i++)
    {
        in[0] = src[i * 4];
        in[1] = src[i * 4 + 1];
        in[2] = src[i * 4 + 2];
        in[3] = src[i * 4 + 3];
        _deBase64Help(in, out);
        outbuf[i * 3]   = out[0];
        outbuf[i * 3 + 1] = out[1];
        outbuf[i * 3 + 2] = out[2];
    }

    int length = src.size() / 4 * 3;
    if (src[src.size() - 1] == '=')
    {
        length--;
        if (src[src.size() - 2] == '=')
        {
            length--;
        }
    }

    return length;
}

string CConvert::deBase64(string src)
{
    char *buf = new char[src.length() * 2];
    int len = deBase64(src, buf);
    buf[len] = '\0';
    string result = string(buf, len);
    delete[] buf;
    return result;
}

//////////////////////////////////////////////////////////////////////

#define GET_UINT32(n,b,i)                                       \
{                                                               \
    (n) = (uint32) ((uint8 *) b)[(i)]                           \
      | (((uint32) ((uint8 *) b)[(i)+1]) <<  8)                 \
      | (((uint32) ((uint8 *) b)[(i)+2]) << 16)                 \
      | (((uint32) ((uint8 *) b)[(i)+3]) << 24);                \
}

#define PUT_UINT32(n,b,i)                                       \
{                                                               \
    (((uint8 *) b)[(i)]  ) = (uint8) (((n)      ) & 0xFF);      \
    (((uint8 *) b)[(i)+1]) = (uint8) (((n) >>  8) & 0xFF);      \
    (((uint8 *) b)[(i)+2]) = (uint8) (((n) >> 16) & 0xFF);      \
    (((uint8 *) b)[(i)+3]) = (uint8) (((n) >> 24) & 0xFF);      \
}

void CMD5::md5_starts(struct md5_context *ctx)
{
    ctx->total[0] = 0;
    ctx->total[1] = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
}

void CMD5::md5_process(struct md5_context *ctx, uint8 data[64])
{
    uint32 A, B, C, D, X[16];
    GET_UINT32(X[0],  data,  0);
    GET_UINT32(X[1],  data,  4);
    GET_UINT32(X[2],  data,  8);
    GET_UINT32(X[3],  data, 12);
    GET_UINT32(X[4],  data, 16);
    GET_UINT32(X[5],  data, 20);
    GET_UINT32(X[6],  data, 24);
    GET_UINT32(X[7],  data, 28);
    GET_UINT32(X[8],  data, 32);
    GET_UINT32(X[9],  data, 36);
    GET_UINT32(X[10], data, 40);
    GET_UINT32(X[11], data, 44);
    GET_UINT32(X[12], data, 48);
    GET_UINT32(X[13], data, 52);
    GET_UINT32(X[14], data, 56);
    GET_UINT32(X[15], data, 60);
#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))
#define P(a,b,c,d,k,s,t)                                \
{                                                       \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;           \
}
    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];
#define F(x,y,z) (z ^ (x & (y ^ z)))
    P(A, B, C, D,  0,  7, 0xD76AA478);
    P(D, A, B, C,  1, 12, 0xE8C7B756);
    P(C, D, A, B,  2, 17, 0x242070DB);
    P(B, C, D, A,  3, 22, 0xC1BDCEEE);
    P(A, B, C, D,  4,  7, 0xF57C0FAF);
    P(D, A, B, C,  5, 12, 0x4787C62A);
    P(C, D, A, B,  6, 17, 0xA8304613);
    P(B, C, D, A,  7, 22, 0xFD469501);
    P(A, B, C, D,  8,  7, 0x698098D8);
    P(D, A, B, C,  9, 12, 0x8B44F7AF);
    P(C, D, A, B, 10, 17, 0xFFFF5BB1);
    P(B, C, D, A, 11, 22, 0x895CD7BE);
    P(A, B, C, D, 12,  7, 0x6B901122);
    P(D, A, B, C, 13, 12, 0xFD987193);
    P(C, D, A, B, 14, 17, 0xA679438E);
    P(B, C, D, A, 15, 22, 0x49B40821);
#undef F
#define F(x,y,z) (y ^ (z & (x ^ y)))
    P(A, B, C, D,  1,  5, 0xF61E2562);
    P(D, A, B, C,  6,  9, 0xC040B340);
    P(C, D, A, B, 11, 14, 0x265E5A51);
    P(B, C, D, A,  0, 20, 0xE9B6C7AA);
    P(A, B, C, D,  5,  5, 0xD62F105D);
    P(D, A, B, C, 10,  9, 0x02441453);
    P(C, D, A, B, 15, 14, 0xD8A1E681);
    P(B, C, D, A,  4, 20, 0xE7D3FBC8);
    P(A, B, C, D,  9,  5, 0x21E1CDE6);
    P(D, A, B, C, 14,  9, 0xC33707D6);
    P(C, D, A, B,  3, 14, 0xF4D50D87);
    P(B, C, D, A,  8, 20, 0x455A14ED);
    P(A, B, C, D, 13,  5, 0xA9E3E905);
    P(D, A, B, C,  2,  9, 0xFCEFA3F8);
    P(C, D, A, B,  7, 14, 0x676F02D9);
    P(B, C, D, A, 12, 20, 0x8D2A4C8A);
#undef F

#define F(x,y,z) (x ^ y ^ z)
    P(A, B, C, D,  5,  4, 0xFFFA3942);
    P(D, A, B, C,  8, 11, 0x8771F681);
    P(C, D, A, B, 11, 16, 0x6D9D6122);
    P(B, C, D, A, 14, 23, 0xFDE5380C);
    P(A, B, C, D,  1,  4, 0xA4BEEA44);
    P(D, A, B, C,  4, 11, 0x4BDECFA9);
    P(C, D, A, B,  7, 16, 0xF6BB4B60);
    P(B, C, D, A, 10, 23, 0xBEBFBC70);
    P(A, B, C, D, 13,  4, 0x289B7EC6);
    P(D, A, B, C,  0, 11, 0xEAA127FA);
    P(C, D, A, B,  3, 16, 0xD4EF3085);
    P(B, C, D, A,  6, 23, 0x04881D05);
    P(A, B, C, D,  9,  4, 0xD9D4D039);
    P(D, A, B, C, 12, 11, 0xE6DB99E5);
    P(C, D, A, B, 15, 16, 0x1FA27CF8);
    P(B, C, D, A,  2, 23, 0xC4AC5665);
#undef F
#define F(x,y,z) (y ^ (x | ~z))
    P(A, B, C, D,  0,  6, 0xF4292244);
    P(D, A, B, C,  7, 10, 0x432AFF97);
    P(C, D, A, B, 14, 15, 0xAB9423A7);
    P(B, C, D, A,  5, 21, 0xFC93A039);
    P(A, B, C, D, 12,  6, 0x655B59C3);
    P(D, A, B, C,  3, 10, 0x8F0CCC92);
    P(C, D, A, B, 10, 15, 0xFFEFF47D);
    P(B, C, D, A,  1, 21, 0x85845DD1);
    P(A, B, C, D,  8,  6, 0x6FA87E4F);
    P(D, A, B, C, 15, 10, 0xFE2CE6E0);
    P(C, D, A, B,  6, 15, 0xA3014314);
    P(B, C, D, A, 13, 21, 0x4E0811A1);
    P(A, B, C, D,  4,  6, 0xF7537E82);
    P(D, A, B, C, 11, 10, 0xBD3AF235);
    P(C, D, A, B,  2, 15, 0x2AD7D2BB);
    P(B, C, D, A,  9, 21, 0xEB86D391);
#undef F
    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
}

void CMD5::md5_update(struct md5_context *ctx, uint8 *input, size_t length)
{
    uint32 left, fill;
    if (! length) return;
    left = (ctx->total[0] >> 3) & 0x3F;
    fill = 64 - left;
    ctx->total[0] += length <<  3;
    ctx->total[1] += length >> 29;
    ctx->total[0] &= 0xFFFFFFFF;
    ctx->total[1] += ctx->total[0] < length << 3;

    if (left && length >= fill)
    {
        memcpy((void *)(ctx->buffer + left), (void *) input, fill);
        md5_process(ctx, ctx->buffer);
        length -= fill;
        input  += fill;
        left = 0;
    }

    while (length >= 64)
    {
        md5_process(ctx, input);
        length -= 64;
        input  += 64;
    }

    if (length)
    {
        memcpy((void *)(ctx->buffer + left), (void *) input, length);
    }
}

static uint8 md5_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void CMD5::md5_finish(struct md5_context *ctx, uint8 digest[16])
{
    uint32 last, padn;
    uint8 msglen[8];
    PUT_UINT32(ctx->total[0], msglen, 0);
    PUT_UINT32(ctx->total[1], msglen, 4);
    last = (ctx->total[0] >> 3) & 0x3F;
    padn = (last < 56) ? (56 - last) : (120 - last);
    md5_update(ctx, md5_padding, padn);
    md5_update(ctx, msglen, 8);
    PUT_UINT32(ctx->state[0], digest,  0);
    PUT_UINT32(ctx->state[1], digest,  4);
    PUT_UINT32(ctx->state[2], digest,  8);
    PUT_UINT32(ctx->state[3], digest, 12);
}

void CMD5::GenerateMD5(unsigned char *buffer, size_t bufferlen)
{
    struct md5_context context;
    md5_starts(&context);
    md5_update(&context, buffer, bufferlen);
    md5_finish(&context, (unsigned char *)m_data);
}

CMD5::CMD5()
{
    for (int i = 0; i < 4; i++)
        m_data[i] = 0;
}

CMD5::CMD5(unsigned long *md5src)
{
    memcpy(m_data, md5src, 16);
}

int _httoi(const char *value)
{
    struct CHexMap
    {
        char chr;
        int value;
    };

    const int HexMapL = 16;
    CHexMap HexMap[HexMapL] =
    {
        {'0', 0}, {'1', 1},
        {'2', 2}, {'3', 3},
        {'4', 4}, {'5', 5},
        {'6', 6}, {'7', 7},
        {'8', 8}, {'9', 9},
        {'a', 10}, {'b', 11},
        {'c', 12}, {'d', 13},
        {'e', 14}, {'f', 15}
    };

    char *mstr = strdup(value);
    char *s = mstr;
    int result = 0;
    if (*s == '0' && *(s + 1) == 'X') s += 2;
    bool firsttime = true;

    while (*s != '\0')
    {
        bool found = false;
        for (int i = 0; i < HexMapL; i++)
        {
            if (*s == HexMap[i].chr)
            {
                if (!firsttime) result <<= 4;
                result |= HexMap[i].value;
                found = true;
                break;
            }
        }
        if (!found) break;
        s++;
        firsttime = false;
    }

    free(mstr);
    return result;
}

CMD5::CMD5(const char *md5src)
{
    if (strcmp(md5src, "") == 0)
    {
        for (int i = 0; i < 4; i++)
            m_data[i] = 0;
        return;
    }

    for (int j = 0; j < 16; j++)
    {
        char buf[10];
        strncpy(buf, md5src, 2);
        md5src += 2;
        ((unsigned char *)m_data)[j] = _httoi(buf);
    }
}

CMD5 CMD5::operator +(CMD5 adder)
{
    unsigned long m_newdata[4];
    for (int i = 0; i < 4; i++)
        m_newdata[i] = m_data[i] ^ (adder.m_data[i]);
    return CMD5(m_newdata);
}

bool CMD5::operator ==(CMD5 cmper)
{
    return (memcmp(cmper.m_data , m_data, 16) == 0);
}

string CMD5::ToString()
{
    char output[33];
    for (int j = 0; j < 16; j++)
    {
        sprintf(output + j * 2, "%02x", ((unsigned char *)m_data)[j]);
    }
    return string(output);
}

//////////////////////////////////////////////////////////////////////////
// initial permutation IP
const static char IP_Table[64] =
{
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

// final permutation IP^-1
const static char IPR_Table[64] =
{
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};

// expansion operation matrix
static const char E_Table[48] =
{
    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
};

// 32-bit permutation function P used on the output of the S-boxes
const static char P_Table[32] =
{
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
    2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};

// permuted choice table (key)
const static char PC1_Table[56] =
{
    57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};

// permuted choice key (table)
const static char PC2_Table[48] =
{
    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

// number left rotations of pc1
const static char LOOP_Table[16] =
{
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// The (in)famous S-boxes
const static char S_Box[8][4][16] =
{
// S1
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
// S2
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
// S3
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
// S4
    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
// S5
    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
// S6
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
// S7
    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
// S8
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

void CDes::Des_Block(char *Out, char *In, bool bEncrypt)
{
    static bool M[64], Tmp[32], *Li = &M[0], *Ri = &M[32];
    ByteToBit(M, In, 64);
    Transform(M, M, IP_Table, 64);
    if (bEncrypt)
    {
        for (int i = 0; i < 16; i++)
        {
            memcpy(Tmp, Ri, 32);
            F_func(Ri, SubKey[i]);
            Xor(Ri, Li, 32);
            memcpy(Li, Tmp, 32);
        }
    }
    else
    {
        for (int i = 15; i >= 0; i--)
        {
            memcpy(Tmp, Li, 32);
            F_func(Li, SubKey[i]);
            Xor(Li, Ri, 32);
            memcpy(Ri, Tmp, 32);
        }
    }
    Transform(M, M, IPR_Table, 64);
    BitToByte(Out, M, 64);
}

void CDes::Des_Full(unsigned char *pdata, size_t len, bool bENCRYPT)
{
    char buf[8];
    size_t start = 0;
    while (start + 8 < len)
    {
        memcpy(buf, pdata + start, 8);
        Des_Block((char *)(pdata + start), (char *)buf, bENCRYPT);
        start += 8;
    }
    memset(buf, 0, 8);
    memcpy(buf, pdata + start, len - start);
    Des_Block((char *)(pdata + start), (char *)buf, bENCRYPT);
}

void CDes::Des_SetKey(const char Key[8])
{
    static bool K[64], *KL = &K[0], *KR = &K[28];
    memcpy(FullKey, Key, 8);
    ByteToBit(K, Key, 64);
    Transform(K, K, PC1_Table, 56);
    for (int i = 0; i < 16; i++)
    {
        RotateL(KL, 28, LOOP_Table[i]);
        RotateL(KR, 28, LOOP_Table[i]);
        Transform(SubKey[i], K, PC2_Table, 48);
    }
}

void CDes::Des_SetKey(const string &Key)
{
    CMD5 md5;
    md5.GenerateMD5((unsigned char *)Key.c_str(), Key.length());
    unsigned long BinKey[2];
    BinKey[0] = md5.m_data[0] ^ md5.m_data[2];
    BinKey[1] = md5.m_data[1] ^ md5.m_data[3];
    Des_SetKey((char *)BinKey);
}

//加密文本
string CDes::Des_EncryptText(string text, string key)
{
    Des_SetKey(key);
    size_t txtlen = text.length();
    char *buf = new char[txtlen + 16];
    memset(buf, 0, txtlen + 16);
    memcpy(buf, text.c_str(), txtlen);
    if (txtlen % 8 != 0)
        txtlen += 8 - (txtlen % 8);
    Des_Full((unsigned char *)buf, txtlen, true);
    string result = CConvert::enBase64(buf, txtlen);
    delete[] buf;
    return result;
}

//解密文本
string CDes::Des_DecryptText(string text, string key)
{
    string result;
    Des_SetKey(key);
    size_t txtlen = text.length();
    char *buf = new char[txtlen + 16];
    memset(buf, 0, txtlen + 16);
    txtlen = CConvert::deBase64(text, buf);
    if (txtlen % 8 != 0)
        txtlen += 8 - (txtlen % 8);
    Des_Full((unsigned char *)buf, txtlen, false);
    result = buf;
    delete[] buf;
    return result;
}

void CDes::F_func(bool In[32], const bool Ki[48])
{
    static bool MR[48];
    Transform(MR, In, E_Table, 48);
    Xor(MR, Ki, 48);
    S_func(In, MR);
    Transform(In, In, P_Table, 32);
}

void CDes::S_func(bool Out[32], bool In[48])
{
    for (char i = 0, j, k; i < 8; i++, In += 6, Out += 4)
    {
        j = (In[0] << 1) + In[5];
        k = (In[1] << 3) + (In[2] << 2) + (In[3] << 1) + In[4];
        ByteToBit(Out, &S_Box[i][j][k], 4);
    }
}

void CDes::Transform(bool *Out, bool *In, const char *Table, int len)
{
    static bool Tmp[256];
    for (int i = 0; i < len; i++)
        Tmp[i] = In[ Table[i] - 1 ];
    memcpy(Out, Tmp, len);
}

void CDes::Xor(bool *InA, const bool *InB, int len)
{
    for (int i = 0; i < len; i++)
        InA[i] ^= InB[i];
}

void CDes::RotateL(bool *In, int len, int loop)
{
    static bool Tmp[256];
    memcpy(Tmp, In, loop);
    memcpy(In, In + loop, len - loop);
    memcpy(In + len - loop, Tmp, loop);
}

void CDes::ByteToBit(bool *Out, const char *In, int bits)
{
    for (int i = 0; i < bits; i++)
        Out[i] = (In[i / 8] >> (i % 8)) & 1;
}

void CDes::BitToByte(char *Out, const bool *In, int bits)
{
    memset(Out, 0, (bits + 7) / 8);
    for (int i = 0; i < bits; i++)
        Out[i / 8] |= In[i] << (i % 8);
}
