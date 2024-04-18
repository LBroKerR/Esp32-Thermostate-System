#pragma once
#ifndef SERVERWIFI_H
#define SERVERWIFI_H

#include <WiFi.h>
#include "esp_task_wdt.h"
#define CODE 1000 //code id + msg in one float, 
#define DECODE 1000 // decode msg, get id and data from one float number
class serverWifi
{
	char SSID[20];
	char PW[20];
	char HOST[20];
	uint16_t port=0;
	WiFiServer server;
	WiFiClient client;
	float rawdata;
	float data;
	int ID;
  int second;
  bool received;

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
  void setReceived(bool param);

  bool getReceived();
  bool CliensDisconnect(int param);
	float getReceivedData();
	int getReceivedID();
	bool receiving();
	bool sending(float, int);
  void login(String );
};

#endif // !SERVERWIFI_H
