
#include <WiFi.h>
#include "esp_task_wdt.h"

const char *SSID="Esp32router";
const char *PW="Kakukk32";
const char *HOST= "192.168.0.102";//102
uint16_t port=8888;
float number=0.0;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  connectToWiFi();
}

void loop() {
  if (!client.connected()) {
    connectToServer();
  }

  if (client.connected()) {
    sendData(1000);
    delay(1000); // Send data every second
    if(receiveData())
    {
      //client.stop();
      sendData(10);
      delay(1000);
      if(receiveData())
      {
        //client.stop();
      }
    }
  }
  else
  {
    Serial.println("what?");
  }
}

void connectToWiFi() 
{
  Serial.println("Connecting to WiFi...");
  WiFi.begin(SSID, PW);
   int i=0;

  while (WiFi.status() != WL_CONNECTED) 
  {
    if(i>=2)
    {
      esp_task_wdt_reset(); // Reset the watchdog timer to prevent a reset
      i=0;
    }
    delay(1000);
    i++;
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
}

void connectToServer() {
  Serial.println("Connecting to server...");
  if (client.connect(HOST, port)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection to server failed");
  }
}

void sendData(float data) {
  // Convert float to byte array
  byte byteArray[sizeof(float)];
  memcpy(byteArray, &data, sizeof(float));
  // Send byte array
  client.write(byteArray, sizeof(byteArray));
}
bool receiveData() 
{
  bool output=false;
  if (client.available() >= sizeof(float)) {
    // Read byte array from server
    byte byteArray[sizeof(float)];
    client.readBytes(byteArray, sizeof(byteArray));
    // Convert byte array to float
    float data;
    memcpy(&data, byteArray, sizeof(float));
    Serial.print("Received data from server: ");
    Serial.println(data, 3); // Print with 3 decimal places
    output=true;
  }
  return output;
}
