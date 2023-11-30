#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/rsa.h"



int main(int argc, char **argv) {
    uint64_t p,q,n,e,d,phin;
    
    char *buffer = malloc(sizeof(uint64_t));
    memset(buffer,0,sizeof(uint64_t));

    // INITIALIZE
    if (argc < 3) {
        printf("p: ");
        fgets(buffer, sizeof(uint64_t), stdin);
        sscanf(buffer, "%lu",&p);

        printf("q: ");
        fgets(buffer, sizeof(uint64_t), stdin);
        sscanf(buffer,"%lu",&q);
 
        
        printf("encryption key: ");
        fgets(buffer, sizeof(uint64_t), stdin);
        sscanf(buffer,"%lu",&e);

    }
    else {
        sscanf(argv[1],"%lu",&p);
        sscanf(argv[2],"%lu",&q);
        sscanf(argv[3],"%lu",&e);
    }

    n = p*q;
    phin = (p-1)*(q-1);

    // CHECK PARAMETERS
    if ( (p < 2) | (q < 2) ) {
        fprintf(stderr,"[ERR] Invalid prime value(s)\n");
        exit(-1);
    }
    if (e == 1) {
        fprintf(stderr,"[WARN] Encryption key is 1. The message will not be encrypted!\n");
    }
    if ( gcd(e,phin) > 1 ) {
        fprintf(stderr,"[WARN] Invalid encryption key value. Must be coprime with (p-1)*(q-1) = %lu\n",phin);
        exit(-2);
    }

    // Dummy primality check
    for (uint64_t i=3; i <= p/2; i+=2) {
        if (p % i == 0) {
            fprintf(stderr,"[ERR] %lu is not prime!\n",p);
            exit(0x101);
        }
    }
    for (uint64_t i=3; i <= q/2; i+=2) {
        if (q % i == 0) {
            fprintf(stderr,"[ERR] %lu is not prime!\n",q);
            exit(0x102);
        }
    }


    printf("Initialized an RSA cryptosystem with\n");
    printf("(p,q): (%lu,%lu)\n",p,q);
    printf("n = p*q: %lu\n",n);
    printf("φ(n): %lu\n",phin);
    printf("--------------------------------------\n");

    //
    // KEY GENERATION
    //

    // Generate encryption key with (e,phin) = 1
    // <TODO> 
    printf("Encryption key:\t\t %lu\n",e);


    // Generate decryption key
    d = mod_inverse(e,phin); // e*d ≡ 1 (mod phi(n)) 

    printf("Decryption key:\t\t %lu\n",d);
   
    
    // Generate a smaller decryption key
    uint64_t d_min, phin1;
    phin1 = phin/gcd((p-1),(q-1));
    d_min = mod_inverse(e, phin1);

    printf("Min. decryption key:\t %lu\n", d_min);   

}