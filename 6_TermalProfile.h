#pragma once
#include <Arduino.h>
#include "1_ClassTimer.h"

#define END_OF_PROFILE -1000 // return -1000 in case of reach end of time array.
#define N 2  //rows size of array
#define M 12 //colm size of array
#define DEG 0 // for degree array
#define MIN 1 // for minute array

//extern uint8_t M;
//extern uint8_t N;
extern const uint16_t arrayProfile[N][M];

extern uint16_t maxTemp;
extern uint16_t maxTime;
extern struct _time  * ptrTimer;


void initProfile();
uint16_t getTermoPtofile(uint16_t actualTime);
static uint16_t interpolate(uint16_t actualTime);
