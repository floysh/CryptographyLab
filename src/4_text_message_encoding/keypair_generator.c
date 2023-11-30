#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "modular_ops.h"
#include "encoding.h"
#include "primes.h"

#define PRIVKEY_PATH "privkey.txt"
#define PUBKEY_PATH "pubkey.txt"


int main(int argc, char **argv) {
    uint64_t p,q,n,e,d,phin;
    FILE *f;

    p = 0;
    q = 0;
    e = 0;

    if (argc >= 2) sscanf(argv[1],"%lu",&p);
    if (argc >= 3) sscanf(argv[2],"%lu",&q);
    if (argc >= 4) sscanf(argv[3],"%lu",&e);

    //sscanf(argv[1],"%lu",&max_prime); //q = argv[2]
    //sscanf(argv[2],"%lu",&p); //p = argv[1]
    //sscanf(argv[3],"%lu",&q); //q = argv[2]
    n = p*q;
    phin = (p-1)*(q-1);

    uint64_t max_encodable = (ipow(N,K) - 1);

    // Check if primes are suitable
    if (p > 0 && !is_prime(p, 64)) {
        fprintf(stderr, "[ERR] p=%lu is not prime\n",p);
        //p = 0;
    }
    if (q > 0 && !is_prime(q, 64)) {
        fprintf(stderr, "[ERR] q=%lu is not prime\n",q);
        //q = 0;
    }

    while (n < max_encodable) {
        if (p > 0 || q > 0) {
            fprintf(stdout, "[INFO] n = %lu\n",n);
            fprintf(stderr, "[ERR] n=p*q must be bigger than %lu and less than %lu\n", max_encodable, 0xFFFFFFFFFFFFFFFF);
            fprintf(stderr, "      Will try to generate suitable primes\n");
        }

        srand(time(0));
        p = primegen();
        q = primegen();

        n = p*q;
        phin = (p-1)*(q-1);
    }

    if (e == 0) e = primegen()*primegen();

    // Check if encryption key is suitable
    if (gcd(e, phin) > 1) {
        fprintf(stderr, "[ERR] encryption key is not coprime with %lu. gcd(e, phin) = %lu\n", phin, gcd(e, phin));

        // "Fix" encryption key
        e = e / gcd(e, phin);
        do {
            e = primegen()*primegen() + 7;
        } while (gcd(e, phin) != 1 || gcd(e, phin) >= phin/4);
    }
    

    printf("Initialized an RSA cryptosystem:\n");
    printf("\tp: %lu",p); printf("\tq: %lu\n",q);
    printf("\tφ(n): %lu\n",phin);

    // Generate the related decryption key
    // ed = 1 (mod phin)
    d = mod_inverse(e,phin/gcd(p-1, q-1)); // smallest decryption key

    printf("\te: %lu\n",e);
    printf("\td: %lu\n",d);
    printf("\tn: %lu\n",n);


    printf("\n----------\n");
    printf("PUBLIC  KEY: (e,n) = (%lu,%lu) \t(encryption key)\n",e,n);
    printf("PRIVATE KEY: (d,n) = (%lu,%lu) \t(decryption key)\n",d,n);
    printf("----------\n");


    if ( (f = fopen(PRIVKEY_PATH, "wt+")) ) {
        fprintf(f,"%lu %lu",e,n);
        printf("[INFO] Private key successfully saved to %s\n",PRIVKEY_PATH);
        fclose(f);
    }

    if ( (f = fopen(PUBKEY_PATH, "wt+")) ) {
        fprintf(f,"%lu %lu",d,n);
        printf("[INFO] Public key successfully saved to %s\n",PUBKEY_PATH);
        fclose(f);
    }    

}