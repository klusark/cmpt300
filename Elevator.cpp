/*
 * Elevator.cpp
 *
 * Created by Joel Teichroeb on 11/25/11
 * for CMPT 300 Project 2.
 *
 * All rights reserved.
 *
 * The Elevator class as defined in Elevator.h.
 *
 */
#include "Elevator.h"
#include "HDMonitor.h"

/*
 * Implement the < operator for requests.
 */
bool Elevator::operator< (const request &r) const{
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
