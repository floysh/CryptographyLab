#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "modular_ops.h"
#include "primes.h"
#include "ciphers/rabin.h"


int main(int argc, char **argv) {
    rabin_private_key privkey;
    rabin_public_key pubkey;
    
    srand(time(0));

    // Print help text when invoked with -h or --help
    if (argc == 2 && (strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0)) {
        printf("Usage:\n\t%s [p] [q]\n", argv[0]);
            printf("p,q\t Blum primes\n");
            printf("All parameters must be unsigned integers in range (1 ... 2^63-1)\n");
            exit(-1);
    }

    // 
    // KEY GENERATION
    //
    privkey.p = blum_primegen();
    privkey.q = blum_primegen();

    // Override with cmd arguments, if any
    switch (argc) {
        case 3:
            sscanf(argv[2],"%lu",&privkey.q);
        case 2:
            sscanf(argv[1],"%lu",&privkey.p);
        default:
            break;
    }

    pubkey.n = privkey.p*privkey.q;

    if (!is_prime(privkey.p,64) || (privkey.p % 4) != 3) {
        fprintf(stderr,"[ERR] %lu is not a Blum prime number.\n",privkey.p);
        exit(0x101);
    }

    if (!is_prime(privkey.q,64) || (privkey.q % 4) != 3) {
        fprintf(stderr,"[ERR] %lu is not a Blum prime number.\n",privkey.q);
        exit(0x102);
    }


    printf("Initialized a Rabin cryptosystem with\n");
    printf("Encryption key:\t (n)    (%lu) \t(0x%016lx)\n", pubkey.n, pubkey.n);
    printf("Decryption key:\t (p,q)  (%lu,%lu) \t(0x%08lx,0x%08lx)\n", privkey.p,privkey.q, privkey.p,privkey.q);


}