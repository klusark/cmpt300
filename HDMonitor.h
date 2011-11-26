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
#include "Request.h"

const int WAIT_FOR_X_REQUESTS = 5;
const int WAIT_X_NSECONDS = 1000000;
class HDMonitor; //forward declaration

typedef std::list<request *> RequestList;
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
//    private:
        int numWaitingToWork;
        int direction; //direction of read/write head {-1, 1}
        int currentTrack; //track that read/write head is on [1, N]
        int numTracks; // Equal to N
        RequestList  *jobsList;
};
#endif
