#include <esp_now.h>
#include <WiFi.h>
#include "dataStruct.h"

// Replace with the COM's MAC address
uint8_t com_mac[] = {0x94, 0xB5, 0x55, 0x6B, 0x37, 0xAC};
int state = 0;
#define RX 22
#define TX 23

String receivedLine = "";


daq_send_struct send_data;


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200,SERIAL_8N1,RX,TX);

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
    // circuitSerial.println(state);
  esp_err_t result = esp_now_send(com_mac, (uint8_t *)&send_data, sizeof(send_data));
}

if (Serial1.available() > 0) {

  String receivedChar = Serial1.readStringUntil('\n');
  // send_data.dataString = receivedChar;
  
  sliceString(receivedChar, send_data.daq_send_data);
  esp_err_t result = esp_now_send(com_mac, (uint8_t *)&send_data, sizeof(send_data));

  
    // if (receivedChar == "\n") {
    //   Serial.print("Whole String: ");
    //   Serial.println(receivedLine);
    //   receivedLine = "";
    // } else {
    //   Serial.print(receivedChar);
    //   receivedLine += receivedChar;
    // }
    // esp_err_t result = esp_now_send(com_mac, (uint8_t *)&send_data, sizeof(send_data));

}

  // if (result == ESP_OK) {
  //   Serial.println("Data sent back to sender!");
  // } else {
  //   Serial.println("Failed to send data back to sender!");
  // }

  delay(30);
}

void sliceString(String data, int * fillArray) {
  int stringSize = data.length();
  int startingPoint = 0;
  int index = 0;
  for (int i = 0; i < stringSize; i++) {
    if (data.charAt(i) == ' ') {
      fillArray[index] = data.substring(startingPoint, i).toInt();
      Serial.print(index);
      startingPoint = (i+1);
      index = index + 1;

    }
  }
  Serial.println();


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
