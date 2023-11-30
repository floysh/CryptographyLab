
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "modular_ops.h"
#include "primes.h"
#include "ciphers/blum-goldwasser.h"

int main(int argc, char **argv) {
    bg_ciphertext enc;
    bg_public_key pubkey;
    bg_private_key privkey;
    
    uint64_t M,s0,z,si;
    int64_t a,b;
    size_t bitlen = 0;

    if (argc < 5) {
        //printf("Encrypt a message with the Blum-Goldwasser probabilistic encryption scheme\n");
        printf("Usage:\n\t%s <p> <q> <M> <s> [bitlen]\n", argv[0]);
        printf("p,q\t Blum primes\n");
        printf("M\t message\n");
        printf("s\t random seed\n");
        printf("bitlen\t mask size (in bit)\n");
        printf("All parameters must be unsigned integers in range (1 ... 2^64-1)\n");
        return(-1);

        //EXAMPLE: ./blum-goldwasser 23 31 26 49
        //EXAMPLE: ./blum-goldwasser 499 547 159201 1027
    }

    sscanf(argv[1],"%lu",&(privkey.p));
    sscanf(argv[2],"%lu",&(privkey.q));
    sscanf(argv[3],"%lu",&(M));
    sscanf(argv[4],"%lu",&(s0));

    if (argc >= 6) {
        sscanf(argv[5],"%lu",&(bitlen));
        bitlen = (bitlen > sizeof(M)*8) ? sizeof(M)*8 : bitlen;
    }
    else {
        bitlen = bg_message_bitlength(M);
    }

    pubkey.n = privkey.p * privkey.q;

    if (!is_prime(privkey.p,64) || (privkey.p % 4) != 3) {
        fprintf(stderr,"[ERR] %lu is not a Blum prime number.\n",privkey.p);
        exit(0x101);
    }

    if (!is_prime(privkey.q,64) || (privkey.q % 4) != 3) {
        fprintf(stderr,"[ERR] %lu is not a Blum prime number.\n",privkey.q);
        exit(0x102);
    }
    
    if (gcd(s0,pubkey.n) > 1) {
        fprintf(stderr,"[ERR] The random seed s0 must be different from p and q.\n");
        exit(0x100);
    }


    extended_euclid(privkey.p,privkey.q, &a,&b);

    printf("Initialized a Blum-Goldwasser cryptosystem with\n");
    printf("Encryption key:\t (n)    (%lu) \t(0x%016lx)\n", pubkey.n, pubkey.n);
    printf("Decryption key:\t (p,q)  (%lu,%lu) \t(0x%08lx,0x%08lx)\n", privkey.p,privkey.q, privkey.p,privkey.q);
    printf("Private key Bézout identity: ap + bq = (%ld * %lu) + (%ld * %lu) = 1\n", a,privkey.p,b,privkey.q);
    printf("------------------------------------------\n");


    printf("BBS PRNG seed: %lu\n", s0);
    printf("Message: %lu (0x%016lx)\n", M,M);

    printf("------------------------------------------\n");

    z = 0; si=s0;
    for (unsigned i=1; i <= bitlen; i++) {
        si = modexp(si,2,pubkey.n);
        z = (z << 1) | (si & 1);     // z contains all the least significant bits obtained from a bbs sequence initialized with s
        printf("s_%02d = %lu\tz_%02d = %01lx\n", i,si, i,(si & 1));
    }
    si = modexp(si,2,pubkey.n);       // s_(i+1) in the bbs sequence
    printf("s_%02u = %lu\n\n",(unsigned)(bitlen+1),si);
    //printf("\nEncryption mask: z = 0x%0lu\n",z);

    
    if (bitlen == bg_message_bitlength(M)) {
        enc = bg_encrypt(M, pubkey, s0); // this will always encrypt with bitlen=64
    }
    else {
        enc.message = (M^z);
        enc.sf = si;
    }
    
    printf("Encrypted message: (C, s_%02lu) = (%lu,%lu) (0x%016lx, 0x%016lx)\n", (bitlen+1), enc.message, enc.sf, enc.message, enc.sf);

    printf("------------------------------------------\n");
    

    uint64_t a1,a2,b1,b2;
    a1 = modexp((privkey.p+1)/4, (bitlen+1), (privkey.p-1));
    a2 = modexp((privkey.q+1)/4, (bitlen+1), (privkey.q-1));
    b1 = modexp(si, a1, privkey.p);
    b2 = modexp(si, a2, privkey.q);
    printf("a1: %lu \tb1: %lu\n",a1,b1);
    printf("a2: %lu \tb2: %lu\n",a2,b2);
    
    // Simplified Chinese Remainder Theorem
    // (for a system of 2 congruences)
    // TODO: merge with the regular algorithm
    uint64_t rs0,tmp;
    //rs0 = (b*privkey.q*b1 + a*privkey.p*b2) % n;
    a = (a<0) ? (a+pubkey.n)%pubkey.n : a;
    b = (b<0) ? (b+pubkey.n)%pubkey.n : b;
    rs0 = modmul(a,b2,pubkey.n);
    rs0 = modmul(rs0,privkey.p,pubkey.n);
    tmp = modmul(b,b1,pubkey.n);
    tmp = modmul(tmp,privkey.q,pubkey.n);
    rs0 = (rs0 + tmp) % pubkey.n;

    printf("Recovered seed: %lu %s\n", rs0, (rs0 == s0) ? "✔️" : "❌");
    
    M = si = z = 0;
    if (bitlen == bg_message_bitlength(M)) {
        M = bg_decrypt(enc, privkey);
    }
    else {
        
        for (unsigned i=1; i <= bitlen; i++) {
            si = modexp(si,2,pubkey.n);
            z = (z << 1) | (si & 1);
        }
        M = enc.message ^ z;
    }
    printf("Decrypted message: %lu (0x%016lx)\n", M,M);
}