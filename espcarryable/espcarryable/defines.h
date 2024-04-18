#pragma once
#ifndef DEFINES_H
#define DEFINES_H

// includes all used library
#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


#include "Button.h"
#include "stawifi.h"
#include "Data.h"


//display desciption, data to set up
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define INFOSIZE 2
#define ID 0
#define DATA 1

// tmp sensor type, and pin
#define DHTTYPE DHT21
#define DHTPIN 27

//buttons data pins 
#define PLUSPIN 32
#define MINUSPIN 33
#define PROGPIN 25
//wanted tmp setting 
#define STEP 0.1



//display starter location in bits
#define WANTEDTXT_X 2
#define WANTEDTMP_Y 2
#define WANTEDTMP_X 70
#define WANTEDC_X 100

#define HMDPROG_Y 55
#define HMDTXT_X 2
#define HMD_X 25
#define HMDPERCEN 55
#define PROGTXT_X 80
#define PROG_X 110

#define TMPC_Y 25
#define TMPC_X 55
#define TMP_X 4
#define TMP_Y 25

#define SWITCH_X 80
#define SWITCH_Y 25

#define SIZE1 1
#define SIZE2 2



#endif // !DEFINES_H
