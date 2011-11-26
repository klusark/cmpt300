#include "HDMonitor.h"
#include <cstdio>
#define NUM_TRACKS 15
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#define WAIT_TIME 1
static int NUM_THREADS = 100;
static int NUM_WORK_THREADS = 1;
using namespace std;

pthread_mutex_t readmutex;

void *Schedule(void* Mon);
void *DoNext(void* Mon);
int main(int argc, char *argv[]){
    if(argc > 2) {
        NUM_THREADS = atoi(argv[1]);
        NUM_WORK_THREADS = atoi(argv[2]);
    }
    HDMonitor M(NUM_TRACKS);
    pthread_t threads[NUM_THREADS + NUM_WORK_THREADS];
    int rc;
    long t;
    void* status;

    pthread_mutex_init(&readmutex, 0);

    for(t=0; t < NUM_WORK_THREADS ; t++){
      rc = pthread_create(&threads[t], NULL, DoNext,  &M);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
    }
    for(t=NUM_WORK_THREADS; t<NUM_THREADS + NUM_WORK_THREADS ; t++){
      rc = pthread_create(&threads[t], NULL, Schedule, & M);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
    }
    //rc = pthread_create(&threads[NUM_THREADS-1], NULL, DoNext, & M);

    if (rc){
     printf("ERROR; return code from pthread_create() is %d\n", rc);
     exit(-1);
    }
    for(t = 0; t < NUM_THREADS + NUM_WORK_THREADS; ++t){
        rc = pthread_join(threads[t], &status);
    }
    return 0;
}


void* Schedule(void* Mon){
    int track, duration;
    FILE* TimingFP = fopen("TimePerRequest.txt", "w");
    fclose(TimingFP);
    HDMonitor* M = (HDMonitor*) Mon;
    while(true){
	pthread_mutex_lock(&readmutex);
	int ret = scanf("%d %d", &track, &duration);
	pthread_mutex_unlock(&readmutex);
        if( ret == 2){
            clock_t start = clock();
            M->Request(track, duration);
            int N;
            M->NumberOfRequests(N);
            TimingFP = fopen("TimePerRequest.txt", "a");
            fprintf(TimingFP, "%d %8.4f\n", N+1, (clock()/(double)CLOCKS_PER_SEC) -
                                        start/(double)CLOCKS_PER_SEC);

            fclose(TimingFP);
            usleep(rand() % WAIT_TIME);
        }
    }
}
void* DoNext(void* Mon){
    HDMonitor* M = (HDMonitor*) Mon;
    while(true){
        M->DoNextJob();
    }
}
