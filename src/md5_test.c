#include "md5.h"

#include <stdio.h>
#include <stdlib.h>

/*

The 128-bit (16-byte) MD5 hashes (also termed message digests) are typically
represented as a sequence of 32 hexadecimal digits. The following demonstrates a
43-byte ASCII input and the corresponding MD5 hash:




MD5("") =
d41d8cd98f00b204e9800998ecf8427e
The MD5 algorithm is specified for messages consisting of any number of bits; it
is not limited to multiples of eight bits (octets, bytes). Some MD5
implementations such as md5sum might be limited to octets, or they might not
support streaming for messages of an initially undetermined length.
*/

int test1() {
    /*
    input: ""
    output: 0x d41d8cd98f00b204e9800998ecf8427e
    */
    unsigned char *des = (unsigned char *)malloc(16 * sizeof(unsigned char));
    printf("input  = \n");

    MD5(des, NULL, 0);
    printf("output = ");
    for (int i = 0; i < 16; ++i) {
        printf("%02x", des[i]);
    }
    putchar(10);
    putchar(10);
    free(des);
    return 0;
}

int test2() {
    /*
    input: "The quick brown fox jumps over the lazy dog"
    output: 0x 9e107d9d372bb6826bd81d3542a419d6
    */
    unsigned char input[] = "The quick brown fox jumps over the lazy dog";
    printf("input  = %s\n", input);
    unsigned char *des = (unsigned char *)malloc(16 * sizeof(unsigned char));
    MD5(des, input, 43);
    printf("output = ");
    for (int i = 0; i < 16; ++i) {
        printf("%02x", des[i]);
    }
    putchar(10);
    putchar(10);
    free(des);

    return 0;
}

int test3() {
    /*
    input: "The quick brown fox jumps over the lazy dog."
    output: 0x e4d909c290d0fb1ca068ffaddf22cbd0
    */

    unsigned char input[] = "The quick brown fox jumps over the lazy dog.";
    printf("input  = %s\n", input);
    unsigned char *des = (unsigned char *)malloc(16 * sizeof(unsigned char));
    MD5(des, input, 44);
    printf("output = ");
    for (int i = 0; i < 16; ++i) {
        printf("%02x", des[i]);
    }
    putchar(10);
    putchar(10);
    free(des);
    return 0;
}