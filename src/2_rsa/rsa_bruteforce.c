#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "modular_ops.h"

#define PLAINTEXT "ABCD"    // multiple characters to avoid bad guesses
#define MSG_LEN strlen(PLAINTEXT)+1

// Attempt to crack my own RSA cryptosystem implementation
//
// if find the secret phi(n) value
// we can easily derive the private key, solving
// ed ≡ 1 mod (phi(n))
// for e
//
// How can we find phi(n)?
// -> a polynomial time algorithm to do this is not known (yet? no one proved it's impossible though...)
// -> the only known method involves factorizing n=p*q
//
// That's why real world cryptography applications use very large primes
// (1024 to 2048 bit primes => the modulo bit size is 2048 to 4096)

int main(int argc, char **argv) {
    uint64_t p,q,n,e,d,phin;
    char buffer[128];
    memset(buffer,0,128);

    // INITIALIZE
    switch (argc) {
    case 2:
        // Import public key (hex format)
        sscanf(argv[1],"%16lx%16lx",&e,&n);
        break;
    
    case 3:
        // Import public key
        sscanf(argv[1],"%lu",&e);
        sscanf(argv[2],"%lu",&n);
        break;
    
    default:
        fprintf(stdout, "Use: %s  <e> <n>\n",argv[0]);
        fprintf(stdout, "e\t encryption key\n");
        fprintf(stdout, "n\t p*q\n");
        exit(-1);
        break;
    }
    

    printf("Bruteforcing (%lu,%lu) = (0x%016lx 0x%016lx)\n",e,n,e,n);
    
    // Attempt to find the prime factors
    //printf("[INFO] sqrt(n) = %i\n",(int)sqrt(n));

    clock_t elapsed_time = clock();

    uint64_t i=0;
    p = sqrt(n);
    p = p % 2 == 0 ? p-1 : p; //workaround for rounding errors
    for(; p>=3; p-=2) { // Go backwards as the primes should not be too small
        i++;
        if (n % p == 0) {
            q = n / p;
            elapsed_time = clock() - elapsed_time;

            printf("DONE in %.3Lfs (%lu attempts)\n", (long double) elapsed_time/CLOCKS_PER_SEC, i);
            printf("\t%lu = %lu * %lu\n",n,p,q);
            phin = (p-1)*(q-1);
            d = mod_inverse(e, phin/gcd(p-1,q-1));
            printf("PRIVATE KEY (d): %lu (0x%016lx)\n",d,d);
            exit(0);
        }
    }

}