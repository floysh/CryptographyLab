#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "modular_ops.h"
#include "ciphers/rabin.h"

rabin_ciphertext rabin_encrypt(uint64_t M, rabin_public_key pubkey) {
    rabin_ciphertext enc;
    enc.message = modexp(M,2,pubkey.n);
    
    // This is used to determine which decryption is correct
    enc._check = (M | (pubkey.n & 0x0000AAAA));
    //printf("[DBG] MAGIC = %lu (0x%016lx)\n", enc._check, enc._check);
    enc._check = modexp(enc._check,2,pubkey.n);
    //printf("[DBG] ENCRYPTED MAGIC = %lu (0x%016lx)\n", enc._check, enc._check);
    return enc;
}


int rabin_decryptions(uint64_t C, rabin_private_key privkey, uint64_t *result) {
    int64_t a,b;
    uint64_t r,s,n;

    n = privkey.p * privkey.q;
    extended_euclid(privkey.p,privkey.q, &a,&b);
    a = (a<0) ? (a+n)%n : a;
    b = (b<0) ? (b+n)%n : b;

    r = modexp(C, (privkey.p+1)/4, privkey.p);
    s = modexp(C, (privkey.q+1)/4, privkey.q);

    a = modmul(a,privkey.p,n);
    a = modmul(a,s,n); // aps
    b = modmul(b,privkey.q,n);
    b = modmul(b,r,n); // bqr

    result[0] = (a+b) % n; //aps+brq
    result[1] = (a-b) % n; //aps-brq
    result[2] = (n - result[0]) % n;
    result[3] = (n - result[1]) % n;

    return 0;
}

uint64_t rabin_decrypt(rabin_ciphertext enc, rabin_private_key privkey) {
    //rabin_decryptionset x, rmsg;
    /* unsigned char magic[5];
    memset(magic,0,5); */

    uint64_t x[4], rmsg[4];

    // We must obtain all the possible decryptions
    rabin_decryptions(enc.message, privkey, x);

    uint64_t mask = ((privkey.p*privkey.q) & 0x0000AAAA);

    
    // Determine which decryption is the actual message
    rabin_decryptions(enc._check, privkey, rmsg);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            //printf("[DBG] %d %d MAGIC = %lu RECONSTR = %lu\n",i,j,rmsg[i], (x[j] | mask));
            if (rmsg[i] == (x[j] | mask)) return x[j];
        }
        
    }
    
    fprintf(stderr, "[ERR] True message detection failed.\n");
    return 0;
}