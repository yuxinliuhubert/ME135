#include <WiFi.h>
#include "wifi_config.h"

const char* ssid = SSID;
const char* password = PASSWORD;
const char* serverIP = IP;
const uint16_t serverPort = 80;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  if (!client.connect(serverIP, serverPort)) {
    Serial.println("Connection to server failed");
    return;
  }

  Serial.println("Connected to server");
}

void loop() {
  if (client.connected()) {
    if (Serial.available()) {
      char c = Serial.read();
      client.write(c);
    }

    if (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  } else {
    Serial.println("Disconnected from server");
    delay(5000);

    if (!client.connect(serverIP, serverPort)) {
      Serial.println("Reconnection failed");
    } else {
      Serial.println("Reconnected to server");
    }
  }
}
