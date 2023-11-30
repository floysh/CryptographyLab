#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "primes.h"

#define MAX_ITER 64

int main(int argc, char **argv) {
    uint64_t p;
    char *buffer = malloc(sizeof(uint64_t));
    memset(buffer,0,sizeof(uint64_t));

    if (argc < 2) {
        printf("prime: ");
        fgets(buffer, sizeof(uint64_t), stdin);
        sscanf(buffer,"%lu",&p);
    }
    else {
        sscanf(argv[1],"%lu",&p);
    }

    // Miller-Rabin probabilistic primality test
    // See src/lib/primes.c for the actual implementation
    if (! is_prime(p, MAX_ITER)) printf("%16lu is not prime\n", p);
    else printf("%16lu is (probably) prime\n", p);

}