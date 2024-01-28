#include <SDcard.h>

// This is just an example for how to cast and access/set the decoded data
// (if you actually want to clear data, you can free the pointers and set to a
// newly allocated array)
void clearAllData() {
  // convert void* back to the actual type
  float (*gainM)[traj::m][traj::n + traj::N] = (float (*)[traj::m][traj::n + traj::N]) traj::vgainM;
  float (*qsm)[traj::m][traj::n] = (float (*)[traj::m][traj::n]) traj::vqsm;
  float (*x)[traj::n] = (float (*)[traj::n]) traj::vx;
  float (*u)[traj::m] = (float (*)[traj::m]) traj::vu;

  traj::k = traj::p = traj::m = traj::n = traj::N = 0;

  // write gain matrices
  for (int i = 0; i < traj::p; i++)
    for (int j = 0; j < traj::m; j++)
      for (int k = 0; k < (traj::n + traj::N); k++)
        gainM[i][j][k] = 0;

  // write quick stabilization matrices
  // currently 3 qsm, may change later
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < traj::m; j++)
      for (int k = 0; k < traj::n; k++)
        qsm[i][j][k] = 0;

  // write trajectory points
  for (int i = 0; i < traj::k; i++)
    for (int j = 0; j < traj::n; j++)
      x[i][j] = 0;

  for (int i = 0; i < traj::k; i++)
    for (int j = 0; j < traj::m; j++)
      u[i][j] = 0;

  for (int i = 0; i < traj::k; i++)
    traj::t[i] = 0;
}
