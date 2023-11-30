#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "modular_ops.h"


// Miller-Rabin's primality condition check
bool is_witness(uint64_t a, uint64_t s, uint64_t d, uint64_t n) {
    uint64_t x = modexp(a,d,n);
    
    if(x == 1) return true;
    for(unsigned i=0; i < s-1; i++) {
        if(x == n-1) return true;
        x = modexp(x,2,n);
    }

    return (x == (n-1));

}

uint64_t randint (uint64_t low, uint64_t up){
    return rand() % (++up - low)+low;
}

// Check if a number is prime
// using the Miller-Rabin primality test
bool is_prime(uint64_t p, uint64_t max_iterations) {
    uint64_t i;
    uint64_t s; // sentinel

    if (p == 2) return true;
    if (p < 2 || p % 2 == 0) return false;
     
    s = p - 1;
    while (s % 2 == 0) {
        s /= 2;
    }
    for (i = 0; i < max_iterations; i++) {
        uint64_t a = rand() % (p - 1) + 1, temp = s;
        uint64_t mod = modexp(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1) {
            mod = modexp(mod, 2, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0) {
            return false;
        }
    }
    return true;
}

// Generates a random (pseudo)prime number
uint64_t primegen() {
    // for RSA, safe as long as
    // m_max = p*p
    // 2*m <= 2^{64}-1 <==> m <= 2^{63}-1 <==> 
    // p^2 <= 2^{63}-1 <<=> p <= sqrt(2^{63}-1) 
    // p <= floor[sqrt(2)*2^(31)] = 0xb504f334 
    for(uint64_t i=0; i < (0x1<<30); i++) {
        //uint64_t p = randint(0xb504f334, 0xc0000000);
        uint64_t p = randint(0x0400000, 0xb504f334);
        //p |= 0x0000001B00000000;
        if (is_prime(p,64)) return p;
    }
    return 3;
}

// Generates a Blum prime
// P = 3+4k
uint64_t blum_primegen() {
    // With 64bit numbers, safe as long as
    // p <= sqrt(2^63-1) <==> 3+4i <= sqrt(2^63-1)
    // ==> i <= 759250124 < (sqrt(2^63-1) - 3)/4 
    while(true) {
        // Pick a random start point
        uint64_t i = randint(0x04000000, 0xb504f334);
        for(; i <= 0xb504f334; i++) {
            uint64_t p = 4*i + 3;
            if (is_prime(p,64)) return p;
        }
    }
}