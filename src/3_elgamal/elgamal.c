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

    eg_private_key privkey;
    eg_public_key pubkey;

    sscanf(argv[1],"%16lu",&(privkey.p));
    sscanf(argv[2],"%16lu",&(privkey.g));
    sscanf(argv[3],"%16lu",&(privkey.a));

    privkey.a %= (privkey.p-1);

    pubkey.p = privkey.p;
    pubkey.g = privkey.g;
    pubkey.beta = modexp(pubkey.g, privkey.a, privkey.p);

    srand(time(0));

    if (!is_primitive_root(privkey.g, privkey.p)) {
        fprintf(stderr, "[ERR] %lu is not a primitive root mod. %lu\n",privkey.g, privkey.p);
        exit(0x100);
    }

    printf("Initialized an ElGamal cryptosystem with\n");
    printf("Encryption key:\t\t (p,g,β)  (%lu,%lu,%lu) \t(0x%08lx,0x%08lx,0x%08lx)\n",pubkey.p,pubkey.g,pubkey.beta,pubkey.p,pubkey.g,pubkey.beta);
    printf("Decryption key:\t\t (p,g,a)  (%lu,%lu,%lu) \t(0x%08lx,0x%08lx,0x%08lx)\n",privkey.p,privkey.g,privkey.a, privkey.p,privkey.g,privkey.a);
    printf("--------------------------------------\n");

    uint64_t M = 0;
    sscanf(argv[4],"%16lu",&M);
    
    uint64_t k;
    if (argc > 5) {
        sscanf(argv[5],"%16lu",&k);
    }
    else {
        //k =  3; // chosen by a dice roll
        //        // guaranteed to be random
        fprintf(stderr,"[INFO] Nonce value not set. A random value for k will be chosen ");
        do {
            k = (3+rand()) % (privkey.p-1);
        } while(gcd(k,privkey.p-1) > 1 && k < privkey.p-1/4);

        fprintf(stderr,"(k=%lu)\n",k);
    }

    printf("Message: %lu\n",M);

    eg_ciphertext C = elgamal_encrypt(M, pubkey, k);
    printf("Ciphertext: (γ,δ) = (%lu,%lu)\n",C.gamma,C.delta);
    
    M = elgamal_decrypt(C, privkey);
    printf("Decrypted message: %lu\n",M);


    printf("--------------------------------------\n");

    if (gcd(k,privkey.p-1) > 1) {
        fprintf(stderr,"[WARN] k=%lu is not coprime with (p-1)=%lu. Another value will be used instead\n",k,privkey.p-1);
        k = (privkey.p-1) / gcd(k,privkey.p-1);
        while(gcd(k,privkey.p-1) > 1 && k < privkey.p-1/4) {
            k = (3+rand()) % (privkey.p-1);
        }
        fprintf(stderr,"[INFO] Set the nonce to k=%lu\n",k);
    }


    eg_signature S;

    S = elgamal_sign(M, privkey, k);
    printf("Signed message: (M,Σ) = (M,(γ,δ)) = (%lu,(%lu,%lu))\n",M,S.gamma,S.delta);

    printf("Signature verified: ");
    if (elgamal_verify(M,S,pubkey)) {
        printf("YES ✔️\n");
    }
    else {
        printf("NOPE 😓\n");
    }


}