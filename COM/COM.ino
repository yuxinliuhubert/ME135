#include <esp_now.h>
#include <WiFi.h>
#include "dataStruct.h"

// Replace with the receiver's MAC address
uint8_t receiver_mac[] = {0x54, 0x43, 0xB2, 0xDC, 0x8C, 0xA0};

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
  memcpy(peerInfo.peer_addr, receiver_mac, 6);
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
  data.temperature = 25.5;
  data.humidity = 60;
  data.isRaining = false;



  esp_err_t result = esp_now_send(receiver_mac, (uint8_t *)&data, sizeof(data));

  // if (result == ESP_OK) {
  //   Serial.println("Data sent!");
  // } else {
  //   Serial.println("Failed to send data!");
  // }

  delay(30);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // if (status == ESP_NOW_SEND_SUCCESS) {
  //   Serial.println("Data was sent successfully!");
  // } else {
  //   Serial.println("Data sent failed!");
  // }
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  dataStruct receivedData;
  memcpy(&receivedData, data, sizeof(receivedData));

  // // Serial.print("Temperature: ");
  Serial.println(receivedData.temperature);
  // Serial.print("Humidity: ");
  // Serial.println(receivedData.humidity);
  // Serial.print("Is raining: ");
  // Serial.println(receivedData.isRaining ? "Yes" : "No");
}
