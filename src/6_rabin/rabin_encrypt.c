#include <stdio.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/rabin.h"


int main(int argc, char **argv) {

    uint64_t M=0;
    rabin_public_key pubkey;
    rabin_ciphertext enc;
    
    if (argc < 3) {
        //printf("Encrypt a message with the Rabin quadratic encryption scheme\n");
        printf("Usage:\n\t%s <n> <M>\n", argv[0]);
        printf("n public key\n");
        printf("M\t message\n");
        printf("All parameters must be unsigned integers in range (1 ... 2^64-1)\n");
        return(-1);

    }

    sscanf(argv[1],"%lu",&(pubkey.n));
    sscanf(argv[2],"%lu",&(M));

    /* printf("Initialized a Rabin cryptosystem with\n");
    printf("Encryption key:\t (n)    (%lu) \t(0x%016lx)\n", pubkey.n, pubkey.n);
    printf("Message: %lu\n",M); */

    // ENCRYPT
    printf("Message: %lu\n",M);
    enc = rabin_encrypt(M,pubkey);
    printf("Ciphertext: %lu\n",enc.message);

    
    return 0;
}
