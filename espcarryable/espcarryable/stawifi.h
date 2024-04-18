#pragma once

#ifndef MYWIFI_H
#define MYWIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include "esp_task_wdt.h"

#define CODE 1000 //code id + msg in one float, 
#define DECODE 1000 // decode msg, get id and data from one float number

//init could be wanted tmp, prog and offset.
//rawdata look likes that -3yxx.x y->min(1) or plus(0), x->numbers
class stawifi
{
  const char* SSID;
  const char* PW;
  const char* HOST;
  uint16_t port;
  WiFiClient client;
  float rawdata;
  float data;
  int ID;

	void sendData(float param);
	bool receiveData();

  void setRawdata(float);
  void setReceivedData(float);
  void setReceivedID(int);
  float getRawdata();

public:
	
	stawifi();
	bool connectToWiFi();
  bool connectToServer();
  bool IsConnected();
  float getReceivedData();
  int getReceivedID();
  bool requestInit(int);
  bool receiving();
  bool sending(float, int);
  bool IsConnectedToWifi();
};

#endif // !MYWIFI_H
