
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "modular_ops.h"
#include "primes.h"
#include "bbs.h"
#include "ciphers/blum-goldwasser.h"

int main(int argc, char **argv) {
    bg_ciphertext enc;
    bg_public_key pubkey;
    
    uint64_t M,s0;
    size_t bitlen = 0;

    if (argc < 3) {
        //printf("Encrypt a message with the Blum-Goldwasser probabilistic encryption scheme\n");
        printf("Usage:\n\t%s <n> <M> [s]\n", argv[0]);
        printf("p,q\t Blum primes\n");
        printf("M\t message\n");
        printf("s\t random seed\n");
        printf("All parameters must be unsigned integers in range (1 ... 2^64-1)\n");
        return(-1);

        //EXAMPLE: ./blum-goldwasser 23 31 26 49
        //EXAMPLE: ./blum-goldwasser 499 547 159201 1027
    }

    sscanf(argv[1],"%lu",&(pubkey.n));
    sscanf(argv[2],"%lu",&(M));

    if (argc >= 4) {
        sscanf(argv[3],"%lu",&(s0));
    }
    else {
        srand(time(0));
        s0 = bbs_gen(pubkey.n, rand());
    }

    if (gcd(s0,pubkey.n) > 1) {
        fprintf(stderr,"[ERR] The random seed s0 must be different from n.\n");
        exit(0x100);
    }

    enc = bg_encrypt(M, pubkey, s0);
    printf("Encrypted message: (C, s_%02lu) = (%lu,%lu) (0x%016lx, 0x%016lx)\n", (bitlen+1), enc.message, enc.sf, enc.message, enc.sf);
}