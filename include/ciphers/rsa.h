#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t d;
    uint64_t n;
} rsa_private_key;

typedef struct {
    uint64_t e;
    uint64_t n;
} rsa_public_key;

typedef uint64_t rsa_signature;


uint64_t rsa_encrypt(uint64_t message, rsa_public_key key);

uint64_t rsa_decrypt(uint64_t ciphertext, rsa_private_key key);


rsa_signature rsa_sign(uint64_t M, rsa_private_key privkey);

bool rsa_verify(uint64_t M, rsa_signature S, rsa_public_key pubkey);