#pragma once
#include <Arduino.h>

struct _time {
  int8_t hourLeft;
  int8_t minuteLeft;
  int8_t secondLeft;
  int32_t totalTime;
  int32_t processClock; //in second
  int32_t countdownClock; //in second

};


//Реализация 1-х байтного таймера на максимальный период 255 мс:
class Timer_8 {
  public:
    Timer_8(uint8_t startPeriod = 0) {
      setPeriod(startPeriod);
    }
    void setPeriod(uint8_t startPeriod) {
      period = startPeriod;
    }
    bool ready() {
      return (period && (millis() & 0xFF) - timer >= period) ? (timer = (millis() & 0xFF), 1) : 0; //return state 1 or 0
    }
  private:
    uint8_t timer = 0, period = 0;

};
//Реализация 2-х байтного таймера на максимальный период 65'535 мс (чуть больше минуты):
class Timer_16 {
  public:
    Timer_16(uint16_t startPeriod = 0) {
      setPeriod(startPeriod);
    }
    void setPeriod(uint16_t startPeriod) {
      period = startPeriod;
    }
    bool ready() {
      return (period && (millis() & 0xFFFF) - timer >= period) ? (timer = (millis() & 0xFFFF), 1) : 0; //return state 1 or 0
    }
  private:
    uint16_t timer = 0, period = 0;

};

//Реализация 4-х байтного таймера на максимальный период 4'294'967'295 мс (примерно 49,71 суток):
class Timer_32 {
  public:
    Timer_32(uint32_t startPeriod = 0) {
      setPeriod(startPeriod);
    }
    void setPeriod(uint32_t startPeriod) {
      period = startPeriod;
    }

    bool ready() {
      return (period && millis() - timer >= period) ? (timer = millis(), 1) : 0; //return state 1 or 0
    }
  private:
    uint32_t timer = 0, period = 0;

};
