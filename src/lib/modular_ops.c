#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "modular_ops.h"

// Modular multiplication
// computes x = (a*b) mod m without overflowing
uint64_t modmul(uint64_t a, uint64_t b, uint64_t m) {
    //if (a > ipow(2,63)-1) printf("[WARN] %lu > %lu",a,ipow(2,63)-1);
    //if (b > ipow(2,63)-1) printf("[WARN] %lu > %lu",b,ipow(2,63)-1);
    
    uint64_t x = 0;
    a = a % m;

    while (b > 0) {
        if (b & 1) { // b % 2 == 1    
            x = (x + a) % m;
        }
        //a = (a * 2) % m;
        a <<= 1;
        a %= m;
        b >>= 1; // b = b/2;
    }
    return x % m;
}


// Integer exponentiation by squaring
uint64_t ipow(uint64_t base, uint64_t exp) {
    uint64_t result = 1;
    for (;;) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}


// Modular exponentiation without integer overflow
// using the Square and Multiply algorithm
/* uint64_t modexp(uint64_t a, uint64_t exp, uint64_t m) {
    uint64_t b, b_exp[KEY_LENGTH];
    uint64_t partial;

    if (exp < 1) return (a % m);

    // Conversion of exp to binary
    for (int i=KEY_LENGTH-1; i >= 0; i--) {
        if (exp > 0) {
            b_exp[i] = exp % 2;
            exp = (exp - b_exp[i]) / 2;
        }
        else b_exp[i] = 0;
    }

    // Calculate partial products
    b = 1;
    partial = a;
    for (int i=0; i<KEY_LENGTH; i++) {

        // multiply step
        if (b_exp[KEY_LENGTH-i-1] > 0) {
            //multiply (and reduce mod m)
            b = (b*partial) % m;
        }

        // (next) square step
        //partial = ipow(partial,2) % m;
        partial = (partial*partial) % m;
    }

    return b;

} */

uint64_t modexp(uint64_t a, uint64_t exp, uint64_t m) {
    uint64_t b;
    uint64_t partial;

    if (exp < 1) return (a % m);

    // Calculate partial products
    b = 1;
    partial = a % m;
    for (int i=0; exp > 0; i++) {

        // multiply step
        if (exp & 1) {
            //multiply (and reduce mod m)
            b = modmul(b,partial,m);//(b*partial) % m;
        }

        // (next) square step
        exp >>= 1; // exp = exp/2
        //partial = ipow(partial,2) % m;
        partial = modmul(partial,partial,m);//(partial*partial) % m;
    }

    return b;

}


// Extended Euclidean algorithm
// Returns GCD(a,b) and finds x,y in the Bezout identity
// a*x + b*y = GCD(a,b)
// <TODO> iterative version
uint64_t extended_euclid(uint64_t a, uint64_t b, int64_t *x, int64_t *y) {
     
    // Base Case
    if (a == 0) {
        *x = 0, *y = 1;
        return b;
    }
     
    // To store results of recursive call
    int64_t x1, y1;
    uint64_t gcd = extended_euclid(b % a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b / a) * x1;
    *y = x1;
 
    return gcd;
}

// Fast GCD computation with the Euclidean Algorithm
uint64_t gcd(uint64_t a, uint64_t b) {
    int64_t unused;
    return extended_euclid(a,b,&unused,&unused);
}


// Find x solution for 
// a*x ≡ 1 (mod m)
uint64_t mod_inverse(uint64_t a, uint64_t m) {
    int64_t y,x;
 
    if (m == 1) return 0;

    extended_euclid(a,m,&x,&y);
 
    // Make x positive
    if (x < 0)
        x += m;
 
    return (uint64_t)x;
}

// Some obscure dark magic
// from: https://stackoverflow.com/questions/45535635/modular-multiplicative-inverse
//
// return a modular multiplicative inverse of n with respect to the modulus.
// return 0 if the linear congruence has no solutions.
/* uint64_t mod_inverse(uint64_t n, const uint64_t mod) {
    uint64_t a, b = mod, c, d = 0, e = 0, f = 1, g = b;
    for (n *= b > 1; n > 1 && (n *= b > 0); f = a, d = (d & 3) | (d & 1) << 2) {
        a = e, e *= n / (c = b);
        b = n % b, n = c;
        d = (d & 6) | (d & 2) >> 1;
        c = d > 1 && d < 6;
        d = (d & 5) | (c || f > e ? (d & 4) >> 1 : ~d & 2);
        e = c ? e + f : f > e ? f - e : e - f;
    }
    return n ? d & 4 ? g - f : f : 0;
} */

// Checks if g is a primitive root modulo p
bool is_primitive_root(uint64_t g, uint64_t p) {
    int phi = p - 1;
    for (int i = 2; i * i <= phi; i++) {
        if (phi % i == 0) {
            if (modexp(g, phi / i, p) == 1)
                return false;
            while (phi % i == 0)
                phi /= i;
        }
    }
    if (phi > 1 && modexp(g, phi, p) == 1)
        return false;
    return true;
}


// CRT optimized for systems of 2 congruences
uint64_t chinese_remainder_2(uint64_t b1, uint64_t b2, uint64_t m1, uint64_t m2) {
    uint64_t n,x,tmp;
    int64_t a,b;

    // M1=m2, M2=m1
    // M1' and M2' can be obtained with a single run of the extended Eulerian algo.
    // also, no need to allocate heap memory
    n = m1*m2;
    a=b=0;
    extended_euclid(m1, m2, &a,&b);
    
    // Simplified Chinese Remainder Theorem
    // (for a system of 2 congruences)
    //x = (b*m2*b1 + a*m1*b2) % n;
    a = (a<0) ? (a+n)%n : a;
    b = (b<0) ? (b+n)%n : b;
    tmp = modmul(b,m2,n);
    tmp = modmul(tmp,b1,n);
    x = modmul(a,m1,n);
    x = modmul(x,b2,n);
    return x = (x + tmp) % n;
}


// Finds the solution to a system of <count> congruences with the Chinese Remainder Theorem.
uint64_t chinese_remainder(uint64_t *b, uint64_t *m, unsigned count) {
    uint64_t x, mi, n, tmp;

    // Use a faster, specific implementation for systems of 2 congruences
    if (count == 2) return chinese_remainder_2(b[0],b[1],m[0],m[1]);

    uint64_t *minv = malloc(count*sizeof(uint64_t));

    n = 1;
    for (int i = 0; i < count; i++) {
        n *= m[i];
    }

    x = 0;
    for (int i = 0; i < count; i++) {
        mi = n / m[i];
        minv[i] = mod_inverse(mi, m[i]);
        
        //x = (x + mi[i]*minv[i]*b[i]) % n;
        tmp = modmul(mi, minv[i], n);
        tmp = modmul(tmp, b[i], n);
        x = (x + tmp) % n;
    }

    free(minv);

    return x;
}

