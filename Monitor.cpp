/*
 * Monitor.cpp
 *
 * Created by Andrew Inwood on 11/15/11
 * for CMPT 300 Project 2.
 *
 * All rights reserved.
 *
 * Implementation of methods of the Monitor class contained in Monitor.h
 *
 */
#include "Monitor.h"
#include <pthread.h>
#include <ctime>
#include <cstdio>
using namespace std;
Monitor::Monitor(){
    occupied = new pthread_mutex_t();
    pthread_mutex_init(occupied, NULL);
}
void Monitor::InitializeCondition(condition &c){
    c = new pthread_cond_t();
    pthread_cond_init(c, NULL);
}

/*
 * wait(condition cond).
 * 
 * The executing thread releases the monitor, and is put to sleep until the
 * condition variable is signalled. The first time a condition variable is
 * passed to wait, it is initialized and associated with a mutex.
 */
void Monitor::wait(condition &cond){
    int ret;
    if((ret = pthread_cond_wait(cond, occupied))){
        printf ("wait failed with %d\n", ret);
    }
}
void Monitor::timedwait(condition &cond, int t){
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_nsec = ts.tv_nsec + t;
    pthread_cond_timedwait(cond, occupied, &ts);
}

/*
 * signal(condition cond).
 * Signals all threads waiting on condition variable cond. The system selects
 * one thread to execute once the Monitor is released.
 *
 */
void Monitor::signal(condition cond){
    //pthread_mutex_lock(condMutexes[cond]);
    pthread_cond_broadcast(cond);
    //pthread_mutex_unlock(&occupied);
    //pthread_mutex_unlock(condMutexes[cond]);
}

/*
 * EnterMonitor().
 * When EnterMonitor() completes, the calling thread has control of the Monitor.
 * This function must be the first line of all functions defined in the Monitor,
 * and enables faking the Monitor construct in C++.
 */
void Monitor::EnterMonitor(){
    while(pthread_mutex_trylock(occupied));
}

/*
 * LeaveMonitor().
 * When LeaveMonitor() completes, the Monitor is empty.
 *
 */
void Monitor::LeaveMonitor(){
    int ret;
    if((ret = pthread_mutex_unlock(occupied))){
        printf("Mutex unlock returned an error: $d\n", ret);
    }
}
