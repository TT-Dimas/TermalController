#pragma once
#include <Arduino.h>
#include "1_ClassTimer.h"
//#define USE_MICRO_WIRE
//#if defined (USE_MICRO_WIRE)
//#include <microWire.h>  // лёгкая библиотека Wire (для atmega328)


#include <Wire.h>     // обычная Wire
#include <GyverOLED.h>

#define X_SIZE 128
#define Y_SIZE 64
#define X_LITERA 5
#define Y_LITERA 7
#define SCALE_1 1
#define SCALE_2 2
#define SCALE_3 3
#define SCALE_4 4

#define N 2  //rows size of array
#define M 12 //colm size of array

//GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
static GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

static bool flag_dot = 0;

//extern uint8_t M;
extern const uint16_t arrayProfile[2][12];
extern uint16_t maxTemp;
extern uint16_t maxTime;
extern struct _time timer;


void initOLED();
void printMenu();
void printData(const uint16_t, const uint16_t, const struct _time *  timer);
