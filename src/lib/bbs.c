#include <stdint.h>
#include "modular_ops.h"
#include "bbs.h"

// Blum-Blum-Shub pseudorandom number generator
// n = p*q, where p and q are primes congruent to 3 mod 4
// seed must be coprime with n
uint64_t bbs_gen(uint64_t n, uint64_t x) {
    x = x % n;
    //return x = modmul(x,x,n); // x^2 mod n;
    return x = modexp(x,2,n);
}