#ifndef MODULAR_OPS
#define MODULAR_OPS

#include <stdint.h>
#include <stdbool.h>

#define KEY_LENGTH 1024

// Modulo multiplication
// computes x = (a*b) mod m without overflowing
uint64_t modmul(uint64_t a, uint64_t b, uint64_t m);

// Integer exponentiation
// with squaring
uint64_t ipow(uint64_t base, uint64_t exp);


// Modular exponentiation without integer overflow
// using the Square and Multiply algorithm
uint64_t modexp(uint64_t a, uint64_t exp, uint64_t m);


// Extended Euclidean algorithm
// Returns GCD(a,b) and finds x,y in the Bezout identity
// a*x + b*y = GCD(a,b)
uint64_t extended_euclid(uint64_t a, uint64_t b, int64_t *x, int64_t *y);

// Fast GCD computation with the Euclidean Algorithm
uint64_t gcd(uint64_t a, uint64_t b);


// Finds x for 
// a*x ≡ 1 (mod m)
uint64_t mod_inverse(uint64_t a, uint64_t m);


// Checks if g is a primitive root modulo p
bool is_primitive_root(uint64_t g, uint64_t p);


// Finds the solution to a system of congruences with the Chinese Remainder Theorem.
uint64_t chinese_remainder(uint64_t *b, uint64_t *m, unsigned count);


#endif