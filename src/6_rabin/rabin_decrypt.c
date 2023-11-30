#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "modular_ops.h"
#include "primes.h"
#include "ciphers/rabin.h"


int main(int argc, char **argv) {

    rabin_private_key privkey;
    rabin_ciphertext enc;
    
    if (argc < 4) {
        //printf("Encrypt a message with the Rabin quadratic encryption scheme\n");
        printf("Usage:\n\t%s <p> <q> <C>\n", argv[0]);
        printf("p,q\t private key Blum primes\n");
        printf("C\t encrypted message\n");
        printf("All parameters must be unsigned integers in range (1 ... 2^64-1)\n");
        return(-1);

    }

    sscanf(argv[1],"%lu",&(privkey.p));
    sscanf(argv[2],"%lu",&(privkey.q));
    sscanf(argv[3],"%lu",&(enc.message));

    if (!is_prime(privkey.p,64) || (privkey.p % 4) != 3) {
        fprintf(stderr,"[ERR] %lu is not a Blum prime number.\n",privkey.p);
        exit(0x101);
    }

    if (!is_prime(privkey.q,64) || (privkey.q % 4) != 3) {
        fprintf(stderr,"[ERR] %lu is not a Blum prime number.\n",privkey.q);
        exit(0x102);
    }


    /* printf("Initialized a Rabin cryptosystem with\n");
    printf("Encryption key:\t (n)    (%lu) \t(0x%016lx)\n", pubkey.n, pubkey.n);
    printf("Decryption key:\t (p,q)  (%lu,%lu) \t(0x%08lx,0x%08lx)\n", privkey.p,privkey.q, privkey.p,privkey.q);
    printf("Private key Bézout identity: ap + bq = (%ld * %lu) + (%ld * %lu) = 1\n", a,privkey.p,b,privkey.q);
    printf("------------------------------------------\n"); */
    printf("Ciphertext: %lu\n",enc.message);

    // DECRYPT
    uint64_t x[4];
    rabin_decryptions(enc.message, privkey, x);
    printf("Possible decryptions: {%lu, %lu, %lu, %lu}\n",x[0],x[1],x[2],x[3]);


    return 0;
}
