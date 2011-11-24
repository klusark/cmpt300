/*
 * HDMonitor.h
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

#ifndef HDMONITOR_H
#define HDMONITOR_H
#include "Monitor.h"
#include <algorithm> //for min_element
#include <cstdio>    //for output messages
#include <list>      //for request list
#include <ctime>     //for request time
#include <unistd.h>  //for usleep
#include <ctime>

//Helper functions for the complex comparison operator (elevator algorithm)
int dist(int a, int b){ return a < b ? b - a : a - b;}
int delta(int a, int b) { return b - a;}
const int WAIT_FOR_X_REQUESTS = 5;
class HDMonitor; //forward declaration
/*
 * request
 * The HDMonitor encapsulates an IO request as an instance of the request class,
 * and maintains a list of these objects. 
 *
 */
class request {
    public:
        int track;
        time_t tor; //Time of Request
        int duration;
        condition &c;
        request(int atrack, time_t ator, int aduration, HDMonitor *aHD, condition &ac):c (ac){
            track = atrack;
            tor = ator;
            duration = aduration;
            H = aHD; //The comparison operator needs the currentTrack from H
        }
        /*
         * operator<
         * This is required to find the next request.
         */
       bool operator< (const request & r) const;
       // bool operator() (request & a, request &b);
       bool operator==(const request & r) const{
            return track == r.track && tor == r.tor && duration == r.duration && H
            == r.H && c == r.c;
       }
        private:
            HDMonitor *H;
};
/*
 * HDMonitor (Hard Drive Monitor)
 *
 */
class HDMonitor : protected Monitor{
    public:
        condition areRequests; //Allows the scheduler to wait for requests
        //Constructors
        HDMonitor(){
            numWaitingToWork = 0;
            direction = 1;
            currentTrack = 1;
            numTracks = 0;
            jobsList = new std::list<request>();
            areRequests = new pthread_cond_t();
            pthread_cond_init(areRequests, NULL);
        }
        HDMonitor(int N){
            numWaitingToWork = 0;
            direction = 1;
            currentTrack = 1;
            numTracks = N;
            jobsList = new std::list<request>();
            //InitializeCondition(areRequests);
            areRequests = new pthread_cond_t();
            pthread_cond_init(areRequests, NULL);
        }
        ~HDMonitor(){
            delete jobsList;
        }

        /*
         * Request(int track, int duration)
         * Creates a new request to the hard drive, and is put on the queue for
         * scheduling.
         */
	void Request(int track, int duration, int & N){
            EnterMonitor();
            int before = jobsList->size();
            condition c;
            InitializeCondition(c);
            request r(track, time(NULL), duration, this, c);
            jobsList->push_back(r);
            printf("Just pushed track %d for %d microseconds\n", track, duration);
            if(numWaitingToWork && jobsList->size() == WAIT_FOR_X_REQUESTS){
            //if(jobsList->size() && !before && numWaitingToWork) {
                signal(areRequests);
            }
            while(find(jobsList->begin(), jobsList->end(), r) !=
                jobsList->end()){
                wait(c);
            }
            N = jobsList->size();
            LeaveMonitor();
	}
        /*
         * DoNextJob().
         * Selects the next job from the list based on the scheduling rule.
         *
         */
        void DoNextJob(){
            EnterMonitor();
            if(!jobsList->size()) { //wait until requests are available
                ++numWaitingToWork;
                //The loop is necessary, or else pthreads will wake up a thread
                //that has been inactive for some time.
                while(!jobsList->size()){
                    printf("going to wait\n");
                    //wait(areRequests);
                    timedwait(areRequests, WAIT_FOR_X_REQUESTS);
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
        friend class request; //Allow request to access currentTrack


    protected:
    private:
        int numWaitingToWork;
        int direction; //direction of read/write head {-1, 1}
        int currentTrack; //track that read/write head is on [1, N]
        int numTracks; // Equal to N

        std::list<request>  *jobsList;
};

/*
 * Implement the < operator for requests.
 */
bool request::operator< (const request & r) const{
    switch(H->direction){
        case 1:
            return ( dist(H->currentTrack, track) < dist(H->currentTrack, r.track) &&
                     delta(H->currentTrack, track) >= 0 ) ||
                   ( track == r.track && delta(H->currentTrack, track) >= 0 &&
                     tor < r.tor);
           break;
        case -1:
            return ( dist(H->currentTrack, track) < dist(H->currentTrack, r.track) &&
                     delta(H->currentTrack, track) <= 0) ||
                   ( track == r.track && delta(H->currentTrack, track) <= 0 &&
                     tor < r.tor);
       break;
       default:
            return false;
            break;
    }
}
#endif
