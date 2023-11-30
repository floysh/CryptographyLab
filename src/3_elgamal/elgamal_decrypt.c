#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "modular_ops.h"
#include "ciphers/elgamal.h"


int main(int argc, char **argv) {

    if (argc < 6) {
        printf("Usage:\n\t%s <p> <g> <a> <γ> <δ>\n", argv[0]);
        printf("p\t prime\n");
        printf("g\t generator (primitive root modulo p)\n");
        printf("a\t private key secret\n");
        printf("γ\t ciphertext gamma\n");
        printf("δ\t ciphertext delta\n");
        printf("All parameters must be positive integers in range (1 ... 2^64-1)\n");
        return(-1);
        //return test_elgamal();
    }

    uint64_t M=0;
    eg_ciphertext C;
    eg_private_key privkey;

    sscanf(argv[1],"%16lu",&(privkey.p));
    sscanf(argv[2],"%16lu",&(privkey.g));
    sscanf(argv[3],"%16lu",&(privkey.a));
    sscanf(argv[4],"%16lu",&(C.gamma));
    sscanf(argv[5],"%16lu",&(C.delta));

    if (!is_primitive_root(privkey.g, privkey.p)) {
        fprintf(stderr, "[ERR] %lu is not a primitive root mod. %lu\n",privkey.g, privkey.p);
        exit(0x100);
    }

    /* printf("Initialized an ElGamal cryptosystem with\n");
    printf("Encryption key:\t\t (p,g,β)  (%lu,%lu,%lu) \t(0x%08lx,0x%08lx,0x%08lx)\n",pubkey.p,pubkey.g,pubkey.beta,pubkey.p,pubkey.g,pubkey.beta);
    printf("Decryption key:\t\t (p,g,a)  (%lu,%lu,%lu) \t(0x%08lx,0x%08lx,0x%08lx)\n",privkey.p,privkey.g,privkey.a, privkey.p,privkey.g,privkey.a);
    printf("--------------------------------------\n"); */

        
    M = elgamal_decrypt(C, privkey);
    printf("Decrypted message: %lu\n",M);



}