#pragma once
#ifndef SDHANDELER_H
#define SDHANDELER_H
#include "FS.h"
//#include "SPI.h"
void readFile(fs::FS &fs, const char * path, String*param);
void writeFile(fs::FS &fs, const char * path, String message);
#endif // !SDHANDELER_H