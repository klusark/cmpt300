/*
 * Monitor.h
 * Created by Andrew Inwood on 11/15/11
 * Modified by Joel Teichroeb 11/27/11
 * for CMPT 300 Project 2.
 *
 * All rights reserved.
 *
 *
 * Defines the interface for a faked Monitor construct, with synchronization 
 * of shared resources implemented using the pthreads library.
 */
#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>

typedef pthread_cond_t *condition;

class Monitor{
public:
    //Constructor
    Monitor();
    void wait(condition &cond);
    void timedwait(condition &cond, int t);
    void signal(condition cond);
    void EnterMonitor();
    void LeaveMonitor();
    void InitializeCondition(condition &c);
private:
    /*The occupied mutex ensures that only one process is present in the
     *mutex at one time.
     */
    pthread_mutex_t *occupied;
};
#endif
