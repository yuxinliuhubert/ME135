#include <esp_now.h>
#include <WiFi.h>
#include "dataStruct.h"

// Replace with the sender's MAC address
uint8_t sender_mac[] = {0x94, 0xB5, 0x55, 0x6B, 0x37, 0xAC};

dataStruct data;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, sender_mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer!");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  delay(30);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  dataStruct receivedData;
  memcpy(&receivedData, data, sizeof(receivedData));

  Serial.print("Temperature: ");
  Serial.println(receivedData.temperature);
  Serial.print("Humidity: ");
  Serial.println(receivedData.humidity);
  Serial.print("Is raining: ");
  Serial.println(receivedData.isRaining ? "Yes" : "No");

  // Send data back to the sender
  esp_err_t result = esp_now_send(sender_mac, (uint8_t *)&receivedData, sizeof(receivedData));

  if (result == ESP_OK) {
    Serial.println("Data sent back to sender!");
  } else {
    Serial.println("Failed to send data back to sender!");
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Data was sent back successfully!");
  } else {
    Serial.println("Data sent back failed!");
  }
}
