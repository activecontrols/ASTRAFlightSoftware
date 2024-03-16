#include "Error.h"
#include "timer.h"

#include <elapsedMillis.h>

/*
Timer.cpp
Description: Defines functions for the global timer
Author: Vincent Palmerio
*/
namespace timer {
    elapsedMicros missionTimer;

    bool g_missionStarted = false;

    int startMissionTimer() {

        g_missionStarted = true;
        missionTimer = 0;

        return NO_ERROR_CODE;
    }

    float getMissionTimeSeconds() {
        if (g_missionStarted == false) {
            return CANNOT_REQUEST_MISSION_TIME_IF_MISSION_NOT_STARTED;
        }

        return (missionTimer / MICROSECONDS_IN_A_SECOND);
    }
}