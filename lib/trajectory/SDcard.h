/*
  SDcard.h - SDcard Header file
  Description: Header file to SDcard.cpp
  Created: 12/2/2023 by Ishan Goel <goeli@purdue.edu>
  Last updated: 12/4/2023 by Ishan Goel <goeli@purdue.edu>
*/

#ifndef TRAJ_H
#define TRAJ_H

namespace traj {
    extern int FILE_READ_ERR, FILE_WRITE_ERR, NO_DATA_POINTS;

  // what do all these vars mean?? refer to the PSRAM structure diagram
    extern int k, p, m, n, N;

  // why did we make the decision to use void*?? isn't that bad practice?
  // we're having to use void* because we don't know the size yet. the caller will have to cast it to the correct type
  // we don't want to use a 1d array with size k*m*n because we want to be able to access it like a 3d array (faster)
  // we also want to it to be stored as a contiguous block of memory which is faster and also allows a single fread
  // using pointers to VLAs on the heap is the best way to accomplish all of this.
  // see SDcard.cpp -> clearAllData() for an example of how to cast and access the data
  // ask ishan for help if needed

    extern void *vgainM; // cast to float (*gainM)[m][n + N]. Access like you would gainM[p][m][n + N]
    extern void *vqsm; // cast to float (*qsm)[m][n]. Access like you would qsm[3][m][n]
    extern void *vx; // cast to float (*x)[n]. Access like you would x[k][n]
    extern void *vu; // cast to float (*u)[m]. Access like you would u[k][m]
    extern float *t; // Last elem is t[k-1]

    typedef struct {
        int k, p, m, n, N;
    } Header;

  // int encode(char *inFile, char *outFile); // runs on the ground station

    int decode(char *inFile);
}

#endif