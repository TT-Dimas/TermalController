#include <Arduino.h>
#include "1_ClassTimer.h"
#include "6_TermalProfile.h"
#include "2_oled.h"
#include <GyverButton.h>
#include <GyverMAX6675.h>

///////////  MAX6675K here////////////////////
#define CLK_PIN   8  // Пин SCK
#define DATA_PIN  10 // Пин SO
#define CS_PIN    9  // Пин CS
#define POWER_PIN 7 // VCC +5V PIN for MAX6775
#define GND_PIN 6 //GND PIN for MAX6775
GyverMAX6675<CLK_PIN, DATA_PIN, CS_PIN> sensor;
///////////END MAX6675K here//////////////////

////////button constant here///////
#define UP 5
#define OK 4
#define DOWN 3
GButton btnUP(UP);
GButton btnOK(OK);
GButton btnDOWN(DOWN);
////////END button constant here///////

uint16_t            sensorTemperature;
uint16_t            desireTemperature;

Timer_16            timerSensor(1000);
Timer_16            timerProfileRead(1000);
Timer_16            timerOLED(500);
Timer_16            timerCountDown(200);

struct _time timer;
struct _time  * ptrTimer = &timer;

void timeUpdate(struct _time *, int16_t steep);
void turnBtn(void);
void startMenu(void);
void workCycle(void);

extern uint16_t maxTime;
extern uint16_t maxTemp;
uint16_t maxTimeSec = maxTime * 60;
uint8_t error = 0;
uint8_t menu = 0;

void setup() {
  extern struct _time const * ptrTimer;
  Serial.begin(9600);
  pinMode(POWER_PIN, OUTPUT);
  pinMode(GND_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  digitalWrite(GND_PIN, LOW);
  initOLED();
  //  Serial.println("Print MaxTime again:");
  //  Serial.println(maxTime * 60);
  btnUP.setTickMode(AUTO);// обязательная функция отработки. Должна постоянно опрашиваться
  btnOK.setTickMode(AUTO);// обязательная функция отработки. Должна постоянно опрашиваться
  btnDOWN.setTickMode(AUTO);// обязательная функция отработки. Должна постоянно опрашиваться
  timer.processClock = 0; // in second
  timer.countdownClock = maxTimeSec;

}
////////////MAIN LOOP here/////////////////
void loop() {
  startMenu();
  if (menu == 0)
  {
    initProfile();
    while (!btnOK.isHold())
      workCycle();
  }

  else if (menu == 1)
  { /*TO DO Drying menu here */
  }
}

////////////END MAIN LOOP here//////////////


/////////////Mesurment, heating, math here/////////
void workCycle(void) {
  if (timerSensor.ready() ) {
    uint8_t attemp = 4;
    while (!sensor.readTemp() && attemp --) // Читаем температуру 5 попыток
      error ++;
    sensorTemperature = (uint16_t)sensor.getTempInt();
  }
  if (timerCountDown.ready())    timeUpdate(ptrTimer, 1);
  if (timerProfileRead.ready())  desireTemperature = getTermoPtofile((uint16_t)timer.processClock);
  if (timerOLED.ready())         printData(sensorTemperature, desireTemperature, ptrTimer);
  turnBtn();
}
/////////////END Mesurment, heating, math here/////////


/////////Timer///////////////////
void timeUpdate(struct _time *timer, int16_t steep) {
  if (timer->processClock + steep > timer->totalTime)  timer->processClock = timer->totalTime;
  else if (timer->processClock + steep < 0)            timer->processClock = 0;
  else                                                 timer->processClock += steep;
  timer->countdownClock = timer->totalTime  - timer->processClock ;

  timer->hourLeft   = (int8_t)(  timer->countdownClock / 3600);
  timer->minuteLeft = (int8_t)(( timer->countdownClock / 60) % 60);
  timer->secondLeft = (int8_t)(  timer->countdownClock % 60);
  Serial.println(timer->processClock);
  Serial.println(timer->countdownClock);
}
///////////END Timer////////////

//////////Encoder here//////////////

void turnBtn(void) {
  static Timer_16 scrollDelayUP(100);
  static Timer_16 scrollDelayDOWN(100);
  
  if (btnUP.isClick()) {
    timeUpdate(ptrTimer, 59);
    return;
  }
  if (btnDOWN.isClick()) {
    timeUpdate(ptrTimer, - 59);
    return;
  }
  if (btnUP.isHold() && scrollDelayUP.ready()) {
    timeUpdate(ptrTimer, 7 * 59);
    return;
  }
  if (btnDOWN.isHold() && scrollDelayDOWN.ready()) {
    timeUpdate(ptrTimer, -7 * 59);
    return;
  }
}
//////////END Encoder here////////////////////

////////////termal read here////////////////////
//uint16_t termalRead (void) {
//  uint8_t attemp = 0;
//  while (!sensor.readTemp() ) { // Читаем температуру 5 попыток
//    error ++;
//    }
//  return (uint16_t)sensor.getTempInt();
//  }
//  Serial.println("Sensor Error"); //else  ошибка чтения или подключения - выводим лог
//  error ++;
//  return 0xFFFFFFFF;
//}
//////////END termal read here/////////////////

void startMenu() {
  uint8_t temp_menu = 0;
  oled.clear();
  oled.home();
  oled.setCursorXY(X_SIZE / 2 - 5 * X_LITERA, 0 );
  oled.setScale(2); //1-4
  oled.print("Режим:");
  oled.setCursorXY(17, Y_LITERA * SCALE_2 * 2 - 2);
  oled.print("Випал");
  oled.setCursorXY(17, Y_LITERA * SCALE_2 * 3 + 3 );
  oled.print("Сушка");
  oled.setCursorXY(1, Y_LITERA * SCALE_2 * 2 );
  oled.print(">");
  while (1) {

    //while (btnOK.isHolded()) {}
    if (btnUP.isClick() || btnDOWN.isClick() ) {
      temp_menu = !temp_menu;// Сушка чи Випал
      oled.clear(0, 0, 15, 64);
      oled.setCursorXY(1, Y_LITERA * SCALE_2 * 2 + (SCALE_2 * Y_LITERA * temp_menu + 3 * temp_menu));
      oled.print(">");
      oled.update(0, 0, 15, 64);
    }
    if (btnOK.isClick()) {
      menu = temp_menu;
      return;
    }
  }
}

void fastWrite(const uint8_t pin, bool val) {
  if (pin < 8) bitWrite(PORTD, pin, val);
  else if (pin < 14) bitWrite(PORTB, (pin - 8), val);
  else if (pin < 20) bitWrite(PORTC, (pin - 14), val);
}
