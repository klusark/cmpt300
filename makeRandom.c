#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 1000
#define M 15

int main(void){
    srand(time(NULL));
    int i;
    for(i = 0; i < N; ++i){
        printf("%d 100\n", rand() % M + 1);
    }
    return 0;
}
