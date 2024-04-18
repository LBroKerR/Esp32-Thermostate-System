
#include <SPI.h>
#include "FS.h"
#include "SD.h"
#include <ESP32Time.h>

#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
#define SD_CS 5
SPIClass Spi = SPIClass(HSPI);
#include "esp_task_wdt.h"
ESP32Time rtc(3600);
void setup() {
  // put your setup code here, to run once:
  Serial.begin( 115200 ); /*serial init */
  
  TaskHandle_t Core1Task;
  xTaskCreatePinnedToCore(Core1TaskFunction, "Core1Task", 10000, NULL/*&msg*/, 1, &Core1Task, 1);
}
float prog[5][24];

void Core1TaskFunction(void *parameter) 
{
   rtc.setTime(30, 24, 16, 17, 1, 2021);
  bool i=false;
  Spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SS, Spi, 80000000)) 
  {
    Serial.println("Card Mount Failed");
    i=true;
    //return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) 
  {
    Serial.println("No SD card attached");
    i=true;
    //return;
  }
  String text;
  if(!i)
  {
    //readFile(SD, "/init.txt", &text);
    //wifi.login(text);
    //readFile(SD, "/progs.txt", &text);
    //data1.ProgstringProcesseing(text);
    //readFile(SD, "/save.txt", &text);
    //data1.offsetstringProcessing(text);
  }
  bool test=false;
  String tmp="";
  for(int i=0; i<4;i++)
  {
    tmp+="anyad\n";
  }
  unsigned size=tmp.length()+1;
  char data[size];
  tmp.toCharArray(data, size);
  while(true)
  {
    //Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format 
  // formating options  http://www.cplusplus.com/reference/ctime/strftime/


  //struct tm timeinfo = rtc.getTimeStruct();
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");   //  (tm struct) Sunday, January 17 2021 07:24:38

  readFile(SD,"/init.txt", &tmp);
  Serial.println(rtc.getHour());
  delay(1000);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void ProgstringProcesseing(String param)
{
  // 24 float-> 3char/number-> space between numbers.
  unsigned size=param.length()+1;
  char data[size];
  param.toCharArray(data, size);
  unsigned array[24+1]={0};
  array[0]=-1;
  String tmp;
  float tmpf[24];
  for(int i=0; i<5; i++)
  {
    for(unsigned index=0, n=1; index<size && n<(24+1); index++)
    {
      if(data[index]==' ' ||data[index]=='\n' || data[index]=='\t' )
      {
        array[n]=index;
        n++;
      }
    }
    for(unsigned elem=1; elem<(24+1); elem++)
    {
      tmp=param.substring(array[elem-1]+1, array[elem]);
      tmpf[elem-1]=tmp.toFloat();
      prog[i][elem-1]=tmpf[elem-1];
    }
  }
}
void loglog(String param)
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
  String SSID=param.substring(0,array[0]);
  String PW=param.substring(array[0]+2, array[1]);
  String HOST=param.substring(array[1]+2,array[2]);
  String port=param.substring(array[2]+2);
  Serial.println(SSID);
  Serial.println(PW);
  Serial.println(HOST);
  Serial.println(port);
}
void offsetstringProcessing(String param)
{
  unsigned size=param.length()+1;
  char data[size];
  param.toCharArray(data, size);
  int array[3]={0};
  float tmpf[3]={0.0};
  for(unsigned i=0, n=0; i<size, n<3; i++)
  {
    if(data[i]==' ' || data[i]=='\n' || data[i]=='\t' )
    {
      array[n]=i;
      n++;
    }
  }
  String tmp=param.substring(0, array[0]);
  tmpf[0]=tmp.toFloat();
  tmp=param.substring(array[0]+1, array[1]);
  tmpf[1]=tmp.toFloat();
  tmp=param.substring(array[1]+1, array[2]);
  tmpf[2]=tmp.toFloat();
  for(unsigned i=0; i<3 ;i++)
  {
    Serial.println(tmpf[i]);
  }
}
void writeFile(fs::FS &fs, String path, String message) 
{
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) 
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) 
  {
    Serial.println("File written");
  } 
  else 
  {
    Serial.println("Write failed");
  }
  file.close();
}
void readFile(fs::FS &fs, const char * path, String* param) 
{
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) 
  {
    Serial.println("Failed to open file for reading");
    return ;
  }
 char buf;
 String tmp;
  Serial.print("Read from file: ");
  while (file.available()) 
  {
    buf=file.read();
    tmp+=buf;
  }
  file.close();
  *param=tmp;
  Serial.println(tmp);
}
