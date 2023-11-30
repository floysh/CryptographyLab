#include <stdint.h>

#ifndef BBS
#define BBS

// Blum-Blum-Shub pseudorandom number generator
// n = p*q, where p and q are primes congruent to 3 mod 4
// seed must be coprime with n
uint64_t bbs_gen(uint64_t n, uint64_t seed);

#endif