#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/rsa.h"

int main(int argc, char **argv) {
    uint64_t M, C;
    rsa_public_key pubkey;
    
    if (argc < 4) {
        //printf("Encrypt a message with RSA\n");
        printf("Usage:\n\t%s <e> <n> <M>\n", argv[0]);
        printf("e\t encryption key\n");
        printf("n\t modulus\n");
        printf("M\t message\n");
        printf("All parameters must be unsigned integers in range (1 ... 2^64-1)\n");
        return(-1);
    }

    sscanf(argv[1],"%lu",&(pubkey.e));
    sscanf(argv[2],"%lu",&(pubkey.n));
    sscanf(argv[3],"%lu",&(M));

    printf("Message:\t\t%lu (0x%016lx)\n", M, M);
    
    //
    // ENCRYPT 
    // C ≡ M^e (mod n)
    //

    C = rsa_encrypt(M, pubkey);
    printf("Ciphertext:\t\t%lu (0x%016lu)\n", C, C);

}