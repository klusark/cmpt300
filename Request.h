#ifndef REQUEST_H
#define REQUEST_H

#include <ctime>
#include "Monitor.h"

class HDMonitor;

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
       virtual bool operator< (const request & r) const = 0;
       bool operator==(const request & r) const{
            return track == r.track && tor == r.tor && duration == r.duration && H
            == r.H && c == r.c;
       }
        protected:
            HDMonitor *H;
};


#endif

