#include "serverWifi.h"

bool serverWifi::receiveData()
{
    bool output = false;
    if (client.connected() && client.available() >= sizeof(float)) 
    {
        // Read byte array from client
        byte byteArray[sizeof(float)];
        client.readBytes(byteArray, sizeof(byteArray));
        // Convert byte array to float
        float receivedData;
        memcpy(&receivedData, byteArray, sizeof(float));
        setRawdata(receivedData);
        Serial.print("Received data from client: ");
        Serial.println(receivedData, 3); // Print with 3 decimal places
        output = true;
    }
    return output;
}

void serverWifi::sendData(float data)
{
    // Convert float to byte array
    byte byteArray[sizeof(float)];
    memcpy(byteArray, &data, sizeof(float));
    // Send byte array
    client.write(byteArray, sizeof(byteArray));
}

void serverWifi::setRawdata(float param)
{
    rawdata = param;
}

void serverWifi::setReceivedData(float param)
{
    if (param > 100.0)
    {
        data = (param - 100) * (-1.0);
    }
    data = param;
}

void serverWifi::setReceivedID(int param)
{
    ID = param;
}

float serverWifi::getRawdata()
{
    return rawdata;
}

serverWifi::serverWifi()
{

}
bool serverWifi::getClient()
{
  if(client.connected())
  {
    return true;
  }
  return false;
}
bool serverWifi::getIsConneted()
{
  if(WiFi.status() == WL_CONNECTED)
  {
    return true;
  }
  return false;
}
void serverWifi::systembegin()
{
    Serial.println("Connecting to WiFi...");
    WiFi.begin(SSID, PW);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (i >= 2)
        {
            esp_task_wdt_reset(); // Reset the watchdog timer to prevent a reset
            i = 0;
        }
        delay(1000);
        i++;
        Serial.println("Connecting...");
             Serial.println(SSID);
                  Serial.println(PW);
                  Serial.println(HOST);
                  Serial.println(port);
    }
    Serial.println("Connected to WiFi");
    //server.begin();
}

float serverWifi::getReceivedData()
{
    return data;
}

int serverWifi::getReceivedID()
{
    return ID;
}

bool serverWifi::receiving()
{
    bool output = false;
    if (receiveData() && getRawdata() != 1.0)//useable data comming
    {
        sendData(1.0);//ack
        delay(1000);
        setReceivedID((int)getRawdata() / DECODE);
        setReceivedData(getRawdata() - getReceivedID() * DECODE);
        output = true;
    }
    return output;
}
bool serverWifi::sending(float data, int id)
{
    bool output = false;
    float info = data + (float)(CODE * id);
    int i = 0, n = 0;
    sendData(info);
    delay(1000);
    while (!output)
    {
        receiveData();
        i++;
        delay(1000);
        if (i >= 2)
        {
            esp_task_wdt_reset();
            i = 0;
            n++;
        }
        if (getRawdata() == 1.0)
        {
            output = true;
        }
        else
        {
            output = true;
            setReceivedID(0.0);
            delay(1000);
        }
        if (!output && n >= 3)
        {
            return false;
        }
    }
    return output;
}

void serverWifi::login(String param)
{
  unsigned size=param.length()+1;
  char data[size];
  param.toCharArray(data, size);
  unsigned array[4]={0,0,0,0};
  for(unsigned i=0, n=0;i<size && n<4; i++)
  {
    if(data[i]==' ' || data[i]=='\n' || data[i]=='\t')
    {
      array[n]=i-1;
      n++;
    }
  }
 setSSID(param.substring(0,array[0]+1));
 setPW(param.substring(array[0]+2, array[1]+1));
 setHOST(param.substring(array[1]+2,array[2]+1));
 setport(param.substring(array[2]+2));
}

void serverWifi::setSSID(String param)
{
 param.toCharArray(SSID, param.length());
 //SSID[param.length()+1]='\0';
}

void serverWifi::setPW(String param)
{
   param.toCharArray(PW, param.length());
   //PW[param.length()+1]='\0';
}

void serverWifi::setHOST(String param)
{
  param.toCharArray(HOST, param.length());
  //HOST[param.length()+1]='\0';
}

void serverWifi::setport(String param)
{
  port=param.toInt();
}







