#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "modular_ops.h"
#include "ciphers/elgamal.h"

eg_ciphertext elgamal_encrypt(uint64_t message, eg_public_key key, uint64_t nonce) {
    eg_ciphertext C;

    C.gamma = modexp(key.g, nonce, key.p);
    C.delta = modexp(key.beta, nonce, key.p);
    C.delta = modmul(message, C.delta, key.p);
    
    return C;
}

uint64_t elgamal_decrypt(eg_ciphertext C, eg_private_key key) {
    uint64_t message;
    uint64_t exp = key.p - key.a - 1;

    message = modexp(C.gamma, exp, key.p);
    message = (message * C.delta) % key.p; //modmul(message, ciphertext.delta, key.p);

    return message;
}

eg_signature elgamal_sign(uint64_t M, eg_private_key privkey, uint64_t nonce) {
    uint64_t k1 = mod_inverse(nonce, privkey.p-1);

    eg_signature S;
    S.gamma = modexp(privkey.g, nonce, privkey.p);
    // (M-a*gamma)k^-1  /mod/ (p-1)
    S.delta = modmul(privkey.a, S.gamma, privkey.p-1);
    if (M < S.delta) M += privkey.p-1;
    S.delta = modmul((M - S.delta), k1, privkey.p-1);

    return S;
}

bool elgamal_verify(uint64_t M, eg_signature S, eg_public_key pubkey) {
    uint64_t a,b;

    a = modexp(pubkey.beta, S.gamma, pubkey.p);
    b = modexp(S.gamma, S.delta, pubkey.p);

    a = modmul(a,b, pubkey.p);
    b = modexp(pubkey.g, M, pubkey.p);


    //printf("(B^G)*(G^D) == g^M <==> %lu == %lu ? \n",a,b);

    return a == b;
}