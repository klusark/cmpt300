/*
 * HDMonitor.cpp
 *
 * Created by Andrew Inwood on 11/15/11
 * for CMPT 300 Project 2.
 *
 * All rights reserved.
 *
 * The HDMonitor provides the interface to a hard drive simulation, consisting
 * of N tracks and a moveable read/write head. Users of the HDMonitor can
 * request IO operations on individual tracks, and request that a queued IO
 * operation be performed.
 *
 * This Monitor schedules the next IO request based on the elevator algorithm.
 *
 */

#include "Monitor.h"
#include "HDMonitor.h"
#include <algorithm> //for min_element
#include <cstdio>    //for output messages
#include <list>      //for request list
#include <ctime>     //for request time
#include <unistd.h>

#include "SSTF.h"
#include "Elevator.h"

using namespace std;

/*
 * HDMonitor (Hard Drive Monitor)
 *
 */
HDMonitor::HDMonitor(int N){
    numWaitingToWork = 0;
    direction = 1;
    currentTrack = 1;
    numTracks = N;
    jobsList = new RequestList();
    //InitializeCondition(areRequests);
    areRequests = new pthread_cond_t();
    pthread_cond_init(areRequests, NULL);
}
HDMonitor::~HDMonitor(){
    delete jobsList;
}

/*
 * Request(int track, int duration)
 * Creates a new request to the hard drive, and is put on the queue for
 * scheduling.
 */
void HDMonitor::Request(int track, int duration){
    EnterMonitor();
    int before = jobsList->size();
    condition c;
    InitializeCondition(c);
    request *r = new SSTF(track, time(NULL), duration, this, c);
    jobsList->push_back(r);
    //printf("The size was %d\n", jobsList->size() +1);
    printf("Just pushed track %d for %d microseconds\n", track, duration);
    if(numWaitingToWork && jobsList->size() >= WAIT_FOR_X_REQUESTS){
    //if(jobsList->size() && !before && numWaitingToWork) {
        signal(areRequests);
    }
    while(find(jobsList->begin(), jobsList->end(), r) !=
        jobsList->end()){
        wait(c);
    }
    //printf("The size is %d\n", jobsList->size() +1);
    LeaveMonitor();
}
/*
 * DoNextJob().
 * Selects the next job from the list based on the scheduling rule.
 *
 */
void HDMonitor::DoNextJob(){
    EnterMonitor();
    if(!jobsList->size()) { //wait until requests are available
        ++numWaitingToWork;
        //The loop is necessary, or else pthreads will wake up a thread
        //that has been inactive for some time.
        while(!jobsList->size()){
            printf("going to wait\n");
            //wait(areRequests);
            timedwait(areRequests, WAIT_X_SECONDS);
        }
        --numWaitingToWork;
    }
    //get next job
    RequestList::iterator nextRequest = min_element(jobsList->begin(), jobsList->end());
    request *r = *nextRequest;
    //change direction if necessary
    if((direction == -1 && r->track > currentTrack) || 
       (direction == 1  && r->track < currentTrack)) {
        direction *= -1;
        nextRequest = min_element(jobsList->begin(), jobsList->end());
	r = *nextRequest;
    }
    currentTrack = r->track;
    //printf("Working on track %d for %d micro seconds\n", r->track, r->duration);
    int sleepytime = r->duration;
    jobsList->erase(nextRequest);
    signal(r->c);
    delete r;
    usleep(sleepytime); //Do some "work"
    LeaveMonitor();
}

/*
 * NumberOfRequests()
 * Returns the number of requests pending
 *
 */
 void HDMonitor::NumberOfRequests(int & N){
    EnterMonitor();
    N = jobsList->size();
    LeaveMonitor();
 }


