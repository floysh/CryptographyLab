#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "modular_ops.h"
#include "primes.h"
#include "bbs.h"

int main (int argc, char **argv) {
    //uint64_t p = 499, q = 547; // Two large primes congruent to 3 mod 4
    //uint64_t seed = 19;
    uint64_t p,q,n,seed,num;
    long long unsigned stop = 1;
    
    char *buffer = malloc(sizeof(uint64_t));
    memset(buffer,0,sizeof(uint64_t));

    // INITIALIZE
    if (argc < 4) {
        //printf("Encrypt a message with RSA\n");
        printf("Usage:\n\t%s <p> <q> <seed>\n", argv[0]);
        printf("p,q\t Blum primes\n");
        printf("seed\t random seed\n");
        printf("All parameters must be unsigned integers in range (1 ... 2^64-1)\n");
        return(-1);
    }
    else {
        sscanf(argv[1],"%lu",&p);
        sscanf(argv[2],"%lu",&q);
        sscanf(argv[3],"%lu",&seed);
    }

    // Check if p and q are Blum primes
    while ((! is_prime(p, 64)) && (p % 4 != 3)) {
        fprintf(stderr, "[ERR] p must be a prime number congruent to 3 mod 4 \n");
        printf("p: ");
        fgets(buffer, sizeof(uint64_t), stdin);
        sscanf(buffer, "%lu",&p);
    }
    while ((! is_prime(q, 64)) && (q % 4 != 3)) {
        fprintf(stderr, "[ERR] q must be a prime number congruent to 3 mod 4 \n");
        printf("q: ");
        fgets(buffer, sizeof(uint64_t), stdin);
        sscanf(buffer, "%lu",&q);
    }

    n = p*q;
    seed = seed % n;

    // Check if the user seed is suitable
    while (gcd(n,seed) != 1) {
        fprintf(stderr, "[ERR] The random seed must be coprime with n = p*q \n");
        printf("random seed: ");
        fgets(buffer, sizeof(uint64_t), stdin);
        sscanf(buffer, "%16lu",&seed);
        seed = seed % n;
    }

    printf("Initialized a Blum-Blum-Shub generator with:\n");
    printf("\t(p,q): (%lu,%lu) (0x%08lx,0x%08lx)\n",p,q,p,q);
    printf("\tn = p*q: %lu (0x%016lx)\n",n,n);
    printf("\tseed: %lu\n",seed);
    printf("--------------------------------------\n");


    printf("How many numbers to generate? ");
    fgets(buffer, sizeof(unsigned), stdin);
    sscanf(buffer, "%llu",&stop);
    
    num = seed;
    // BLUM-BLUM-SHUB number Generation
    for (; stop > 0; stop--) {
        num = bbs_gen(n, num);
        printf("%lu\t(0x%016lx)\n",num,num);
    }

    exit(0);

}