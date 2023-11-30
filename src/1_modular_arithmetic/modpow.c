#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "modular_ops.h"

#define BUFF_SIZE 1024


// Same algorithm as in lib/modular_ops.c
// but prints steps too
uint64_t square_multiply(uint64_t a,uint64_t exp,uint64_t m) {
    uint64_t b = 1, partial = a % m;
    for (int i=0;;i++) {
        if (exp & 1) {// if (least significant bit of exp is 1)
            b = (b * partial) % m;
            printf("[INFO] a^(2^%i): %lu\n",i,partial);
        }
        exp >>= 1; // exp = exp >> 1  <==> exp = exp / 2
        if (!exp) // if (exp == 0)
            break;
        partial *= partial;
        partial %= m;
    }

    return b;
}


int main(int argc, char **argv) {
    uint64_t a,b,exp,m;

    if (argc < 4) {
        char *buffer = malloc(BUFF_SIZE*sizeof(char));
        memset(buffer,0,BUFF_SIZE);

        printf("a: ");
        fgets(buffer, BUFF_SIZE, stdin);
        sscanf(buffer, "%lu",&a);
        
        printf("exp: ");        
        fgets(buffer, BUFF_SIZE, stdin);
        sscanf(buffer, "%lu",&exp);
        
        printf("m: ");
        fgets(buffer, BUFF_SIZE, stdin);
        sscanf(buffer, "%lu",&m);

        free(buffer);
    }
    else {
        a = atoi(argv[1]);
        exp = atoi(argv[2]);
        m = atoi(argv[3]);
        printf("a: %lu,\texp: %lu,\tm:%lu\n",a,exp,m);
    }
    

    if (a < 0 || exp < 0 || m < 0) {
        fprintf(stderr, "Base, exponent and modulus must all be non negative integers");
        exit(-1);
    }

    b = square_multiply(a,exp,m);
    
    printf("\na^exp ≡ %lu^(%lu) ≡ %lu (mod %lu)\n",a,exp,b,m);
}