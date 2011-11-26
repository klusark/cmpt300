#include "SSTF.h"
#include "HDMonitor.h"

/*
 * Implement the < operator for requests.
 */
bool SSTF::operator< (const request &r) const{
	return dist(H->currentTrack, track) < dist(H->currentTrack, r.track);
}
