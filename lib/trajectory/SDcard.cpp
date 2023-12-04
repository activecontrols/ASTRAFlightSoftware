/*
 * SDcard.cpp
 *
 * The code below reads a file called inFile
 * and writes to a binary SD card file called outFile
 *
 * Created: 11/11/2023 by Teresa Wan <teresawan04@gmail.com>
 * Last updated: 12/4/2023 by Ishan Goel
 *
 */

#include <SPI.h>
#include <SD.h>
#include <./SDcard.h>

// reading from binary SD card file, will return 0 if successful
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
  // printf("%ld\n", sizeof(*gainM) * p);
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

// This is just an example for how to cast and access/set the decoded data
// (if you actually want to clear data, you can free the pointers and set to a
// newly allocated array)
void clearAllData() {
  // convert void* back to the actual type
  // TODO: check if static cast still works
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
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
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
