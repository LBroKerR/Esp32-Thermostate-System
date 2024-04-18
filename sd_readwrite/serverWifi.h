#pragma once
#ifndef SERVERWIFI_H
#define SERVERWIFI_H

#include <WiFi.h>
#include "esp_task_wdt.h"
#define CODE 1000 //code id + msg in one float, 
#define DECODE 1000 // decode msg, get id and data from one float number
class serverWifi
{
	char *SSID="Esp32router";
	char *PW="Kakukk32";
	char *HOST="192.168.0.102";
	uint16_t port=8888;
	WiFiServer server;
	WiFiClient client;
	float rawdata;
	float data;
	int ID;

	bool receiveData();
	void sendData(float data);
	void setRawdata(float);
	void setReceivedData(float);
	void setReceivedID(int);
	float getRawdata();

  void setSSID(String);
  void setPW(String);
  void setHOST(String);
  void setport(String);

public:
	serverWifi();
	void systembegin();

  bool getClient();
  bool getIsConneted();

	float getReceivedData();
	int getReceivedID();
	bool receiving();
	bool sending(float, int);
  void login(String );
};

#endif // !SERVERWIFI_H
