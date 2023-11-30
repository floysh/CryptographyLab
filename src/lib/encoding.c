#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "modular_ops.h"
#include "encoding.h"


// Returns the number of blocks required to encode
// a string
unsigned count_blocks(unsigned char *msg) {
    unsigned padding_required = (strlen((char*)msg) + 1) % K;
    unsigned block_count = (strlen((char*)msg)+1)/K + (padding_required > 0);
    return block_count;
}


// Encodes K letters from the buffer into a number
// return the encoded k-gram
uint64_t encode_kgram(unsigned char *buffer) {
    uint64_t M = 0;
    for (unsigned j=0; j < K; j++) {
        M += buffer[j] * ipow(N,j);
    }
    return M;
}


// Encodes a string message
// returns the number of k-grams written
unsigned encode(unsigned char *msg, uint64_t *encoded_buffer) {
    unsigned block_count = count_blocks(msg);

    for (unsigned i=0; i < block_count; i++) {
        uint64_t M = 0;

        M = encode_kgram((msg+(i*K)));

        encoded_buffer[i] = M;
    }

    return block_count;

}


// Decodes a single k-gram into a readable text
// returns the number of char extracted
unsigned decode_kgram(uint64_t M, unsigned char* buffer) {
    unsigned j = 0;
    unsigned char c;
    //msg = 0x0;
    while (M > 0 && j < K) {
        c = M % N;
        M = M / N;

        buffer[j] = (unsigned char) c;
        //msg ^= c << 2*(K-j-1);
        //printf("%08x == %08x", c,);
        j++;
    }
    return j;
}


// Decodes a full buffer
// returns message length
unsigned decode(uint64_t *encoded_buffer, unsigned char *msg, unsigned buff_size) {
    unsigned j = 0;
    for (unsigned i=0; i<buff_size && j < MSG_LEN; i++) {
        j += decode_kgram(encoded_buffer[i], msg+j);
    }
    return j;
}