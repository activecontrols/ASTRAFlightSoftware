/*
 * SDcard.cpp
 *
 * The code below reads a file called inFile
 * and writes to a binary SD card file called outFile
 *
 * Created: 11/11/2023 by Teresa Wan <teresawan04@gmail.com>
 * Last updated: 11/27/2023 by Ishan Goel
 *
 */

#include <SPI.h>
#include <SD.h>

int FILE_READ_ERR = -1;
int FILE_WRITE_ERR = -2;
int NO_DATA_POINTS = -3;

// reading from inFile, will return 0 if successful
int encode(char *inFile, char *outFile) { // this function doesn't run on the teensy: it generates the file to be flashed to the SD card
  FILE *filePointer = fopen(inFile, "rb");
  if (filePointer == NULL) return FILE_READ_ERR;
  // what do all these vars mean?? refer to the PSRAM structure diagram
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
//  float gainM[p][m][n + N];
  float (*gainM)[m][n + N] = calloc(p, sizeof *gainM);
  for (int i = 0; i < p; i++)
    for (int j = 0; j < m; j++)
      for (int k = 0; k < (n + N); k++)
        fscanf(filePointer, "%f,", &gainM[i][j][k]);

  // reading quick stabilization matrices
  // currently 3 qsm, may change later
//  float qsm[3][m][n];
  float (*qsm)[m][n] = calloc(3, sizeof *qsm);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < m; j++)
      for (int k = 0; k < n; k++)
        fscanf(filePointer, "%f,", &qsm[i][j][k]);

  // read trajectory points
//  float x[k][n];
  float (*x)[n] = calloc(k, sizeof *x);
  for (int i = 0; i < k; i++)
    for (int j = 0; j < n; j++)
      fscanf(filePointer, "%f,", &x[i][j]);

//  float u[k][m];
  float (*u)[m] = calloc(k, sizeof *u);
  for (int i = 0; i < k; i++)
    for (int j = 0; j < m; j++)
      fscanf(filePointer, "%f,", &u[i][j]);

//  float t[k];
  float (*t) = calloc(k, sizeof *t);
  for (int i = 0; i < k; i++)
    fscanf(filePointer, "%f,", &t[i]);
  // write out binary format
  FILE *outFilePtr = fopen(outFile, "wb");
  if (outFilePtr == NULL) {
    fclose(filePointer);
    filePointer = NULL;
    return FILE_WRITE_ERR;
  }
  // write header
  struct {
      int k, p, m, n, N
  } header = {k, p, m, n, N};
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

typedef struct {
    int k, p, m, n, N;
} Header;

// reading from binary SD card file, will return 0 if successful
int decode(char *inFile, char *outFile) {
  // open file
  FILE *filePointer = fopen(inFile, "rb");
  if (filePointer == NULL) {
    return FILE_READ_ERR;
  }
  // read header
  static Header header;
  fread(&header, sizeof(header), 1, filePointer);
  int k = header.k;
  int p = header.p;
  int m = header.m;
  int n = header.n;
  int N = header.N;

  // float gainM[p][m][n + N];
  float (*gainM)[m][n + N] = calloc(p, sizeof *gainM);
  fread(gainM, sizeof(*gainM), p, filePointer);
  printf("%ld\n", sizeof(*gainM) * p);
  // read quick stabilization matrices. currently 3 qsm, may change later
  // float qsm[3][m][n];
  float (*qsm)[m][n] = calloc(3, sizeof *qsm);
  fread(qsm, sizeof(*qsm), 3, filePointer);

  // read trajectory points
//  float x[k][n];
  float (*x)[n] = calloc(k, sizeof *x);
  fread(x, sizeof(*x), k, filePointer);
//  float u[k][m];
  float (*u)[m] = calloc(k, sizeof *u);
  fread(u, sizeof(*u), k, filePointer);
//  float t[k];
  float (*t) = calloc(k, sizeof *t);
  fread(t, sizeof(*t), k, filePointer);

  // TODO: change to actual usage
  // convert back to the original format
  FILE *outFilePtr = fopen(outFile, "w");
  if (outFilePtr == NULL) {
    fclose(filePointer);
    filePointer = NULL;
    return FILE_WRITE_ERR;
  }
  // write header
  fprintf(outFilePtr, "%d,%d,%d,%d,%d\n", k, p, m, n, N);
  // write gain matrices
  for (int i = 0; i < p; i++)
    for (int j = 0; j < m; j++)
      for (int k = 0; k < (n + N); k++)
        fprintf(outFilePtr, "%f,", gainM[i][j][k]);
  // write quick stabilization matrices
  // currently 3 qsm, may change later
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < m; j++)
      for (int k = 0; k < n; k++)
        fprintf(outFilePtr, "%f,", qsm[i][j][k]);

  // write trajectory points
  for (int i = 0; i < k; i++)
    for (int j = 0; j < n; j++)
      fprintf(outFilePtr, "%f,", x[i][j]);

  for (int i = 0; i < k; i++)
    for (int j = 0; j < m; j++)
      fprintf(outFilePtr, "%f,", u[i][j]);

  for (int i = 0; i < k; i++)
    fprintf(outFilePtr, "%f,", t[i]);
  // close files
  fclose(outFilePtr);
  outFilePtr = NULL;
  fclose(filePointer);
  filePointer = NULL;
  return 0;
}


/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) { ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
}
