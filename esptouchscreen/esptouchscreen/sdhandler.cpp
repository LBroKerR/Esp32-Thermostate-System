#include "sdhandler.h"

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
}
void writeFile(fs::FS &fs, const char * path, String message) 
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