#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "modular_ops.h"
#include "ciphers/elgamal.h"


int main(int argc, char **argv) {

    if (argc < 5) {
        printf("Usage:\n\t%s <p> <g> <a> <M> [k]\n", argv[0]);
        printf("p\t prime\n");
        printf("g\t generator (primitive root modulo p)\n");
        printf("a\t private key secret\n");
        printf("M\t message\n");
        printf("k\t random nonce\n");
        printf("All parameters must be positive integers in range (1 ... 2^64-1)\n");
        return(-1);
        //return test_elgamal();
    }

    uint64_t M,k;
    eg_signature S;
    eg_private_key privkey;

    sscanf(argv[1],"%16lu",&(privkey.p));
    sscanf(argv[2],"%16lu",&(privkey.g));
    sscanf(argv[3],"%16lu",&(privkey.a));
    sscanf(argv[4],"%16lu",&(M));

    if (argc > 5) {
        sscanf(argv[5],"%16lu",&k);
    }
    else {
        //k =  3; // chosen by a dice roll
        //        // guaranteed to be random
        fprintf(stderr,"[INFO] Nonce value not set. A random value for k will be chosen ");
        do {
            srand(time(0));
            k = (3+rand()) % (privkey.p-1);
        } while(gcd(k,privkey.p-1) > 1 && k < privkey.p-1/4);

        fprintf(stderr,"(k=%lu)\n",k);
    }

    if (!is_primitive_root(privkey.g, privkey.p)) {
        fprintf(stderr, "[ERR] %lu is not a primitive root mod. %lu\n",privkey.g, privkey.p);
        exit(0x100);
    }

    /* printf("Initialized an ElGamal cryptosystem with\n");
    printf("Encryption key:\t\t (p,g,β)  (%lu,%lu,%lu) \t(0x%08lx,0x%08lx,0x%08lx)\n",pubkey.p,pubkey.g,pubkey.beta,pubkey.p,pubkey.g,pubkey.beta);
    printf("Decryption key:\t\t (p,g,a)  (%lu,%lu,%lu) \t(0x%08lx,0x%08lx,0x%08lx)\n",privkey.p,privkey.g,privkey.a, privkey.p,privkey.g,privkey.a);
    printf("--------------------------------------\n"); */

    S = elgamal_sign(M, privkey, k);
    printf("(M,Σ) = (M,(γ,δ)) = (%lu,(%lu,%lu))\n",M,S.gamma,S.delta);

}