#include <WiFi.h>
#include "esp_task_wdt.h"
#define CODE 1000 //code id + msg in one float, 
#define DECODE 1000 // decode msg, get id and data from one float number
const char *SSID="Esp32router";
const char *PW="password";
const char *HOST= "192.168.0.102";//102
uint16_t port=8888;
WiFiServer server(port);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  systembegin();
}

void loop() {
  if (!client.connected()) 
  {
    client = server.available();
    Serial.println("ASD");
  }
    if (client) 
    {
      //Serial.println("Client connected");
      if(receiveData())
      {
        sendData(1.0);
        delay(1000);
        sendData(2022.2);
        delay(1000);
        if(receiveData())
        {
          sendData(1.0);
        }
      }
    }
}

void systembegin() {
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
  server.begin();
}

bool receiveData() 
{
  bool output=false;
  if (client.connected() && client.available() >= sizeof(float)) {
    // Read byte array from client
    byte byteArray[sizeof(float)];
    client.readBytes(byteArray, sizeof(byteArray));
    // Convert byte array to float
    float receivedData;
    memcpy(&receivedData, byteArray, sizeof(float));
    Serial.print("Received data from client: ");
    Serial.println(receivedData, 3); // Print with 3 decimal places
    output=true;
  }
  return output;
}
void sendData(float data) {
  // Convert float to byte array
  byte byteArray[sizeof(float)];
  memcpy(byteArray, &data, sizeof(float));
  // Send byte array
  client.write(byteArray, sizeof(byteArray));
  
}
