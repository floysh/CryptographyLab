#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modular_ops.h"
//#include "ciphers/rsa.h"


// This attack aims to find

int main(int argc, char **argv) {
    uint64_t n,e;
    uint64_t M,C;
    uint64_t period;

    if (argc < 4) {
        char buffer[1024];
        memset(buffer,0,1024);
        
        printf("modulus (n): ");
        fgets(buffer, 1024, stdin);
        sscanf(buffer,"%lu",&n);
        
        printf("public exponent (e): ");
        fgets(buffer, 1024, stdin);
        sscanf(buffer,"%lu",&e);
        
        printf("candidate message (M): ");
        fgets(buffer, 1024, stdin);
        sscanf(buffer,"%lu",&M);
        /* n = 133;
        e = 5;
        M = 61; */
    }
    else {
        sscanf(argv[1],"%lu",&n);
        sscanf(argv[2],"%lu",&e);
        sscanf(argv[3],"%lu",&M);
    }

    printf("PUBLIC KEY:\t(n,e) = (%lu,%lu) = (0x%08lx 0x%08lx)\n\n",n,e,n,e);

    // Arguments checking
    M %= n;
    if (gcd(M,n) > 1) {
        // We've got extremely lucky!
        uint64_t p,q;
        printf("We've got lucky! 🍀\nM = %lu is not coprime with the modulus! Factorization is trivial 😎\n",M);
        p = gcd(M,n);
        q = n / p;

        printf("\t%lu = %lu x %lu\n", n,p,q);

        printf("\nNow we can easily find the private key.\n");
        uint64_t phin = (p-1)*(q-1);
        uint64_t d = mod_inverse(e,phin);
        printf("PRIVATE KEY:\t(n,d) = (%lu,%lu)\n",n,d);

        exit(0);
    }

    C = modexp(M,e,n);

    // We can guess the encryption period
    // by performing subsequent encryptions of a message,
    // given that (M,n) = 1
    for(period=1; C != M; period++) {
        C = modexp(C,e,n);
    }

    printf("Period is %lu\n",period);
    printf("To decrypt a ciphertext made with this public key, you have to encrypt it %lu times!\n",period-1);

    // Proof that ciphertexts can be decrypted 
    // going through the cycle
    printf("Let's see if it really works:\n");
    M = M*5 % n;
    C = modexp(M,e,n);
    printf("M = %lu -> C = %lu ",M,C);
    for (int i=0; i<period-1; i++) {
        C = modexp(C,e,n);
    }
    printf("-> M = %lu\n",C);

    // Attempt to factorize the modulus

    printf("\n------------------------------------------------\n");
    printf("\nHaving the period, we can attempt to factorize the modulus. Which means, compromising *every* possible keypair sharing this value for n=%lu!)\n",n);

    uint64_t magic_exponent = modexp(e,period,n) - 1;
    uint64_t square_residual;

    
    M=1;
    for (int i=3; i<n; i++) {
        M++;
        do {
            square_residual = modexp(M,magic_exponent,n);            
            magic_exponent = magic_exponent / 2;
        } while(square_residual == 1 && magic_exponent % 2 == 0);

        uint64_t p=1, q=1;
        if (square_residual > 1) {
            if (n % (square_residual - 1) == 0) {
                p = (square_residual - 1);
                q = n / p;
            }
            else if (n % (square_residual + 1) == 0) {
                p = (square_residual + 1);
                q = n / p;
            }
        }

        if (n == p*q) { // check against possible integer overflows
            printf("Good news! M=%lu factorized the modulus 🎉\n",M);

            printf("\t\t%lu = %lu x %lu\n\n",n,p,q);

            printf("Now we can easily find the private key\n");
            uint64_t phin = (p-1)*(q-1);
            uint64_t d = mod_inverse(e,phin);
            printf("PRIVATE KEY:\t(n,d) = (%lu,%lu)\n",n,d);

            printf("\nLet's try it with another message:\n");
            M = (M*3 + 4) % n;
            C = modexp(M,e,n);
            printf("M = %lu -> C = %lu -> M = %lu\n",M,C,modexp(C,d,n));

            exit(0);

        }

    }
    printf("Looks like we ran out of luck 😢\n");
    exit(0);
}