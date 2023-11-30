#include <stdint.h>

typedef struct {
    uint64_t message;   // ciphertext
    uint64_t sf;        // final seed s_(i+1)
} bg_ciphertext;

typedef struct {
    uint64_t n;         // Blum integer
} bg_public_key;

typedef struct {
    uint64_t p,q;       // Blum primes
} bg_private_key;

size_t bg_message_bitlength(uint64_t M);

bg_ciphertext bg_encrypt(uint64_t M, bg_public_key pubkey, uint64_t s);

uint64_t bg_decrypt(bg_ciphertext enc, bg_private_key privkey);

uint64_t bg_recover_bbs_seed(bg_ciphertext enc, bg_private_key privkey);