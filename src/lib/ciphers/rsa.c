#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "modular_ops.h"

#include "ciphers/rsa.h"

uint64_t rsa_encrypt(uint64_t message, rsa_public_key key) {
    return modexp(message, key.e, key.n);
}

uint64_t rsa_decrypt(uint64_t ciphertext, rsa_private_key key) {
    return modexp(ciphertext, key.d, key.n);
}


rsa_signature rsa_sign(uint64_t M, rsa_private_key privkey) {
    return modexp(M, privkey.d, privkey.n);;
}

bool rsa_verify(uint64_t M, rsa_signature S, rsa_public_key pubkey) {
    //printf("[INFO] S = %lu, M = %lu\n",S,modexp(S, pubkey.e, pubkey.n));
    if (M == modexp(S, pubkey.e, pubkey.n)) return true;
    else return false;
}