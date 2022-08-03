#include "6_TermalProfile.h"


//uint16_t arrayProfile1[N][M] = {
//  {40, 400, 400, 600, 600, 1030, 1030, 600, 500,  40}, /// Temperature stages here [:degree] DEG
//  { 0, 230, 300, 340, 390,  590,  650, 690, 720, 760}  /// Time when DEG should be reach here [:minute] MIN
//};

// update array here: //////////////
const uint16_t arrayProfile[2][12] = {
  {40, 200, 400, 400, 580, 580, 800, 1020, 1020, 600, 230,  40}, // Temperature stages here [:degree] DEG
  { 0, 120, 240, 260, 400, 430, 520,  680,  690, 720, 760, 790}  // Time when DEG should be reach here [:minute] MIN
};



//uint8_t M = sizeof(arrayProfile) / sizeof(uint16_t) / 2;
//uint8_t N = 2;
//uint16_t *ptrProfile = arrayProfile;

uint16_t maxTemp;//arrayProfile[DEG][M-1];
uint16_t maxTime;
void initProfile() {
  maxTime = arrayProfile[MIN][M - 1];
  maxTemp = 0;
  for (int i = 0; i < M; i ++) {
    if (maxTemp <  arrayProfile[DEG][i])
      maxTemp = arrayProfile[DEG][i];
  }
 // Serial.println(maxTime);
 // Serial.println(maxTemp);
  ptrTimer->totalTime = maxTime * 60 ;
  // Serial.println (sizeC);

}

uint16_t getTermoPtofile(uint16_t actualTime) {
  return interpolate(actualTime / 60); //convert to minutes
}


uint16_t interpolate(uint16_t actualTime) {
  // check if we reach end of  profile
  if (actualTime > arrayProfile[MIN][M - 1] || actualTime < arrayProfile[MIN][0] ) return END_OF_PROFILE;

  byte i = 0;
  //lookup i-value of array location
  if (actualTime == arrayProfile[MIN][0]) i = 0;
  else if (actualTime == arrayProfile[MIN][M - 1]) i = M - 1;
  else while ( !(actualTime >= arrayProfile[MIN][i] && actualTime < arrayProfile[MIN][i + 1])) i++;

  if (i > M) {
//    Serial.println("Wrong [i] Here:");
//    Serial.println(i);
    return 9999;
  }
//  Serial.println("Here is i-array counter");
//  Serial.println(i);
//  Serial.println("ActualTime Here:");
//  Serial.println(actualTime);

  // if we short into Node, we already know it value, so just return it
  if (actualTime == arrayProfile[MIN][i])                return arrayProfile[DEG][i];

  // if Node [DEG] [i] == Node[DEG][i +1] than temperature is constant, so just return it
  if (arrayProfile[DEG][i] == arrayProfile[DEG][i + 1] ) return arrayProfile[DEG][i];

  // all math happens there:
  int16_t a = arrayProfile[MIN][i + 1] - arrayProfile[MIN][i];
  int16_t b = arrayProfile[DEG][i + 1] - arrayProfile[DEG][i];
  uint16_t deltaMinute = actualTime - arrayProfile[MIN][i];
  uint16_t  deltaDegree = (float)b / (float)a * deltaMinute ;

  return arrayProfile[DEG][i] + (uint16_t) deltaDegree;
  ;
}
