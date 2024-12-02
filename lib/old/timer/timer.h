#ifndef TIMER_H
#define TIMER_H

#include <elapsedMillis.h>
#include "Error.h"

/*
Timer.h
Description: Global timer for the mission
Author: Vincent Palmerio
*/

#define MICROSECONDS_IN_A_SECOND (1000000)

namespace timer {
    
    /*
    * Starts mission timer. Timer is in microseconds
    */
    extern int startMissionTimer();

    /*
    * Returns mission time in seconds, CANNOT_REQUEST_MISSION_TIME_IF_MISSION_NOT_STARTED if
    * function is called before mission is started.
    */
    extern float getMissionTimeSeconds();
}

#endif