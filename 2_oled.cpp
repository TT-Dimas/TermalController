#include "2_oled.h"


#define DEG 0 // for degree array
#define MIN 1 // for minute array
#define X_GRAF 64
#define Y_GRAF 42

void pointer(int x0, int y0, int x1, int y1, int X);
void graficDraw(void);

void initOLED() {
  oled.init();
  Wire.setClock(800000L);
  oled.invertDisplay(1);
  oled.setContrast(15); //1-15

}

void printData(const uint16_t actualTemp, const uint16_t desireTemp, const struct _time * timer) {
  uint16_t _actualTemp = actualTemp;
  uint16_t _desireTemp = desireTemp;
  uint8_t n = 1;

  oled.clear();
  oled.home();
  oled.setCursorXY(X_SIZE  - X_LITERA, 0 );
  oled.setScale(1); //1-4
  oled.print("o");


  while ((_actualTemp /= 10) > 0) n++; // quantity of digit in _actualTemp
  oled.setScale(SCALE_3); //1-4
  oled.setCursorXY(X_SIZE - X_LITERA - (X_LITERA * SCALE_3 + SCALE_3) * n , Y_LITERA);
  oled.print(actualTemp);

  n = 1;
  _desireTemp = desireTemp;
  while ((_desireTemp /= 10) > 0) n++; // quantity of digit in _desireTemp

  oled.setScale(SCALE_2); //1-4
  oled.setCursorXY(X_SIZE - X_LITERA - (X_LITERA * SCALE_2 + SCALE_2) * n , Y_LITERA * 5 );
  oled.print(desireTemp);

  ///// timer print Here://///////////
  oled.setScale(SCALE_1); //1-4
  oled.setCursorXY(3, 7 );
  oled.print(timer->hourLeft);
  oled.print(":");
  if (timer->minuteLeft < 10)
    oled.print("0");
  oled.print(timer->minuteLeft);
  oled.print(":");
  if (timer->secondLeft < 10)
    oled.print("0"); //
  oled.print(timer->secondLeft);
  //////END  timer print Here////////////


  //////Draw Grafic Here////////////
  uint16_t xTime;
  xTime = (uint16_t)(timer->processClock / 60);

  uint16_t X0, X1, Y0, Y1;
  for (uint8_t i = 0; i < 12; i ++) {
    uint8_t X = xTime * X_GRAF / maxTime;
    X0 = arrayProfile[MIN][i] * X_GRAF / maxTime;
    X1 = arrayProfile[MIN][i + 1] * X_GRAF / maxTime;
    Y0 = arrayProfile[DEG][i] * Y_GRAF / maxTemp;
    Y1 = arrayProfile[DEG][i + 1] * Y_GRAF / maxTemp;
    oled.line(X0, Y_SIZE - Y0 , X1, Y_SIZE - Y1,  1);
    if (X >= X0 && X < X1) pointer(X0, Y0, X1, Y1, X);
  }
  /*Serial.println(X0);
    Serial.println(Y0);
    Serial.println(X1);
    Serial.println(Y1);*/
  //////END Draw Grafic Here/////////////
  oled.update();

}


///// dot pointer drawing/////////////////
void pointer(int x0, int y0, int x1, int y1, int X) {
  //  _x = 0;
  //  _y = 0;
  int sx, sy, e2, err;
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  sx = (x0 < x1) ? 1 : -1;
  sy = (y0 < y1) ? 1 : -1;
  err = dx - dy;
  for (;;) {
    if (x0 == X) {
      oled.circle(x0, Y_SIZE - y0, 3, 1);   // dot(x0, y0, fill);
      return;
    }
    if (x0 == x1 && y0 == y1) return;
    e2 = err << 1;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
}

/////END dot pointer drawing/////////////////
