/*
 * SDcard.cpp
 *
 * The code below reads a file called inFile
 * and writes to a binary SD card file called outFile
 *
 * Created: 11/11/2023 by Teresa Wan <teresawan04@gmail.com>
 * Last updated: 12/4/2023 by Ishan Goel <goeli@purdue.edu>
 *
 */

#include <SPI.h>
#include <SD.h>
#include <SDcard.h>

// reading from binary SD card file, will return 0 if successful
namespace traj {
  int FILE_READ_ERR = -1, FILE_WRITE_ERR = -2, NO_DATA_POINTS = -3, MALLOC_ERR = -4;
  int k, p, m, n, N;
  void *vgainM, *vqsm, *vx, *vu;
  float *t;
}

// traj::decode takes in the name of the file to decode and returns an error if any.
// data read from the file is decoded into the variables defined in the traj namespace.
// see clearAllData() in trajectory/example.cpp for an example of how to cast and use the variables.
int traj::decode(char* inFile) {
  // TODO: error out if the amount of bytes asked for is not the same as the amount of bytes read
  // open file
  File file = SD.open(inFile, FILE_READ);
  if (!file) return FILE_READ_ERR;
  // read header
  Header header;
  file.read(&header, sizeof(header));
  k = header.k;
  p = header.p;
  m = header.m;
  n = header.n;
  N = header.N;

  float (*gainM)[m][n + N] = (float (*)[m][n + N]) extmem_malloc(p * sizeof(*gainM)); // extmem means PSRAM. malloc slightly faster than calloc
  if (gainM == nullptr) return MALLOC_ERR;
  file.read(gainM, sizeof(*gainM) * p);
  vgainM = gainM;
  // read quick stabilization matrices. currently 3 qsm, may change later
  float (*qsm)[m][n] = (float (*)[m][n]) extmem_malloc(3 * sizeof(*qsm));
  if (qsm == nullptr) return MALLOC_ERR;
  file.read(qsm, sizeof(*qsm) * 3);
  vqsm = qsm;

  // read trajectory points
  float (*x)[n] = (float (*)[n]) extmem_malloc(k * sizeof(*x));
  if (x == nullptr) return MALLOC_ERR;
  file.read(x, sizeof(*x) * k);
  vx = x;

  float (*u)[m] = (float (*)[m]) extmem_malloc(k * sizeof(*u));
  if (u == nullptr) return MALLOC_ERR;
  file.read(u, sizeof(*u) * k);
  vu = u;

  t = (float *) calloc(k, sizeof *t);
  if (t == nullptr) return MALLOC_ERR;
  file.read(t, sizeof(*t) * k);

  file.close();
  return 0;
}