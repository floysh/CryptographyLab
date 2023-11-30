#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/rsa.h"

#define MSG_LEN 2000


int main(int argc, char **argv) {
    uint64_t p,q,n,e,d,phin;
    unsigned char message[MSG_LEN];
    uint64_t encrypted_buffer[MSG_LEN];
    memset(message,0,MSG_LEN);

    char *buffer = malloc(MSG_LEN*sizeof(char));
    memset(buffer,0,MSG_LEN);

    // INITIALIZE
    if (argc < 4) {
        printf("p: ");
        fgets(buffer, MSG_LEN, stdin);
        sscanf(buffer, "%16lu",&p);

        printf("q: ");
        fgets(buffer, MSG_LEN, stdin);
        sscanf(buffer,"%16lu",&q);
 
        
        printf("encryption key: ");
        fgets(buffer, MSG_LEN, stdin);
        sscanf(buffer,"%16lu",&e);

    }
    else {
        sscanf(argv[1],"%16lu",&p);
        sscanf(argv[2],"%16lu",&q);
        sscanf(argv[3],"%16lu",&e);
    }

    //printf("p: %016lx\nq: %016lx\ne: %016lx\n",p,q,e);

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
    printf("(p,q): (%lu,%lu) (0x%08lx,0x%08lx)\n",p,q,p,q);
    printf("n = p*q: %lu (0x%016lx)\n",n,n);
    printf("phi(n): %lu\n",phin);
    printf("--------------------------------------\n");

    //
    // KEY GENERATION
    //

    // Generate encryption key with (e,phin) = 1
    // <TODO> 
    printf("Encryption key:\t\t %lu (0x%08lx)\n",e,e);


    // Generate decryption key
    d = mod_inverse(e,phin); // e*d ≡ 1 (mod phi(n)) 

    printf("Decryption key:\t\t %lu (0x%08lx)\n",d,d);
   
    
    // Generate a smaller decryption key
    uint64_t d_min, phin1;
    phin1 = phin/gcd((p-1),(q-1));
    d_min = mod_inverse(e, phin1);

    printf("Min. decryption key:\t %lu (0x%08lx)\n", d_min,d_min);


    printf("--------------------------------------\n");

    rsa_private_key privkey = (rsa_private_key) {.d = d_min, .n = p*q};
    rsa_public_key  pubkey = (rsa_public_key) {.e = e, .n = p*q};


    printf("\nType your Message: ");
    fgets((char*)message, MSG_LEN, stdin);
    // Properly terminate input string
    for (int i=0; i < MSG_LEN; i++) {
        if (message[i] == 10) {message[i] = 0; break;}
    }

    free(buffer);

    //printf("%s\n",message);
    for (int i=0; i<MSG_LEN; i++) {
        if (message[i] != 0) printf("%016x ",message[i]);
    }
    printf("\n\n");

    

    //
    // ENCRYPT 
    // C ≡ M^e (mod n)
    //

    printf("Ciphertext: ");
    //printf("%2s\n",(char*)encrypted_buffer);
    for (int i=0; i<MSG_LEN; i++) {
        //encrypted_buffer[i] = modexp(message[i], e, n);
        encrypted_buffer[i] = rsa_encrypt(message[i], pubkey);
        printf("%c",(unsigned char) encrypted_buffer[i] );
    }
    printf("\n");

    for (int i=0; i<MSG_LEN; i++) {
        if (encrypted_buffer[i] != 0) printf("%016lx ",encrypted_buffer[i]);
    }
    printf("\n\n");



    
    //
    // DECRYPT 
    // M ≡ C^d (mod n)
    //

    memset(message,0,MSG_LEN);
    printf("Decrypted messages: ");
    for (int i=0; i<MSG_LEN; i++) {
        // Print decrypted content on the fly
        //encrypted_buffer[i] = modexp(encrypted_buffer[i], d, n);
        printf("%c", (char) rsa_decrypt(encrypted_buffer[i], privkey));
        encrypted_buffer[i] = rsa_decrypt(encrypted_buffer[i], privkey);
    }

    printf("\n");

    
    for (int i=0; i<MSG_LEN; i++) {
        if (encrypted_buffer[i] != 0) printf("%016lx ",encrypted_buffer[i]);
    }
    printf("\n");

}