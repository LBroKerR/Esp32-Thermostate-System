/*
 Name:		esptest.ino
 Created:	2/24/2024 2:39:00 PM
 Author:	rabai
*/


#include <Arduino.h>
#define INFOSIZE 2
#define ID 0
#define DATA 1
//typedef struct QueueList{ QueueHandle_t *core0;QueueHandle_t *core1;} Msg;
typedef struct QueueList{ QueueHandle_t core0;QueueHandle_t core1;} Msg;
Msg msg;
bool receive(float* data /*,QueueHandle_t queue*/)
{
  int ret=xQueueReceive(msg.core0, data, (TickType_t)1000);
    if ( ret == pdPASS) 
    {
     //Serial.print("Received data on Core 0: ");
      //Serial.println(*data);
      return true;
    }
    else if( ret == pdFALSE)
    {
      //Serial.println("False");
    }
    return false;
}

bool send(float *data /*,QueueHandle_t queue*/)
{
int ret=xQueueSend(msg.core0, data, 0);
    if(ret == pdTRUE)
    {
        return true;
    }
    else if(ret==errQUEUE_FULL)
    {
     // Serial.println("error");
    }
    return false;
}

void taskCore0(void *parameter)
{
  //Msg *msg=(Msg*)parameter;
  float receivedData[INFOSIZE];
  bool n=false;
  int ret;
  float ack[INFOSIZE]={1.0,0.0};
  while (true) 
  {
      n=receive(receivedData);
      Serial.print("msg from core1: ");
      Serial.print(receivedData[ID]);
      Serial.println(receivedData[DATA]);
      if(n)
      {
        ret=xQueueSend(msg.core1, &ack, 0);
        n=false;
      }
    receivedData[ID]=0;
    receivedData[DATA]=0;
  }
  vTaskDelete(NULL); // Delete the task after completion
}

void taskCore1(void *parameter)
{
  //Msg *msg=(Msg*)parameter;
  float dataToSend[INFOSIZE] = {2.0, 12.36};
  bool n=false;
  int ret;
  float ack[INFOSIZE]={1.0,0.0};
  while(true)
  {
    if(!n)
    {
      n=send(dataToSend);
    }
    else
    {
      ret=xQueueReceive(msg.core1, ack, (TickType_t)1000);
      if(ret == pdPASS && ack[ID]==1.0)
      {
        Serial.println("got ack");
        n=false;
      }
    }
      delay(1000);
  }
vTaskDelete(NULL); // Delete the task after completion
}

void setup() 
{
  Serial.begin(115200);
  // Create msg for inter-core communication
  //Msg msg;
  //QueueHandle_t queue1=xQueueCreate(1, sizeof(float));
  //QueueHandle_t queue2=xQueueCreate(1, sizeof(float));
  //msg->core0=&queue1;
  //msg->core1=&queue2;
  msg.core0=xQueueCreate(DATA, sizeof(float[INFOSIZE]));
  msg.core1=xQueueCreate(DATA, sizeof(float[INFOSIZE]));
  // Create and start tasks for both cores
  xTaskCreatePinnedToCore(taskCore0, "taskCore0", 10000, NULL/*&msg*/, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskCore1, "taskCore1", 10000, NULL/*&msg*/, 1, NULL, 1);
}
void loop(){delay(1000);}
