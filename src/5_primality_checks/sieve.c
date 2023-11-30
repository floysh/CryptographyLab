#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "primes.h"


int main(int argc, char **argv) {
    uint64_t p,r,d;
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

    // Sieve of Eratosthenes
    // if I can't factorize p, then p is prime
    r = p % 2;
    for (d=3; ((r!=0) && (d*d < p)); d+=2) {
        r = p % d;
    }
    
    if (r == 0) printf("%16lu is not prime\n", p);
    else printf("%16lu is prime\n", p);

}