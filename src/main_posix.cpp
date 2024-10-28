// Include guard - don't build this file if building for arduino
#if IS_ARDUINO
#else

#include "Scheduler.h"
#include "Router.h"
#include "FakeIMU.h"
#include "FlightModule.h"
#include <vector>
#include <chrono>
#include <iostream>

namespace flightData {
  Eigen::VectorXd measurementVectorY(9);
  Router *router;
}

FakeIMUModule fakeIMU;

std::vector<FlightModule*> basicSchedule = {
    (FlightModule*) &fakeIMU,
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
    std::cout << flightData::measurementVectorY.reshaped(1, 9).format(CleanFmt) << "\n";
  }
}


#endif // IS_ARDUINO