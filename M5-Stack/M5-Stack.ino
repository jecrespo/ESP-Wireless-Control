/**
    Basic Use

    System:
        M5.begin();
        M5.update();
    LCD:
        M5.lcd.setBrightness(uint8_t brightness);
        M5.Lcd.drawPixel(int16_t x, int16_t y, uint16_t color);
        M5.Lcd.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        M5.Lcd.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        M5.Lcd.fillScreen(uint16_t color);
        M5.Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
        M5.Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
        M5.Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.Lcd.drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
        M5.Lcd.drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
        M5.Lcd.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
        M5.Lcd.setCursor(uint16_t x0, uint16_t y0);
        M5.Lcd.setTextColor(uint16_t color);
        M5.Lcd.setTextColor(uint16_t color, uint16_t backgroundcolor);
        M5.Lcd.setTextSize(uint8_t size);
        M5.Lcd.setTextWrap(boolean w);
        M5.Lcd.printf();
        M5.Lcd.print();
    Button:
        M5.BtnA/B/C.read();
        M5.BtnA/B/C.isPressed();
        M5.BtnA/B/C.isReleased();
        M5.BtnA/B/C.wasPressed();
        M5.BtnA/B/C.wasReleased();
        M5.BtnA/B/C.pressedFor(uint32_t ms);
        M5.BtnA/B/C.releasedFor(uint32_t ms);
        M5.BtnA/B/C.lastChange();
    Speaker:
        M5.Speaker.tone(uint32_t freq);
        M5.Speaker.tone(freq, time);
        M5.Speaker.beep();
        M5.Speaker.setBeep(uint16_t frequency, uint16_t duration);
        M5.Speaker.mute();

   \par History:
   <pre>
   `<Author>`         `<Time>`        `<Version>`        `<Descr>`
   Zibin Zheng         2017/07/14        0.0.1          Rebuild the new.
   </pre>

*/

#include <M5Stack.h>
#include <Timer.h>

float temp = 25.5;
boolean relay = true;

Timer t;

void setup() {
  // Setup the TFT display
  M5.begin();
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillRect(0, 0, 320, 30, BLUE);
  M5.Lcd.drawCentreString(" TEMPERATURE CONTROL ", 320 / 2, 0, 2);

  M5.Lcd.drawRoundRect(0, 40, 320, 30, 5, RED);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 50);
  M5.Lcd.printf("Temperature: %2.2f", temp);

  M5.Lcd.drawRoundRect(0, 80, 320, 30, 5, GREEN);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 90);
  M5.Lcd.printf("Relay: ");
  relay ? M5.Lcd.printf("ON ") : M5.Lcd.printf("OFF");

  M5.Lcd.setTextColor(TFT_PINK);
  M5.Lcd.drawCentreString(" P.OFF ", 320 / 4, 310, 2);
  M5.Lcd.drawCentreString(" R.ON ", (320 / 4) * 2, 310, 2);
  M5.Lcd.drawCentreString(" R.OFF ", (320 / 4) * 3, 310, 2);

  M5.setWakeupButton(BUTTON_A_PIN);

  t.every(2000, takeReading);
}

void loop(void) {
  M5.update();
  t.update();

  if (M5.BtnA.wasPressed()) {
    M5.powerOFF();
  }

  if (M5.BtnB.wasPressed()) {
    relay = true;
    Serial.println("Relay ON");
    M5.Speaker.beep();
  }

  if (M5.BtnC.wasPressed()) {
    relay = false;
    Serial.println("Relay OFF");
    M5.Speaker.beep();
  }

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 50);
  M5.Lcd.setTextColor(RED, BLACK);
  M5.Lcd.printf("Temperature: %2.2f", temp);
  M5.Lcd.setCursor(5, 90);
  M5.Lcd.setTextColor(GREEN, BLACK);

  M5.Lcd.printf("Relay: ");
  relay ? M5.Lcd.printf("ON ") : M5.Lcd.printf("OFF");
}

void takeReading () {
  temp += 0.1;
  if (temp > 35)
    temp = 25;
    Serial.println("Change temperature to: " + String(temp));
}

