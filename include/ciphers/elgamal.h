#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t p;
    uint64_t g;
    uint64_t a;
} eg_private_key;

typedef struct {
    uint64_t p;
    uint64_t g;
    uint64_t beta;  //g^a mod p
} eg_public_key;

typedef struct elgamal_ciphertext {
    uint64_t gamma;
    uint64_t delta;
} eg_ciphertext;

//same structure as ciphertext, different delta definition
typedef struct elgamal_signature {
    uint64_t gamma;
    uint64_t delta;
} eg_signature;


eg_ciphertext elgamal_encrypt(uint64_t message, eg_public_key key, uint64_t nonce);

uint64_t elgamal_decrypt(eg_ciphertext C, eg_private_key key);


eg_signature elgamal_sign(uint64_t M, eg_private_key privkey, uint64_t nonce);

bool elgamal_verify(uint64_t M, eg_signature S, eg_public_key pubkey);