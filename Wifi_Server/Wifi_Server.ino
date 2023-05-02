#include <WiFi.h>
#include "wifi_config.h"

const char* ssid = SSID;
const char* password = PASSWORD;
#define LED 13
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(LED,OUTPUT);
  digitalWrite(LED, HIGH);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  digitalWrite(LED, LOW);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        client.write(c);
        digitalWrite(LED, LOW);
      } else {
        digitalWrite(LED, HIGH);
      }


    }
    client.stop();
    Serial.println("Client disconnected");
  }

}

