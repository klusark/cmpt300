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
#include <cstdio>
using namespace std;
Monitor::Monitor(){
    pthread_mutex_init(&occupied, NULL);
}
void Monitor::InitializeCondition(condition &c){
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
    /*
    if(!condMutexes.count(cond)){
        cond = new pthread_cond_t();
        pthread_cond_init(cond, NULL);
        pthread_mutex_t *mut = new pthread_mutex_t();
        pthread_mutex_init(mut, NULL);
        condMutexes.insert(pair<condition, pthread_mutex_t*>(cond, mut)); 
    }
    */
    //pthread_mutex_unlock(&occupied);
    int ret;
    //if((ret = pthread_cond_wait(cond, condMutexes[cond]))){
    if((ret = pthread_cond_wait(cond, &occupied))){
        printf ("wait failed with %d\n", ret);
    }
    //pthread_mutex_lock(&occupied);
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
    pthread_mutex_unlock(&occupied);
    //pthread_mutex_unlock(condMutexes[cond]);
}

/*
 * EnterMonitor().
 * When EnterMonitor() completes, the calling thread has control of the Monitor.
 * This function must be the first line of all functions defined in the Monitor,
 * and enables faking the Monitor construct in C++.
 */
void Monitor::EnterMonitor(){
    pthread_mutex_lock(&occupied);
}

/*
 * LeaveMonitor().
 * When LeaveMonitor() completes, the Monitor is empty.
 *
 */
void Monitor::LeaveMonitor(){
    pthread_mutex_unlock(&occupied);
}
