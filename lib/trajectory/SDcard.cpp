/*
 * SDcard.cpp
 *
 * The code below reads a file called inFile
 * and writes to a binary SD card file called outFile
 *
 * Created: 11/11/2023 by Teresa Wan <teresawan04@gmail.com>
 * Last updated: 11/11/2023 by Teresa Wan
 *
 */

#include <SPI.h>
#include <SD.h>

int FILE_READ_ERR = -1;
int NO_DATA_POINTS = -2;

// reading from inFile, will return 0 if successful

int read (char *inFile) {
  FILE* filePointer = fopen(inFile, "r");
  if (filePointer == NULL) {
    return FILE_READ_ERR;
  }
  // what do all these vars mean?? refer to the PSRAM structure diagram
  // reading header
  int k = 0;
  int p = 0;
  int m = 0;
  int n = 0;
  int N = 0;
  // update format strings depending on imput format, may or may not change
  int next = fscanf(filePointer,"%d,%d,%d,%d,%d\n", &k, &p, &m, &n, &N);
  if ((next == 0) || (next == EOF)) {
    fclose(filePointer);
    filePointer = NULL;
    return NO_DATA_POINTS;
  }
  // reading gain matrices
  float gainM[p][m][n + N];
  for (int i = 0; i < p; i++) {
    for (int j = 0; j < m; j++) {
      for (int k = 0; k < (n + N); k++) {
        fscanf(filePointer, "%d,", gainM[i][j][k];
      }
    }
  }
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
