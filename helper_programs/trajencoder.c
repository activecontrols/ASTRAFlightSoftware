//
// Created by Ishan Goel on 2/24/24.
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
  int k, p, m, n, N;
} Header;

int FILE_READ_ERR = -1;
int FILE_WRITE_ERR = -2;
int NO_DATA_POINTS = -3;

int encode(char *inFile, char *outFile);

int main() {
  clock_t tm;
  tm = clock();

  int err = encode("traj.txt", "1.traj");
  if (err) {
    printf("Error: %d\n", err);
    return err;
  }

  tm = clock() - tm;
  printf("Encoded in %.3f seconds into 1.traj \n", ((double)tm)/CLOCKS_PER_SEC);
}

// reading from inFile, will return 0 if successful
int encode(char* inFile, char* outFile) { // TODO: more error checking
  // this function doesn't run on the teensy: it generates the file to be flashed to the SD card
  FILE* filePointer = fopen(inFile, "rb");
  if (filePointer == NULL) return FILE_READ_ERR;
  // reading header
  int k, p, m, n, N;
  // k = p = m = n = N = 0;
  // update format strings depending on input format, may or may not change
  int next = fscanf(filePointer, "%d,%d,%d,%d,%d\n", &k, &p, &m, &n, &N);
  if ((next == 0) || (next == EOF)) {
    fclose(filePointer);
    filePointer = NULL;
    return NO_DATA_POINTS;
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

  printf("Done reading. Now writing\n");

  // write out binary format
  FILE* outFilePtr = fopen(outFile, "wb");
  if (outFilePtr == NULL) {
    fclose(filePointer);
    filePointer = NULL;
    return FILE_WRITE_ERR;
  }
  // write header
  Header header = {k, p, m, n, N};
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
  outFilePtr = NULL;
  fclose(filePointer);
  filePointer = NULL;
  return 0;
}
