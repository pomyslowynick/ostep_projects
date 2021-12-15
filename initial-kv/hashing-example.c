#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define M_VAL 4
#define X_VAL 10

uint32_t hashf_division(uint32_t x, uint32_t M) {
    printf("%d\n", x % M);
    return x % M;
};

int main(int argc, char* argv[]){

    // initiate rand
    srand(time(0));

    unsigned int i, hash;

    // buckets
    int buckets[M_VAL];
    
    for(i = 0; i < X_VAL; i++) {
        uint32_t random_val = rand();
        printf("%d\n", random_val);
        hash = hashf_division(random_val, M_VAL);
        printf("Hash is: %d", hash);
        buckets[hash]++;
    }

    for(i = 0; i < M_VAL; i++) {
        printf("bucket[%u] has %u elements\n", i, buckets[i]);
    }
}
