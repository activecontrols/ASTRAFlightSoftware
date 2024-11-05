/*
  Decoder.h - Decoder Header file
  Description: Header file to Decoder.cpp
  Created: 11/4/2024 by Jack Fang <fang355@purdue.edu>
  Last updated: 11/4/2024 by Jack Fang <fang355@purdue.edu>
*/


#ifndef DECODER_H
#define DECODER_H

#include <Arduino.h>
#include <SD.h>

#include <iostream>
#include <string>

using namespace std;

enum DataType {
  STRING,
  PERSON,
  EXAMPLE1,
  EXAMPLE2,
};

#endif // DECODER_H