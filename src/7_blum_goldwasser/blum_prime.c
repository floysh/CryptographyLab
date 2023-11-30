#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "primes.h"

int main(int argc, char **argv) {
    uint64_t p;
    srand(time(0));
    
    p = blum_primegen();

    printf("%lu",p);
    return p;
}
