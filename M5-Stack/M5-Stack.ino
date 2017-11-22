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
*/

#include <M5Stack.h>
#include <WiFi.h>
#include <Timer.h>

const char *ssid = "ESP32ap";
const char *password = "ESP32wifi";

const IPAddress ip(192, 168, 10, 1);
const IPAddress subnet(255, 255, 255, 0);

float temp = 25.5;
boolean relay = true;

Timer t;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  WiFi.softAPConfig(ip, ip, subnet); //to configure AP IP
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

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

  server.begin();

  t.every(2000, takeReading);

  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void loop(void) {
  M5.update();
  t.update();
  updateScreen();
  updateButtons();
  checkWifiClient();

}

void takeReading () {
  temp += 0.1;
  if (temp > 35)
    temp = 25;
  Serial.println("Change temperature to: " + String(temp));
}

void updateScreen() {
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 50);
  M5.Lcd.setTextColor(RED, BLACK);
  M5.Lcd.printf("Temperature: %2.2f", temp);
  M5.Lcd.setCursor(5, 90);
  M5.Lcd.setTextColor(GREEN, BLACK);

  M5.Lcd.printf("Relay: ");
  relay ? M5.Lcd.printf("ON ") : M5.Lcd.printf("OFF");
}

void updateButtons() {
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
}

void checkWifiClient() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<body>");
            if (relay == HIGH) {
              client.println("<p>RELAY ON</p>");
              client.println("<form action=\"/OFF\" method=\"get\">");
            }
            else {
              client.println("<p>RELAY OFF</p>");
              client.println("<form action=\"/ON\" method=\"get\">");
            }

            if (relay == HIGH) client.println("<input type=\"submit\" value=\"SWITCH OFF\" />");
            else client.println("<input type=\"submit\" value=\"SWITCH ON\" />");
            client.println("</form>");
            client.println("<p>Room Temperature: " + String(temp) + "</p>");
            client.println("</body>");
            client.println("</html>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /ON")) {
          digitalWrite(5, HIGH);               // GET /H turns the LED on
          relay = true;
        }
        if (currentLine.endsWith("GET /OFF")) {
          digitalWrite(5, LOW);                // GET /L turns the LED off
          relay = false;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
