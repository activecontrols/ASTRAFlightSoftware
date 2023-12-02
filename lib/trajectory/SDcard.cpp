/*
 * SDcard.cpp
 *
 * The code below reads a file called inFile
 * and writes to a binary SD card file called outFile
 *
 * Created: 11/11/2023 by Teresa Wan <teresawan04@gmail.com>
 * Last updated: 12/2/2023 by Ishan Goel
 *
 */

#include <SPI.h>
#include <SD.h>

int FILE_READ_ERR = -1;
int FILE_WRITE_ERR = -2;
int NO_DATA_POINTS = -3;

// what do all these vars mean?? refer to the PSRAM structure diagram
int k, p, m, n, N;

// why did we make the decision to use void*?? isn't that bad practice?
// we're having to use void* because we don't know the size yet. the caller will have to cast it to the correct type
// we don't want to use a 1d array with size k*m*n because we want to be able to access it like a 3d array (faster)
// we also want to it to be stored as a contiguous block of memory which is faster and also allows a single fread
// using pointers to VLAs on the heap is the best way to accomplish all of this.
// ask ishan for help if needed

void *vgainM; // cast to float (*gainM)[m][n + N]. Access like you would gainM[p][m][n + N]
void *vqsm; // cast to float (*qsm)[m][n]. Access like you would qsm[3][m][n]
void *vx; // cast to float (*x)[n]. Access like you would x[k][n]
void *vu; // cast to float (*u)[m]. Access like you would u[k][m]
float *t; // Last elem is t[k-1]

typedef struct {
    int k, p, m, n, N;
} Header;


// reading from inFile, will return 0 if successful
int encode(char *inFile, char *outFile) {
  FILE *filePointer = fopen(inFile, "rb");
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
  float (*gainM)[m][n + N] = calloc(p, sizeof *gainM);
  for (int i = 0; i < p; i++)
    for (int j = 0; j < m; j++)
      for (int k = 0; k < (n + N); k++)
        fscanf(filePointer, "%f,", &gainM[i][j][k]);

  // reading quick stabilization matrices
  // currently 3 qsm, may change later
  float (*qsm)[m][n] = calloc(3, sizeof *qsm);
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < m; j++)
      for (int k = 0; k < n; k++)
        fscanf(filePointer, "%f,", &qsm[i][j][k]);

  // read trajectory points
  float (*x)[n] = calloc(k, sizeof *x);
  for (int i = 0; i < k; i++)
    for (int j = 0; j < n; j++)
      fscanf(filePointer, "%f,", &x[i][j]);

  float (*u)[m] = calloc(k, sizeof *u);
  for (int i = 0; i < k; i++)
    for (int j = 0; j < m; j++)
      fscanf(filePointer, "%f,", &u[i][j]);

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

// reading from binary SD card file, will return 0 if successful
int decode(char *inFile) {
  // open file
  FILE *filePointer = fopen(inFile, "rb");
  if (filePointer == NULL) {
    fclose(filePointer);
    filePointer = NULL;
    return FILE_READ_ERR;
  }
  // read header
  static Header header;
  fread(&header, sizeof(header), 1, filePointer);
  k = header.k;
  p = header.p;
  m = header.m;
  n = header.n;
  N = header.N;

  float (*gainM)[m][n + N] = calloc(p, sizeof *gainM);
  fread(gainM, sizeof(*gainM), p, filePointer);
  printf("%ld\n", sizeof(*gainM) * p);
  vgainM = gainM;
  // read quick stabilization matrices. currently 3 qsm, may change later
  float (*qsm)[m][n] = calloc(3, sizeof *qsm);
  fread(qsm, sizeof(*qsm), 3, filePointer);
  vqsm = qsm;

  // read trajectory points
  float (*x)[n] = calloc(k, sizeof *x);
  fread(x, sizeof(*x), k, filePointer);
  vx = x;

  float (*u)[m] = calloc(k, sizeof *u);
  fread(u, sizeof(*u), k, filePointer);
  vu = u;

  t = calloc(k, sizeof *t);
  fread(t, sizeof(*t), k, filePointer);

  fclose(filePointer);
  filePointer = NULL;
  return 0;
}

// This just serves as an example for how to use the decoded data
// In this example, we just write out the plaintext format to a file
void writePlainText() {
  char* outFile = "newplain.txt";
  // convert back to the original format
  FILE *outFilePtr = fopen(outFile, "w");
  if (outFilePtr == NULL) {
    fclose(outFilePtr);
    outFilePtr = NULL;
    return FILE_WRITE_ERR;
  }
  // write header
  fprintf(outFilePtr, "%d,%d,%d,%d,%d\n", k, p, m, n, N);

  // convert void* back to the actual type
  float (*gainM)[m][n + N] = vgainM;
  float (*qsm)[m][n] = vqsm;
  float (*x)[n] = vx;
  float (*u)[m] = vu;

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
