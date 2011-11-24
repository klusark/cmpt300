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
    jobsList = new std::list<request>();
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
    request r(track, time(NULL), duration, this, c);
    jobsList->push_back(r);
    printf("The size was %d\n", jobsList->size() +1);
    printf("Just pushed track %d for %d microseconds\n", track, duration);
    if(numWaitingToWork && jobsList->size() >= WAIT_FOR_X_REQUESTS){
    //if(jobsList->size() && !before && numWaitingToWork) {
        signal(areRequests);
    }
    while(find(jobsList->begin(), jobsList->end(), r) !=
        jobsList->end()){
        wait(c);
    }
    printf("The size is %d\n", jobsList->size() +1);
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
    std::list<request>::iterator nextRequest = min_element(jobsList->begin(), jobsList->end());
    //change direction if necessary
    if((direction == -1 && nextRequest->track > currentTrack) || 
       (direction == 1  && nextRequest->track < currentTrack)) {
        direction *= -1;
        nextRequest = min_element(jobsList->begin(), jobsList->end());
    }
    currentTrack = nextRequest->track;
    printf("Working on track %d for %d micro seconds\n", nextRequest->track, nextRequest->duration);
    int sleepytime = nextRequest->duration;
    jobsList->erase(nextRequest);
    usleep(sleepytime); //Do some "work"
    signal(nextRequest->c);
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

/*
 * Implement the < operator for requests.
 */
bool request::operator< (const request & r) const{
    switch(H->direction){
        case 1:
            return ( request::dist(H->currentTrack, track) < request::dist(H->currentTrack, r.track) &&
                     request::delta(H->currentTrack, track) >= 0 ) ||
                   ( track == r.track && request::delta(H->currentTrack, track) >= 0 &&
                     tor < r.tor);
           break;
        case -1:
            return ( request::dist(H->currentTrack, track) < request::dist(H->currentTrack, r.track) &&
                     request::delta(H->currentTrack, track) <= 0) ||
                   ( track == r.track && request::delta(H->currentTrack, track) <= 0 &&
                     tor < r.tor);
       break;
       default:
            return false;
            break;
    }
}
