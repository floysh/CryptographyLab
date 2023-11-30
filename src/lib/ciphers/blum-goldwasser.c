#include <stdio.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/blum-goldwasser.h"


size_t bg_message_bitlength(uint64_t M) {

    // Keep it constant to prevent decryption failures
    // as bitlength(C) may not be the same as bitlenght(M)
    return sizeof(uint64_t)*8;

    //
    size_t bitlen = 0;
    uint64_t Mtmp = M;
    while ( Mtmp > 0) {
        Mtmp >>= 1;
        bitlen++;
    }
    return bitlen;
}


bg_ciphertext bg_encrypt(uint64_t M, bg_public_key pubkey, uint64_t s) {
    uint64_t z = 0;
    bg_ciphertext enc;

    size_t bitlen = bg_message_bitlength(M);
    
    for (unsigned i=1; i <= bitlen; i++) {
        s = modexp(s,2,pubkey.n);
        z = (z << 1) | (s & 1);     // store all the z_i  contains all the least significant bits obtained from a bbs sequence initialized with s
        //printf("s_%02d = %lu\tz_%02d = %01lx\n", i,s, i,z);
    }
    s = modexp(s,2,pubkey.n);       // s_(i+1) in the bbs sequence 

    enc.message = M ^ z; // bitwise XOR is equivalent to (M + z) % 2 
    enc.sf = s;
    return enc;
}

uint64_t bg_decrypt(bg_ciphertext enc, bg_private_key privkey) {
    uint64_t b1, b2, s;
    uint64_t z,n;

    size_t bitlen = bg_message_bitlength(enc.message);

    n = privkey.p * privkey.q;
    b1 = modexp((privkey.p+1)/4, (bitlen+1), (privkey.p-1)); // a1
    b2 = modexp((privkey.q+1)/4, (bitlen+1), (privkey.q-1)); // a2
    b1 = modexp(enc.sf, b1, privkey.p); // sf^(a1) (mod p)
    b2 = modexp(enc.sf, b2, privkey.q); // sf^(a2) (mod q)
    
    // Recover the seed that was used to generate the encryption mask
    // | s ≡ b1 mod(p)
    // | s ≡ b2 mod(q)
    // -> s ≡ M1*M1'*b1 + M2*M2'*b2 (mod n)
    // 
    // M1 = n/p = q
    // M2 = n/q = p 
    // s ≡ q*M1'*b1 + p*M2'*b2 (mod n)
    //
    // also, M1'q + M2'p = 1
    //
    uint64_t bs[2] = {b1,b2};
    uint64_t ms[2] = {privkey.p,privkey.q};
    s = chinese_remainder(bs,ms,2); // will call the following optimization automatically
    
    /* int64_t a=0,b=0;
    extended_euclid(privkey.p, privkey.q, &a,&b);
    //printf("[DBG] a: %ld (0x%016lx), b: %ld (0x%016lx)\n",a,a, b,b);
    // int -> unsigned int conversion
    a = (a<0) ? (a+n)%n : a;
    b = (b<0) ? (b+n)%n : b;
    //printf("[DBG] a: %ld (0x%016lx), b: %ld (0x%016lx)\n",a,a, b,b);
    //s = (a*privkey.p*b2 + b*privkey.q*b1) % n;
    s = modmul(a,b2,n);
    s = modmul(s,privkey.p,n);
    tmp = modmul(b,b1,n);
    tmp = modmul(tmp,privkey.q,n);
    s = (s + tmp) % n;
    //printf("[DBG] S0: %lu (0x%016lx)\n",s,s); */


    // With the original seed, we can rebuild the mask
    z = 0;
    for (unsigned i=1; i <= bitlen; i++) {
        s = modexp(s,2,n);
        z = (z << 1) | (s & 1);     // z contains all the least significant bits obtained from a bbs sequence initialized with s

        //printf("s_%02d = %lu\tz_%02d = %01lx\n", i,s, i,(s & 1));
    }
    s = modexp(s,2,n);      // s_(i+1) in the bbs sequence

    // and unmask the original message
    return enc.message ^ z;

}