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
  int FILE_READ_ERR = -1, FILE_WRITE_ERR = -2, NO_DATA_POINTS = -3;
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

  float (*gainM)[m][n + N] = (float (*)[m][n + N]) extmem_malloc(
    p * sizeof(*gainM)); // extmem means PSRAM. malloc slightly faster than calloc
  file.read(gainM, sizeof(*gainM) * p);
  vgainM = gainM;
  // read quick stabilization matrices. currently 3 qsm, may change later
  float (*qsm)[m][n] = (float (*)[m][n]) extmem_malloc(3 * sizeof(*qsm));
  file.read(qsm, sizeof(*qsm) * 3);
  vqsm = qsm;

  // read trajectory points
  float (*x)[n] = (float (*)[n]) extmem_malloc(k * sizeof(*x));
  file.read(x, sizeof(*x) * k);
  vx = x;

  float (*u)[m] = (float (*)[m]) extmem_malloc(k * sizeof(*u));
  file.read(u, sizeof(*u) * k);
  vu = u;

  t = (float *) calloc(k, sizeof *t);
  file.read(t, sizeof(*t) * k);

  file.close();
  return 0;
}

// reading from inFile, will return 0 if successful
int encode(char* inFile,
           char* outFile) {
  // this function doesn't run on the teensy: it generates the file to be flashed to the SD card
  FILE* filePointer = fopen(inFile, "rb");
  if (filePointer == nullptr) return traj::FILE_READ_ERR;
  // reading header
  int k, p, m, n, N;
  // k = p = m = n = N = 0;
  // update format strings depending on input format, may or may not change
  int next = fscanf(filePointer, "%d,%d,%d,%d,%d\n", &k, &p, &m, &n, &N);
  if ((next == 0) || (next == EOF)) {
    fclose(filePointer);
    filePointer = nullptr;
    return traj::NO_DATA_POINTS;
  }
  // reading gain matrices
  float (*gainM)[m][n + N] = (float (*)[m][n + N]) calloc(p, sizeof *gainM);
  for (int i = 0; i < p; i++)
    for (int j = 0; j < m; j++)
      for (int K = 0; K < (n + N); K++) // avoid shadowing
        fscanf(filePointer, "%f,", &gainM[i][j][K]);

  // reading quick stabilization matrices
  // currently 3 qsm, may change later
  float (*qsm)[m][n] = (float (*)[m][n]) calloc(3, sizeof *qsm);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < m; j++)
      for (int K = 0; K < n; K++)
        fscanf(filePointer, "%f,", &qsm[i][j][K]);

  // read trajectory points
  float (*x)[n] = (float (*)[n]) calloc(k, sizeof *x);
  for (int i = 0; i < k; i++)
    for (int j = 0; j < n; j++)
      fscanf(filePointer, "%f,", &x[i][j]);

  float (*u)[m] = (float (*)[m]) calloc(k, sizeof *u);
  for (int i = 0; i < k; i++)
    for (int j = 0; j < m; j++)
      fscanf(filePointer, "%f,", &u[i][j]);

  float (*t) = (float *) calloc(k, sizeof *t);
  for (int i = 0; i < k; i++)
    fscanf(filePointer, "%f,", &t[i]);
  // write out binary format
  FILE* outFilePtr = fopen(outFile, "wb");
  if (outFilePtr == nullptr) {
    fclose(filePointer);
    filePointer = nullptr;
    return traj::FILE_WRITE_ERR;
  }
  // write header
  traj::Header header = {k, p, m, n, N};
  fwrite(&header, sizeof(header), 1, outFilePtr);
  // write gain matrices
  fwrite(gainM, sizeof(*gainM), p, outFilePtr);
  // write quick stabilization matrices
  fwrite(qsm, sizeof(*qsm), 3, outFilePtr);
  // write trajectory points
  fwrite(x, sizeof(*x), k, outFilePtr);
  fwrite(u, sizeof(*u), k, outFilePtr);
  fwrite(t, sizeof(*t), k, outFilePtr);
  // close files
  fclose(outFilePtr);
  outFilePtr = nullptr;
  fclose(filePointer);
  filePointer = nullptr;
  return 0;
}