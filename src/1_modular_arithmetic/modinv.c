#include <stdio.h>
#include <stdlib.h>
#include "modular_ops.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage:\n\t%s <a> <m>\n", argv[0]);
        printf("Finds x for: ax ≡ 1 (mod m)\n");
        exit(-1);
    }

    uint64_t a,x,m;
    sscanf(argv[1], "%lu", &a);
    sscanf(argv[2], "%lu", &m);
    
    uint64_t d = gcd(a,m);
    if (d > 1) {
        printf("[ERR] Modular inverse does not exist because GCD(a,m) = %lu\n",d);
        exit(-2);
    }
    
    x = mod_inverse(a,m);

    //printf("%lu*x ≡ 1 (mod %lu)\n",a,m);
    printf("x ≡ %lu (mod %lu)\n",x,m);

    exit(0);
}