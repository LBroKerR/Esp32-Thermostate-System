/*
 Name:		espcarryable.ino
 Created:	2/26/2024 9:18:23 AM
 Author:	rabai
*/

// the setup function runs once when you press reset or power the board
#include "defines.h"
#define WIFI 1

void setup() 
{
	Serial.begin(115200);
  Data *data=new Data;
  TaskHandle_t Core1Task;// Task handle for Core 1
#if WIFI
  TaskHandle_t Core0Task; // Task handle for Core 0
  xTaskCreatePinnedToCore(Core0TaskFunction, "Core0Task", 10000, data, 1, &Core0Task, 0);
#endif
  xTaskCreatePinnedToCore(Core1TaskFunction, "Core1Task", 10000, data, 1, &Core1Task, 1);
}
#if WIFI
void ReceiveChangedData(Data*data, stawifi* wifi)
{
  if(wifi->receiving())
  {
    if(wifi->getReceivedID()!=0)
    {
      //set data
      data->setData(wifi->getReceivedData(), wifi->getReceivedID());
    }
  }
}
void SendChangedData(Data*data, stawifi* wifi)
{
      if(data->getChanged_tmp())
      {
        wifi->sending(data->getData(TMP),TMP);
        data->setChanged_tmp(false);
      }
      if(data->getChanged_prog())  
      {
        wifi->sending(data->getData(PROG),PROG);
        data->setChanged_prog(false);
      }
      if(data->getChanged_wantedTmp())
      {
        wifi->sending(data->getData(WANTEDTMP), WANTEDTMP);
        data->setChanged_wantedTmp(false);
      }
}
bool Init(stawifi wifi, int param, Data*data)
{
    while(!wifi.requestInit(param))
    {
      if(!wifi.IsConnected())
      {
        while(!wifi.connectToServer());
      }
    }
    Serial.println(wifi.getReceivedID());
    return data->setData(wifi.getReceivedData(),wifi.getReceivedID());
}
void Core0TaskFunction(void *parameter) 
{
  Data*data=(Data*)parameter;
  stawifi wifi;
  wifi.connectToWiFi();
  while(!wifi.connectToServer());
  //request init data!!
  while(!Init(wifi, WANTEDTMP, data));
  while(!Init(wifi, OFFSET, data));
  while(!Init(wifi, MAXPRG, data));
  while(!Init(wifi, PROG, data));
  while (true) 
  {
    // Core 0 task code here
    if(!wifi.IsConnectedToWifi())
    {
      wifi.connectToWiFi();
    }
    if(!wifi.IsConnected())
    {
      while(!wifi.connectToServer());
    }
    ReceiveChangedData(data, &wifi);
    SendChangedData(data, &wifi);
    delay(5);
  }
}
#endif
void Core1TaskFunction(void *parameter) 
{
  Data*data=(Data*)parameter;
  DHT sensor(DHTPIN,DHTTYPE);
  Buttons buttons(PLUSPIN,MINUSPIN,PROGPIN);
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  sensor.begin();

  while(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS));
    //Serial.println(F("SSD1306 allocation failed"));
    // Don't proceed, infinte loop
  while(true)
  {
    mesureSensor(data, &sensor);
    data->ChangeProg(buttons.progchange());
    data->ChangeWantedTmp(buttons.tempchange(STEP));
    printThermos(data, &display);
    delay(5);
  }
}
// the loop function runs over and over again until power down or reset
void loop()
{
}

void printThermos(Data *data, Adafruit_SSD1306 *display )
{
    display->setTextColor(SSD1306_WHITE);
    display->clearDisplay();
    display->setTextSize(SIZE1);

    display->setCursor(WANTEDTXT_X, WANTEDTMP_Y);
    display->print("wanted tmp:");

    display->setCursor(WANTEDTMP_X,WANTEDTMP_Y);
    display->print(data->getWantedTmp(),1);// text + if its double or  float, it will printed out like this xy.z

    display->setCursor(WANTEDC_X, WANTEDTMP_Y);
    display->print("C");

    display->setCursor(HMDTXT_X, HMDPROG_Y);
    display->print("hmd:");

    display->setCursor(HMD_X, HMDPROG_Y);
    display->print(data->getHMD(),1);

    display->setCursor(HMDPERCEN, HMDPERCEN);
    display->print("%");

    display->setCursor(PROGTXT_X, HMDPROG_Y);
    display->print("prog:");

    display->setCursor(PROG_X, HMDPROG_Y);
    display->print(data->getProg(),1);

    display->setCursor(TMPC_X, TMPC_Y);// x,y
    display->print("C");

    display->setTextSize(SIZE2);
    display->setCursor(TMP_X, TMP_Y);// x,y
    display->print(data->getTemp(),1);

    display->setCursor(SWITCH_X, SWITCH_Y);

    if(data->getHeatSwitch())
    {
      display->print("ON");
    }
    else
    {
      display->print("OFF");
    }

    display->display();
}
void mesureSensor(Data *data, DHT *sensor)
{
  data->setSensor(sensor->readTemperature(), sensor->readHumidity());
}
