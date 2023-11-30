#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "modular_ops.h"

#define MSG_LEN 1024
#define PADDING_SYMBOL '\0'

#define N 0b11111111    // Alphabet cardinality (255 for 8-bit char)
#define K 8             // Block size

int main(int argc, char **argv) {
    unsigned char message[MSG_LEN];
    uint64_t encoded_message[MSG_LEN];
    unsigned char decoded_message[MSG_LEN];

    uint64_t m = 0;
    
    printf("[INFO] The maximum encodable char is %u\n", N);
    printf("[INFO] N = %u, K = %u\n",N,K);
    printf("[INFO] Encoded messages will be less or equal to %lu\n", (ipow(N,K) - 1));
    printf("[INFO] Number size is %lu bit, can represent up to %lu different messages\n",sizeof(uint64_t)*8, ipow(2,sizeof(uint64_t)*8)-1);
    if ((ipow(N,K) - 1) <= ipow(2,sizeof(uint64_t)*8)-1) {
        printf("[INFO] OK");
    }
    else {
        printf("[WARN] Some messages will overflow the number size!\n");
    }


    printf("\nMessage (max. %i char): ",MSG_LEN);
    memset(message, 0, MSG_LEN);
    fgets(message, MSG_LEN, stdin);
    // Properly terminate input string
    for (int i=0; i < MSG_LEN; i++) {
        if (message[i] == 10) {message[i] = 0; break;}
    }
    printf("[INFO] Message len is %li (w/ string terminator)\n", strlen(message)+1);

    // Align message block size
    int padding_required = (strlen(message) + 1) % K;
    /* if (padding_required)  {
        printf("[INFO] %lu is not divisible by %i, will try to space padding it (%i spaces will be added)\n",strlen(message)+1,K,K-padding_required);
        unsigned lastchar_pos = strlen(message);
        unsigned terminator_pos = lastchar_pos + (K - padding_required);
        for (unsigned i=lastchar_pos; i < terminator_pos; i++) {
            message[i] = PADDING_SYMBOL;
        }
        //printf("[DEBUG] %i is the nwe terminator position\n", terminator_pos);
        message[terminator_pos] = '\0';
        
        printf("[INFO] Message len is now %lu char\n",strlen(message)+1);
    } */



    // Encode in base MAX_CHAR
    unsigned block_count = (strlen(message)+1)/K + (padding_required > 0);
    for (unsigned i=0; i < block_count; i++) {
        m = 0;
        for (unsigned j=0; j < K; j++) {   
            printf("[DEBUG] read char %u (%c) (%u)\n",(i*K+j),message[i*K+j],message[i*K+j]); 
            m += message[i*K+j] * ipow(N,j);
        }
        encoded_message[i] = m;
    }

    printf("[INFO] Message successfully encoded with %u block(s)\n", block_count);

    printf("Encoded: ");
    for (unsigned i=0; i<block_count; i++) {
        printf("%lu ", encoded_message[i]);
    }
    printf("\n");

    // Decode from base MAX_CHAR
    memset(decoded_message,0,MSG_LEN);
    for (unsigned i=0; i < block_count; i++) {
        m = encoded_message[i];
        unsigned j = 0; unsigned char c;
        while (m > 0) {
            c = m % N;
            m = m / N;

            decoded_message[i*K+j] = (unsigned char) c;
            printf("%u ",c);
            j++;
        }
    }

    printf("\n");

    if(decoded_message[strlen(decoded_message)+1] & 1) {
        printf("[WARN] STRING TERMINATOR WAS ERASED!");
    }

    printf("Decoded: %s\n",decoded_message);

}