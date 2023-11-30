#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "modular_ops.h"

#ifndef PRIMES
#define PRIMES

// Check if a number is prime with the 
// Miller-Rabin primality test.
// Error rate is 1/4^max_iterations.
// Suggested values for max_iterations are 64+.
bool is_prime(uint64_t p, uint64_t max_iterations);

// Generates a random (pseudo)prime number
uint64_t primegen();

// Generates a Blum prime
// P = 3 + 4k
uint64_t blum_primegen();


#endif