#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 1000
#define M 15

int main(int argc, char * argv[]){
    srand(time(NULL));
    int i;
    int n = N;
    if(argc > 1){
        n = atoi(argv[1]);
    }
    for(i = 0; i < n; ++i){
        printf("%d 100\n", rand() % M + 1);
    }
    return 0;
}
