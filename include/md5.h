#ifndef MD5_H
#define MD5_H

// MD5 算法
// 可处理数据的最大长度为 2305843009213693951 字节，即 2 的 61 次方 - 1 字节
void MD5(unsigned char *output, const unsigned char *input,
         const unsigned long long len_input);

#endif  // MD5_H
