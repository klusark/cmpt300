/*
 * Elevator.h
 *
 * Created by Joel Teichroeb on 11/25/11
 * for CMPT 300 Project 2.
 *
 * All rights reserved.
 *
 * The elevator class.
 *
 */
#ifndef ELEVATOR_H
#define ELEVATOR_h

#include "Request.h"


class Elevator : public request {
public:
       Elevator(int atrack, time_t ator, int aduration, HDMonitor *aHD, condition &ac): request(atrack, ator, aduration, aHD, ac){}
       virtual bool operator< (const request & r) const;
};

#endif

