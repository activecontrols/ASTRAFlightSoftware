/**
 * This file contains the template definition for the FlightModule.
 * Each module in the ASTRA flight software system must implement a FlightModule to be run.
 * A Scheduler will run a loop which calls init() on startup and update() on loop.
 */
#ifndef FLIGHTMODULE_H
#define FLIGHTMODULE_H
class FlightModule {
public:
    /**
     * Initialize module during startup. This might e.g. load data, initialize an I2C device, etc.
     */
    virtual void init() {}

    /**
     * Run an update during mainloop. This might e.g. update the estimator, read data, etc.
     */
    virtual void update() {}
};

#endif // FLIGHTMODULE_H