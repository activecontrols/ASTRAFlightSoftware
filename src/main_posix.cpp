// Include guard - don't build this file if building for arduino
#if IS_ARDUINO
#else

#include "Scheduler.h"
#include "Router.h"
#include "FakeIMU.h"
#include "MEKFEstimatorModule.h"
#include "Comms.h"
#include "FlightModule.h"
#include <vector>
#include <chrono>
#include <iostream>

namespace flightData {
  Eigen::VectorXd measurementVectorY(9);
  Eigen::VectorXd estimatedStateX(6);
  float voltage[1] = {12.6f};
  Router *router;
}

FakeIMUModule fakeIMU;
MEKFEstimatorModule estimator;
CommsManager comms;

std::vector<FlightModule*> basicSchedule = {
    (FlightModule*) &fakeIMU,
    (FlightModule*) &estimator,
    (FlightModule*) &comms,
};
Scheduler scheduler(basicSchedule);
Router centralRouter;

Eigen::IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]");

uint64_t micros() {
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}

int main(int argc, char** argv) {
  flightData::router = &centralRouter;
  centralRouter.registerSchedule(ASTRA_MAINLOOP, &scheduler);
  centralRouter.changeSchedule(ASTRA_MAINLOOP);

  centralRouter.init();

  while (1) {
    centralRouter.update(micros());
    // std::cout << flightData::estimatedStateX.reshaped(1, 6).format(CleanFmt) << "\n";
  }
}


#endif // IS_ARDUINO