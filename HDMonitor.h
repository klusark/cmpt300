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

#ifndef HD_MONITOR_H
#define HD_MONITOR_H
#include "Monitor.h"
#include <list>      //for request list
#include <ctime>     //for request time

const int WAIT_FOR_X_REQUESTS = 5;
const int WAIT_X_SECONDS = 1;
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
        static int dist(int a, int b){ return a < b ? b - a : a - b;}
        static int delta(int a, int b) { return b - a;}
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
        HDMonitor(int N);
        ~HDMonitor();

        /*
         * Request(int track, int duration)
         * Creates a new request to the hard drive, and is put on the queue for
         * scheduling.
         */
	void Request(int track, int duration);
        /*
         * DoNextJob().
         * Selects the next job from the list based on the scheduling rule.
         *
         */
        void DoNextJob();
        void NumberOfRequests(int &N);
        friend class request; //Allow request to access currentTrack
    private:
        int numWaitingToWork;
        int direction; //direction of read/write head {-1, 1}
        int currentTrack; //track that read/write head is on [1, N]
        int numTracks; // Equal to N
        std::list<request>  *jobsList;
};
#endif
