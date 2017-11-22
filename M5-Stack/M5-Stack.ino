#include <M5Stack.h>

float temp = 25.5;
boolean relay = true;

void setup() {
  // Setup the TFT display
  M5.begin();
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.fillRect(0, 0, 320, 40, BLUE);
  M5.Lcd.drawCentreString(" TEMPERATURE CONTROL ", 320 / 2, 0, 2);

  M5.Lcd.drawRect(0, 50, 320, 90, RED);
  M5.Lcd.setCursor(60, 0);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("Temperature: %2.2f", temp);

  M5.Lcd.drawRect(0, 100, 320, 140, GREEN);
  M5.Lcd.setCursor(60, 0);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("Relay: ");
  relay ? M5.Lcd.printf("ON") : M5.Lcd.printf("OFF");

  M5.setWakeupButton(BUTTON_A_PIN);
}


void loop(void) {
  if (M5.BtnA.wasPressed()) {
    M5.powerOFF();
  }

  M5.update();
}
