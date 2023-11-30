#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/rsa.h"

int main(int argc, char **argv) {
    uint64_t M, C;
    rsa_private_key privkey;
    
    if (argc < 3) {
        //printf("Decrypt RSA ciphertexts encrypted with rsa_encrypt\n");
        printf("Usage:\n\t%s <d> <n> <C>\n", argv[0]);
        printf("d\t decryption key\n");
        printf("n\t modulus\n");
        printf("C\t encrypted message\n");
        printf("All parameters must be unsigned integers in range (1 ... 2^64-1)\n");
        return(-1);
    }

    sscanf(argv[1],"%lu",&(privkey.d));
    sscanf(argv[2],"%lu",&(privkey.n));
    sscanf(argv[3],"%lu",&(C));

    printf("Ciphertext:\t\t%lu (0x%016lu)\n", C, C);
    
    //
    // DECRYPT 
    // C ≡ M^e (mod n)
    //

    M = rsa_decrypt(C, privkey);
    printf("Message:\t\t%lu (0x%016lx)\n", M, M);

}