#include <ESP8266WiFi.h>
#include <Timer.h>
#include <dht.h>

#define DHT22_PIN D5

const char* ssid     = "ESP32ap";
const char* password = "ESP32wifi";

IPAddress ip;
Timer t;
dht DHT;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  ip = WiFi.localIP();
  Serial.println("IP address: ");
  Serial.println(ip);

  t.every(30000, sendTemperature);
}

void loop() {
  t.update();
}

void sendTemperature() {
  Serial.println("Reading sensor...");
  float temp;
  int chk = DHT.read22(DHT22_PIN);
  if (chk == DHTLIB_OK) {
    temp = DHT.temperature;
  }

  Serial.print("connecting to ");
  Serial.println(ip);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(ip, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/temperature";
  url += "?temp=";
  url += String(temp);

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + String(ip) + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
}

