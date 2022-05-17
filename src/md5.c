#include "md5.h"

#include <limits.h>  // ULLONG_MAX
#include <math.h>    // fabs, sin
#include <stdio.h>
#include <stdlib.h>  // malloc, free
#include <string.h>  // memcpy, memset

#define DEBUG 0

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned char uint8;
typedef char bool;

// init () 检测运行环境
bool init(uint64 len);
// getPadLen () 计算填充后的信息长度
uint64 getPadLen(uint64 len);
// leftrotate () 32 位数据循环左移
uint32 leftrotate(uint32 src, uint8 cx);

// MD5 算法
void MD5(unsigned char *output, const unsigned char *input,
         const unsigned long long len_input) {
    // ****** 检测环境 ******
    if (init(len_input) != 1) {
        return;
    }
    // ****** 函数初始化 ******
    uint32 K[64];
    for (int i = 0; i < 64; ++i) {
        K[i] = (uint32)floor((((uint64)1) << 32) * fabs(sin(i + 1)));
    }
    const uint8 s[4][4] = {
        {7, 12, 17, 22}, {5, 9, 14, 20}, {4, 11, 16, 23}, {6, 10, 15, 21}};

    // ****** 填充 ******
    uint64 lenm = getPadLen(len_input);
    unsigned char *message =
        (unsigned char *)malloc(lenm * sizeof(unsigned char));
    memset(message, 0, lenm);
    memcpy(message, input, len_input);
    message[len_input] = 0x80;
    uint64 lenBit = len_input << 3;  // len_in * 8 表示比特数目
    memcpy(message + lenm - 8, &lenBit, 8);
#if DEBUG
    printf("pading = ");
    for (int i = 0; i < lenm; ++i) {
        printf("%02x", message[i]);
    }
    putchar(10);
#endif

    // ****** 缓冲区初始化 ******
    uint32 a0 = 0x67452301;
    uint32 b0 = 0xEFCDAB89;
    uint32 c0 = 0x98BADCFE;
    uint32 d0 = 0x10325476;

    // ****** 循环压缩 ******
    for (int k = 0; k < lenm; k += 64) {
        uint32 M[16];

        memcpy(&M, message + k, 64);
#if DEBUG
        printf("M = ");
        for (int i = 0; i < 16; ++i) {
            printf("%08x", M[i]);
        }
        putchar(10);
#endif
        uint32 A = a0, B = b0, C = c0, D = d0;

        for (int i = 0; i < 64; ++i) {
            uint32 F;
            int g;
            if (0 <= i && i < 16) {
                F = (B & C) | ((~B) & D);
                g = i;
            } else if (16 <= i && i < 32) {
                F = (D & B) | ((~D) & C);
                g = (5 * i + 1) % 16;
            } else if (32 <= i && i < 48) {
                F = B ^ C ^ D;
                g = (3 * i + 5) % 16;
            } else if (48 <= i && i < 64) {
                F = C ^ (B | (~D));
                g = (7 * i) % 16;
            }
            F = F + A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + leftrotate(F, s[i / 16][i % 4]);
#if DEBUG
            printf("A = %08x\n", A);
            printf("B = %08x\n", B);
            printf("C = %08x\n", C);
            printf("D = %08x\n", D);
            printf("-----------------------\n");
#endif
        }
        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
#if DEBUG
        printf("a = %08x\n", a0);
        printf("b = %08x\n", b0);
        printf("c = %08x\n", c0);
        printf("d = %08x\n", d0);
#endif
    }

    // ****** 得出结果 ******
    memcpy(output, &a0, 4);
    memcpy(output + 4, &b0, 4);
    memcpy(output + 8, &c0, 4);
    memcpy(output + 12, &d0, 4);
    free(message);
    return;
}

// init () 检测运行环境
bool init(uint64 len) {
    // 检测 long long 数据类型的大小
    if (sizeof(unsigned long long) != 8) {
        printf("期望 unsigned long long 的长度为 8 个字节，期望没有满足");
        return 0;
    }
    // 检测 int 数据类型的大小
    if (sizeof(unsigned int) != 4) {
        printf("期望 unsigned int 的长度为 4 个字节，期望没有满足");
        return 0;
    }
    // 检测输入信息的长度
    if (len > ULLONG_MAX / 8) {
        printf("期望输入信息的长度小于等于为 2 的 64 比特，期望没有满足\n");
        return 0;
    }
    // 检测是否满足小端序
    unsigned char tmp = 0;
    unsigned int mask = 0x12345678;
    memcpy(&tmp, &mask, 1);
    if (tmp == 0x12) {
        printf("期望环境为小端序，期望没有满足");
        return 0;
    } else if (tmp != 0x78) {
        printf("检测端序出错");
        return 0;
    }
    return 1;
}

// 计算填充后的信息长度
uint64 getPadLen(uint64 len) {
    int FillSize = 448 - ((len * 8) % 512);
    if (FillSize <= 0) {
        FillSize += 512;
    }
    unsigned int FSbyte = (unsigned int)FillSize / 8;
    return (len + 8 + FSbyte);
}

// 32 位数据循环左移
uint32 leftrotate(uint32 src, uint8 cx) {
    if (cx > 32) cx %= 32;
    return (src << cx) | (src >> (32 - cx));
}
