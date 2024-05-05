#include "stawifi.h"
stawifi::stawifi()
{
  SSID="Esp32router";
  PW="password";
  HOST= "192.168.0.102";//102
  port=8888;
  ID=0.0;
  data=0.0;
  rawdata=0.0;
}

bool stawifi::IsConnected()
{
  return client.connected();
}

bool stawifi::IsConnectedToWifi()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    return false;
  }
  return true;
}

bool stawifi::connectToWiFi()
{
    Serial.println("Connecting to WiFi...");
  WiFi.begin(SSID, PW);
  int i=0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    if(i>2)
    {
      esp_task_wdt_reset(); // Reset the watchdog timer to prevent a board reset
      i=0;
    }
    delay(1000);
    i++;
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
  return true;
}
bool stawifi::connectToServer()
{
  Serial.println("Connecting to server...");
  if (client.connect(HOST, port)) 
  {
    Serial.println("Connected to server");
    return true;
  } 
  else 
  {
    Serial.println("Connection to server failed");
    return false;
  }
}
void stawifi::sendData(float data)
{
  // Convert float to byte array
  byte byteArray[sizeof(float)];
  memcpy(byteArray, &data, sizeof(float));
  // Send byte array
  client.write(byteArray, sizeof(byteArray));
}

bool stawifi::receiveData()
{
  bool output=false;
  if (client.available() >= sizeof(float)) {
    // Read byte array from server
    byte byteArray[sizeof(float)];
    client.readBytes(byteArray, sizeof(byteArray));
    // Convert byte array to float
    float data;
    memcpy(&data, byteArray, sizeof(float));
    setRawdata(data);
    Serial.print("Received data from server: ");
    Serial.println(data, 3); // Print with 3 decimal places
    output=true;
  }
  return output;
}
float stawifi::getReceivedData()
{
  return data;
}

int stawifi::getReceivedID()
{
  return ID;
}
float stawifi::getRawdata()
{
  return rawdata;
}

bool stawifi::requestInit(int param)
{
  int i=0;
  int n=0;
  bool output=false;
  sendData(-param*CODE);
  delay(1000);
  while(!output)
  {
    receiveData();// receive data
    i++;
    delay(1000);
    if(i>=2)// after waing 2s and nothing happens reset wdt flag
    {
      esp_task_wdt_reset();
      i=0;
      n++;
    }
    if(getRawdata()==1.0)// if get ack reseting n, to have enought time to receive the data answare
    {
      n=0;
      setRawdata(0.0);
    }
    if(getRawdata()>1000.0)//rawdata is bigger then 1000.0 float
    {
      output=true;//output will because the device got the answare what it is waiting for
      sendData(1.0);//sending ack, to inform the sender, it msg arrived in time
      setReceivedID((int)getRawdata()/DECODE);// setting the id and data, to ready to use
      setReceivedData(getRawdata()-(getReceivedID()*DECODE));
    }
    if(!output && n>=3)// every 2s n incrise, so every 2s*3=6s return false. that means the request will send out agian, until the ack is arriverd
    {
      Serial.println("init failed");
      return false;
    }
  }
  return output;
}
bool stawifi::receiving()
{
  if(receiveData() && getRawdata()!=1.0)//useable data comming
  {
    sendData(1.0);//ack
    delay(1000);
    setReceivedID((int)getRawdata()/DECODE);
    setReceivedData(getRawdata()-(getReceivedID()*DECODE));
    return true;
  }
  return false;
}
bool stawifi::sending(float data, int id)
{
  int i=0, n=0;
  if(data<0)
  {
    data=100-data;
  }
  sendData(data+(float)(CODE*id));
  delay(1000);
  while(true)
  {
    receiveData();
    i++;
    delay(1000);
    if(i>=2)
    {
      esp_task_wdt_reset();
      i=0;
      n++;
    }
    if(getRawdata()==1.0)
    {
      return true;
    }
    if(n>=3)
    {
      break;
    }
  }
  return false;;
  
}
void stawifi::setRawdata(float param)
{
  rawdata=param;
}
void stawifi::setReceivedData(float param)
{
  if(param>100.0 && param<=135)
  {
    data=(param-100)*(-1.0);
  }
  data=param;
}
void stawifi::setReceivedID(int param)
{
  ID=param;
}


