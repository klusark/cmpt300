/*
 * SSTF.h
 *
 * Created by Joel Teichroeb on 11/25/11
 * for CMPT 300 Project 2.
 *
 * All rights reserved.
 *
 * The shortest seek time first class.
 *
 */
#ifndef SSTF_H
#define SSTF_h

#include "Request.h"


class SSTF : public request {
public:
       SSTF(int atrack, time_t ator, int aduration, HDMonitor *aHD, condition &ac): request(atrack, ator, aduration, aHD, ac){}
       virtual bool operator< (const request & r) const;
};

#endif

