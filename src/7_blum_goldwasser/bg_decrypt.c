
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "modular_ops.h"
#include "primes.h"
#include "ciphers/blum-goldwasser.h"

int main(int argc, char **argv) {
    uint64_t M;
    bg_ciphertext enc;
    bg_private_key privkey;

    if (argc < 4) {
        //printf("Encrypt a message with the Blum-Goldwasser probabilistic encryption scheme\n");
        printf("Usage:\n\t%s <p> <q> <M>\n", argv[0]);
        printf("p,q\t private key (Blum primes)\n");
        printf("M\t message\n");
        printf("s\t random seed\n");
        printf("All parameters must be unsigned integers in range (1 ... 2^64-1)\n");
        return(-1);

        //EXAMPLE: ./blum-goldwasser 23 31 26 49
        //EXAMPLE: ./blum-goldwasser 499 547 159201 1027
    }

    sscanf(argv[1],"%lu",&(privkey.p));
    sscanf(argv[2],"%lu",&(privkey.q));
    sscanf(argv[3],"%lu",&(M));


    if (!is_prime(privkey.p,64) || (privkey.p % 4) != 3) {
        fprintf(stderr,"[ERR] %lu is not a Blum prime number.\n",privkey.p);
        exit(0x101);
    }

    if (!is_prime(privkey.q,64) || (privkey.q % 4) != 3) {
        fprintf(stderr,"[ERR] %lu is not a Blum prime number.\n",privkey.q);
        exit(0x102);
    }

   
    M = bg_decrypt(enc, privkey);
    printf("Decrypted message: %lu (0x%016lx)\n", M,M);
}