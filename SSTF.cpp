/*
 * SSTF.cpp
 *
 * Created by Joel Teichroeb on 11/25/11
 * for CMPT 300 Project 2.
 *
 * All rights reserved.
 *
 * The SSTF class as defined in SSTF.h.
 *
 */
#include "SSTF.h"
#include "HDMonitor.h"

/*
 * Implement the < operator for requests.
 */
bool SSTF::operator< (const request &r) const{
	return dist(H->currentTrack, track) < dist(H->currentTrack, r.track);
}

