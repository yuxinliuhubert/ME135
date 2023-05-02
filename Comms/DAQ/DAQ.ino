#include <esp_now.h>
#include <WiFi.h>
#include "dataStruct.h"
#include <SoftwareSerial.h>

// Replace with the COM's MAC address
uint8_t com_mac[] = {0x94, 0xB5, 0x55, 0x6B, 0x37, 0xAC};
const size_t bufferSize = 10;
uint8_t buffer[bufferSize]; 
int state = 0;
#define RX 22
#define TX 23

daq_send_struct send_data;

EspSoftwareSerial::UART circuitSerial;

void setup() {
  Serial.begin(115200);
  circuitSerial.begin(115200,SWSERIAL_8N1,RX,TX);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, com_mac, 6);
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

if (send_data.daq_current_state != state) {
  send_data.daq_current_state = state;
    // Send data back to the sender

    circuitSerial.println(state);
  esp_err_t result = esp_now_send(com_mac, (uint8_t *)&send_data, sizeof(send_data));
}

if (circuitSerial.available() > 0) {

  int avail = circuitSerial.read(buffer,bufferSize);

     Serial.print("received confirmation: ");
     for (size_t i = 0; i < bufferSize; ++i) {
  Serial.print("buffer[");
  Serial.print(i);
  Serial.print("] = ");
  Serial.println(buffer[i], DEC); // DEC is used to print the numbers in decimal format
}


  // Serial.print("received confirmation: ");
  // Serial.println(Serial1.readStringUntil('\n'));
    // esp_err_t result = esp_now_send(com_mac, (uint8_t *)&send_data, sizeof(send_data));

}

  // if (result == ESP_OK) {
  //   Serial.println("Data sent back to sender!");
  // } else {
  //   Serial.println("Failed to send data back to sender!");
  // }

  delay(30);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  daq_receive_struct received_command;
  memcpy(&received_command, data, sizeof(received_command));


  state = received_command.commanded_state;
  Serial.print("received: ");
  Serial.println(state);


}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // if (status == ESP_NOW_SEND_SUCCESS) {
  //   Serial.println("Data was sent back successfully!");
  // } else {
  //   Serial.println("Data sent back failed!");
  // }
}
