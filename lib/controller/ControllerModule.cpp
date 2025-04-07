#include "ControllerModule.h"
#include "settings.h"
// #include "EncoderModule.h"
#include "VEigen.h"
#include <cstdio>
#include <ctime>
extern "C" {
  #include "cpg_workspace.h"
  #include "cpg_solve.h"
}
// #include "cpg_workspace.h"
#include "MathFunctions.h"

int ControllerModule::init() {
  // Set final state costs
  cpg_update_Psqrt(0, 0);
  cpg_update_Psqrt(12*1 + 1, 0);
  cpg_update_Psqrt(12*2 + 2, 0);
  cpg_update_Psqrt(12*3 + 3, 0);
  cpg_update_Psqrt(12*4 + 4, 0);
  cpg_update_Psqrt(12*5 + 5, 0);
  cpg_update_Psqrt(12*6 + 6, 0);
  cpg_update_Psqrt(12*7 + 7, 0);
  cpg_update_Psqrt(12*8 + 8, 0);
  cpg_update_Psqrt(12*9 + 9, 0);
  cpg_update_Psqrt(12*10 + 10, 0);
  cpg_update_Psqrt(12*11 + 11, 0);

  // Set state error costs
  // sqrt(10000) = 100
  // sqrt(2000) = 44.7
  // sqrt(5) = 2.236
  // pos
  cpg_update_Qsqrt(0, 60);
  cpg_update_Qsqrt(12*1 + 1, 60);
  cpg_update_Qsqrt(12*2 + 2, 100);
  // vel
  cpg_update_Qsqrt(12*3 + 3, 60.0);
  cpg_update_Qsqrt(12*4 + 4, 60.0);
  cpg_update_Qsqrt(12*5 + 5, 90.7);
  // ang
  cpg_update_Qsqrt(12*6 + 6, 100);
  cpg_update_Qsqrt(12*7 + 7, 100);
  cpg_update_Qsqrt(12*8 + 8, 2.2360);
  // angvel
  cpg_update_Qsqrt(12*9 + 9, 2);
  cpg_update_Qsqrt(12*10 + 10, 2);
  cpg_update_Qsqrt(12*11 + 11, 2.2360);

  // Set control costs
  cpg_update_Rsqrt(0, 0.1);
  cpg_update_Rsqrt(4*1 + 1, 0.1);
  cpg_update_Rsqrt(4*2 + 2, 0.1);
  cpg_update_Rsqrt(4*3 + 3, 0.1);

  // TODO: Set A and B matrices here
  // Currently A and B are defined as constants in cvx.py, change this to parameter later!

  // Set initial X
  cpg_update_x_init(0, 0);
  cpg_update_x_init(1, 0);
  cpg_update_x_init(2, 0);
  cpg_update_x_init(3, 0);
  cpg_update_x_init(4, 0);
  cpg_update_x_init(5, 0);
  cpg_update_x_init(6, 0);
  cpg_update_x_init(7, 0);
  cpg_update_x_init(8, 0);
  cpg_update_x_init(9, 0);
  cpg_update_x_init(10, 0);
  cpg_update_x_init(11, 0);

  return NO_ERROR_CODE;
}

void ControllerModule::update(unsigned long time) {
  // printf("%lf\n", flightData::estimatedStateX(2));
  std::clock_t start = std::clock();
  this->update_state();
  cpg_solve();
  // Control action
  flightData::controllerInputU(0) = CPG_Result.prim->U[0];
  flightData::controllerInputU(1) = CPG_Result.prim->U[1];
  flightData::controllerInputU(2) = CPG_Result.prim->U[2];
  flightData::controllerInputU(3) = 98.1 + CPG_Result.prim->U[3];
  std::clock_t end = std::clock();
  double elapsed_time = static_cast<double>(end - start) / CLOCKS_PER_SEC;
  printf("Elapsed time: %lf seconds\n", elapsed_time);


  // printf("X: [%lf, %lf, %lf, %lf, %lf, %lf] U: [%lf, %lf, %lf, %lf]\n",
  //   flightData::estimatedStateX(0),
  //   flightData::estimatedStateX(1),
  //   flightData::estimatedStateX(2),
  //   flightData::estimatedStateX(3),
  //   flightData::estimatedStateX(4),
  //   flightData::estimatedStateX(5),
  //   flightData::controllerInputU(0),
  //   flightData::controllerInputU(1),
  //   flightData::controllerInputU(2),
  //   flightData::controllerInputU(3)
  // );
}

void ControllerModule::update_state() {
  // Landing point: [2, 1, 0]
  cpg_update_x_init(0,  flightData::estimatedStateX(0) - 10);
  cpg_update_x_init(1,  flightData::estimatedStateX(1) - 20);
  cpg_update_x_init(2,  flightData::estimatedStateX(2));
  cpg_update_x_init(3,  flightData::estimatedStateX(3));
  cpg_update_x_init(4,  flightData::estimatedStateX(4));
  cpg_update_x_init(5,  flightData::estimatedStateX(5));
  cpg_update_x_init(6,  flightData::estimatedStateX(6));
  cpg_update_x_init(7,  flightData::estimatedStateX(7));
  cpg_update_x_init(8,  flightData::estimatedStateX(8));
  cpg_update_x_init(9,  flightData::estimatedStateX(9));
  cpg_update_x_init(10, flightData::estimatedStateX(10));
  cpg_update_x_init(11, flightData::estimatedStateX(11));
}