#include <stdint.h>
#include "modular_ops.h"

#ifndef RABIN
#define RABIN

//#define RABIN_check_GRAM ((unsigned char*) "RABI") // must be of the same lenght of K (encoding.h)

typedef struct {
    uint64_t p,q;       // Blum primes
} rabin_private_key;

typedef struct {
    uint64_t n;         // Blum integer
} rabin_public_key;

typedef struct {
    uint64_t _check;
    uint64_t message;
} rabin_ciphertext;

rabin_ciphertext rabin_encrypt(uint64_t M, rabin_public_key pubkey);

int rabin_decryptions(uint64_t C, rabin_private_key privkey, uint64_t *result);

uint64_t rabin_decrypt(rabin_ciphertext C, rabin_private_key privkey);

#endif