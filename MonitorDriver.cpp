/*
 * MonitorDriver.cpp
 *
 * Created by Andrew Inwood on 11/15/11
 * Modified by Joel Teichroeb 11/27/11
 * for CMPT 300 Project 2.
 *
 * All rights reserved.
 *
 * 
 *
 */

#include "HDMonitor.h"
#include <cstdio>
#define NUM_TRACKS 15
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <map>
#define WAIT_TIME 1
static int NUM_THREADS = 100;
static int NUM_WORK_THREADS = 1;
using namespace std;

// The mutex used so that only one thread calls scanf at a time
pthread_mutex_t readmutex;

void *Schedule(void* Mon);
void *DoNext(void* Mon);
int main(int argc, char *argv[]){
    if(argc > 2) {
        NUM_THREADS = atoi(argv[1]); //Number of threads are command line arguments
        NUM_WORK_THREADS = atoi(argv[2]);
    }
    HDMonitor M(NUM_TRACKS);
    pthread_t threads[NUM_THREADS + NUM_WORK_THREADS];
    int rc;
    long t;
    void* status;
    pthread_mutex_init(&readmutex, 0);

    // Create the worker threads
    for(t=0; t < NUM_WORK_THREADS ; t++){
      rc = pthread_create(&threads[t], NULL, DoNext,  &M);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
    }

    // Create the request threads
    for(t=NUM_WORK_THREADS; t<NUM_THREADS + NUM_WORK_THREADS ; t++){
      rc = pthread_create(&threads[t], NULL, Schedule, & M);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
    }

    if (rc){
     printf("ERROR; return code from pthread_create() is %d\n", rc);
     exit(-1);
    }

    // Wait for all the request threads to finish before exiting the program
    for(t = NUM_WORK_THREADS; t < NUM_THREADS + NUM_WORK_THREADS; ++t){
        rc = pthread_join(threads[t], &status);
    }
    return 0;
}

void *Schedule(void *Mon){
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
            int N;    //These variables will hold test data
            double T;
            int  turns, distance;
            M->Request(track, duration, N, T, turns, distance);
            TimingFP = fopen("TimePerRequest.txt", "a");
            fprintf(TimingFP, "%d %16.14f %d %d\n", N, T, turns, distance);

            fclose(TimingFP);
            usleep(rand() % WAIT_TIME);
        } else if (ret == EOF) {
            // if scanf outputs that there is no more data left stop the thread
            break;
        }
    }
}

void* DoNext(void* Mon){
    HDMonitor* M = (HDMonitor*) Mon;
    while(true){
        M->DoNextJob(); //Asks for a request to be serviced indefinitely
    }
}
