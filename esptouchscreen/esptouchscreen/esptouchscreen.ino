/*
 Name:		esptouchscreen.ino
 Created:	3/3/2024 6:53:19 PM
 Author:	rabai
*/
#include "OBJ.h"
#include <DHT.h>
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <SPI.h>
#include <ESP32Time.h>
#include "SD.h"
#include "sdhandler.h"
#define SWITCHPIN 22
#define AUTOMODE 1
#define GASMODE 2
#define OFFMODE 3
#define DHTTYPE DHT11
#define DHTPIN 27

#define UTC_MINUS_1 3600
#define CHARTSIZE 24
#define SD_SCK 18
#define SD_MISO 19
#define SD_MOSI 23
#define SD_CS 5

#define WIFI 1
#define USE_UI 1         //if you want to use the ui export from Squareline ,please define USE_UI.
#define Display_28        //according to the board you using ,if you using the ESP32 Display 3.5inch board, please define 'Display_35'.if using 2.4inch board,please define 'Display_24'.
#include <lvgl.h>
#ifdef USE_UI
#include "ui.h"
#endif
#include <XPT2046_Touchscreen.h>
#if defined Display_35      //ESP32 Display 3.5inch Board
/*screen resolution*/
static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 320;
uint16_t calData[5] = { 353, 3568, 269, 3491, 7  };     /*touch caldata*/

#elif defined Display_24    //ESP32 Display 2.4inch Board
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
uint16_t calData[5] = { 557, 3263, 369, 3493, 3  };

#elif defined Display_28    //ESP32 Display 2.8inch Board
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
uint16_t calData[5] = { 189, 3416, 359, 3439, 1 };
#endif
TFT_eSPI lcd = TFT_eSPI(); /* TFT entity */
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

SPIClass *mySpi = new SPIClass(VSPI);
SPIClass *SDSpi = new SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);

#if defined USE_UI
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[ screenWidth * screenHeight / 13 ];

//_______________________
/* display flash */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  lcd.startWrite();
  lcd.setAddrWindow( area->x1, area->y1, w, h );
  lcd.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  lcd.endWrite();

  lv_disp_flush_ready( disp );
}
/*touch read*/
uint16_t touchX, touchY;
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
  if(!ts.tirqTouched() && !ts.touched())
  {
        data->state = LV_INDEV_STATE_REL;
  }
  else
  {
        TS_Point p = ts.getPoint();
        uint16_t X, Y;
        X = map(p.x, 200, 3700, 1, screenWidth);  /* Touchscreen X calibration */
        Y = map(p.y, 240, 3800, 1, screenHeight); /* Touchscreen Y calibration */
        if(touchX!=X || touchY!=Y)
        {
          touchX=X;
          touchY=Y;
         data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
          data->point.x = X;
          data->point.y = Y;

          Serial.print("Data x ");
          Serial.println(X);

          Serial.print("Data y ");
          Serial.println(Y);
        }
  }
}
#endif

void setup() 
{
	Serial.begin( 115200 ); /*serial init */
  OBJ* obj=new OBJ;
    bool i=false;
  SDSpi->begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SS, *SDSpi, 80000000)) 
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
    readFile(SD, "/init.txt", &text);
    obj->getWifi()->login(text);
    readFile(SD, "/progs.txt", &text);
    obj->getData()->ProgstringProcesseing(text);
    readFile(SD, "/save.txt", &text);
    obj->getData()->offsetstringProcessing(text);
    readFile(SD, "/clock.txt", &text);
    obj->getData()->clockinit(text);
    SD.end();
  }
  SDSpi->end();
    TaskHandle_t Core1Task;
    xTaskCreatePinnedToCore(Core1TaskFunction, "Core1Task", 10000, obj/*&msg*/, 1, &Core1Task, 1);
    #if WIFI
    TaskHandle_t Core0Task;
    xTaskCreatePinnedToCore(Core0TaskFunction, "Core0Task", 10000, obj/*&msg*/, 1, &Core0Task, 0);
    #endif
}
#if WIFI
void CarryableDataCheck(OBJ*obj)
{
 if(obj->getWifi()->receiving())
  {
    obj->getWifi()->setReceived(true);
    if(obj->getWifi()->getReceivedID()<0)//init request
    {
      obj->getWifi()->sending(obj->getData()->getData(-(obj->getWifi()->getReceivedID())),-(obj->getWifi()->getReceivedID()));
    }
    else if(obj->getWifi()->getReceivedID()>0)//data changed msg
    {
      obj->getData()->setData(obj->getWifi()->getReceivedData(),obj->getWifi()->getReceivedID());
    }
  }
  else
  {
    obj->getWifi()->setReceived(false);
  }
}

void  sendChangedDatas(OBJ*obj)
{ //heatswitch, wanted tmp, , current prognumber // elküldendők
  //Heat_switch, getWantedTmp(), , current_prog      // hozzátartozó változók
  //még nincs       wifiwtmp           wifiprognumber  // bool változók, amiket figyelünk, hogy kell küldeni
  if(obj->getData()->Shouldwtmpsending())//
  {
    obj->getWifi()->sending(obj->getData()->getData(WANTEDTMP),WANTEDTMP);
    obj->getData()->setHaveToSendWtmp(false);
  }
  if(obj->getData()->getWifiprognumber())
  {
    obj->getWifi()->sending(obj->getData()->getData(PROG),PROG);
    obj->getData()->setWifiprognumber(false);
  }
  if(obj->getData()->getSwitched())
  {
    obj->getWifi()->sending(obj->getData()->getData(SWITCH),SWITCH);
    obj->getData()->setSwitched(false);
  }
}

void Core0TaskFunction(void *parameter) 
{
  OBJ* obj=(OBJ*)parameter;
  obj->getWifi()->systembegin();
  while(true)
  {
    if(!obj->getWifi()->getIsConneted())
    {
      obj->getWifi()->systembegin();
    }
    if(obj->getWifi()->getClient())
    {
      CarryableDataCheck(obj);
      sendChangedDatas(obj);
    }
    delay(5);
  }
}
#endif

void all_elementsOfchart_update(lv_coord_t* array, unsigned size, Data* data1)
{
  for(unsigned i=0; i<size; i++)
  {
    array[i]=(int)(data1->getCurrent_Prog_Hour(i)*10.0);
  }
}
void updateScreen2(lv_coord_t* array,lv_chart_series_t * ui_TmpChart_series_1, OBJ *obj)
{
  bool chartprint=false;
  int index=lv_roller_get_selected(ui_Roller2);
  int progindex=lv_roller_get_selected(ui_Roller1);
  String str;
  if(obj->getData()->getProgNumberChanged())
  {
    lv_roller_set_selected(ui_Roller1, (uint16_t) obj->getData()->getCurrent_progNumber(), LV_ANIM_OFF);
    all_elementsOfchart_update(array, CHARTSIZE,obj->getData());
    obj->getData()->setProgNumberChanged(false);
    chartprint=true;
  }
  else
  {
    if(progindex!=obj->getData()->getCurrent_progNumber())
    {
      obj->getData()->setprogHour_tmp_changed(true);
      obj->getData()->setCurrent_progNumber(progindex);
      obj->getData()->setWifiprognumber(true);
      //chart array elems changing;
      all_elementsOfchart_update(array, CHARTSIZE, obj->getData());
      //print chart if array changed 
      chartprint=true;
    }
  }
  if(obj->getData()->getprogHour_tmp_changed())
  {
    //lv_roller_set_selected(ui_Roller2, (uint16_t) index, LV_ANIM_OFF); //does it need it?
    lv_slider_set_value(ui_wtmpSLider, (int32_t)(obj->getData()->getCurrent_Prog_Hour(index)*10.0), LV_ANIM_OFF);
    obj->getData()->setcurrent_progHour(index);
    str= String((float)(obj->getData()->getCurrent_Prog_Hour(index)));
    lv_label_set_text(ui_WTMPSliderLable, str.c_str());
    obj->getData()->setprogHour_tmp_changed(false);
  }
  else
  {
    if( obj->getData()->getcurrent_progHour()!=index)
    {
      obj->getData()->setcurrent_progHour(index);
      obj->getData()->setprogHour_tmp_changed(true);
    }
    else
    {
      if(((float)(lv_slider_get_value(ui_wtmpSLider)/10.0))!= obj->getData()->getCurrent_Prog_Hour(index))
      {
        str= String((float)(lv_slider_get_value(ui_wtmpSLider)/10.0));
        lv_label_set_text(ui_WTMPSliderLable, str.c_str());
        obj->getData()->setCurrent_Prog_Hour((float)(lv_slider_get_value(ui_wtmpSLider)/10.0), index);
        //chart array[index] equal to previously set data
        array[index]=(int)(obj->getData()->getCurrent_Prog_Hour(index)*10.0);
        //print chart if array element changed 
        chartprint=true;
      }
    }
  }
  if(chartprint)
  {
    lv_chart_set_ext_y_array(ui_TmpChart, ui_TmpChart_series_1, array);
  }
}

void updateScreen3(OBJ *obj)
{
 if(obj->getData()->getoffsetchaged())
 {
    lv_slider_set_value(ui_offsetSlider, (int32_t)(obj->getData()->getOffset()*10.0), LV_ANIM_OFF);
    obj->getData()->setoffsetchanged(false);
 }
 else
 {
    obj->getData()->setOffset(((float)lv_slider_get_value(ui_offsetSlider))/10.0);
 }
    String tmp=String(obj->getData()->getOffset());
    lv_label_set_text(ui_offsetNumberLabel, tmp.c_str());

    obj->getData()->setHeaterOption((bool)lv_dropdown_get_selected(ui_HeatSetting));
}

void updateScreen1(ESP32Time *rtc,lv_coord_t* array,lv_chart_series_t * ui_TmpChart_series_1, OBJ *obj)
{ 
  if(obj->getData()->getWtmpChanged())
  {
    if(!obj->getData()->wtmpSatThroughtWiFi())
    {
      obj->getData()->setWantedTmp(obj->getData()->getCurrent_Prog_Hour(rtc->getHour(true)));
    }
     //Serial.println(obj->getData()->getWantedTmp());
    lv_arc_set_value(ui_WtmpARC,(obj->getData()->getWantedTmp()*10.0));
    obj->getData()->setWtmpChanged(false);
  }
  else
  {
    if(!obj->getData()->wtmpSatThroughtWiFi())
    {
      obj->getData()->setWantedTmp(((float)lv_arc_get_value(ui_WtmpARC))/10.0);
    }
    obj->getData()->getCurrentprog()[rtc->getHour(true)]=obj->getData()->getWantedTmp();
    if(((int)array[rtc->getHour(true)])!=(int)(obj->getData()->getWantedTmp()*10.0))
    {
      array[rtc->getHour(true)]=(int)(obj->getData()->getWantedTmp()*10.0);
      lv_chart_set_ext_y_array(ui_TmpChart, ui_TmpChart_series_1, array);
      obj->getData()->wtmpChangedThroughtWiFi(false);
    }
    obj->getData()->setWtmpChanged(true);
  }
  if(obj->getData()->getHeatSwitch())
  {
    lv_label_set_text(ui_Label1, "ON");
  }
  else
  {
    lv_label_set_text(ui_Label1, "OFF");
  }
  if(obj->getWifi()->getClient() && !obj->getWifi()->CliensDisconnect(rtc->getSecond()))
  {
    lv_label_set_text(ui_serverlabel, "Client active");
  }
  else
  {
    lv_label_set_text(ui_serverlabel, "No Client");
  }
  if(obj->getWifi()->getIsConneted())
  {
    lv_label_set_text(ui_wifiLabel, "Conneted!");
  }
  else
  {
    lv_label_set_text(ui_wifiLabel, "Disconneted!");
  }
  String tmp=String(obj->getData()->getWantedTmp())+" °C";
  lv_label_set_text(ui_WtmpLabel, tmp.c_str());
  tmp=String(obj->getData()->getTmp())+" °C";
  lv_label_set_text(ui_TmpLabel, tmp.c_str());
  tmp=String(obj->getData()->getHmd())+" %";
  lv_label_set_text(ui_HmdLabel, tmp.c_str());
  if(rtc->getMinute()<10)
  {
    tmp=String(rtc->getHour(true))+":0"+ String(rtc->getMinute());
  }
  else
  {
    tmp=String(rtc->getHour(true))+":"+ String(rtc->getMinute());
  }
  lv_label_set_text(ui_TimeLabel, tmp.c_str());
}

bool prepair_to_write(ESP32Time *rtc, Data *data1, bool enable)
{
  //Serial.println(SD.)
  if(rtc->getMinute()%1==0 && rtc->getSecond()==0 && !enable)// || save button
  {//write out progs, offsets, clock
    mySpi->end();
    SDSpi->begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  if (!SD.begin(SS, *SDSpi, 80000000)) 
  {
    Serial.println("Card Mount Failed");
    SDSpi->end();
    mySpi->begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    return false;
  }
   String tmp=String(rtc->getHour(true))+" "+String(rtc->getMinute());
   writeFile(SD, "/clock.txt", tmp);
   tmp=String(data1->getCurrent_progNumber()+1)+"\n"+String(data1->getOffset())+"\n"+String(data1->getCarryableOffset());
   writeFile(SD, "/save.txt", tmp);
   tmp="";
   for(unsigned i=0; i<5; i++)
   {
    float*array=data1->getprogByindex(i);
    for(unsigned elem=0; elem<CHARTSIZE ;elem++)
    {

      tmp+=String(array[elem]);
      if(elem+1!=CHARTSIZE)
      {
        tmp+="\t";
      }
    }
    if(i+1!=5)
    {
      tmp+="\n";
    }
   }
   writeFile(SD, "/progs.txt", tmp);
   SD.end();
   SDSpi->end();
   mySpi->begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
 }
 return true;
}

void mesureSensor(DHT *sensor, Data* data1)
{
  data1->setHmd(sensor->readHumidity());
  data1->setTmp(sensor->readTemperature());
}
void switch_To_mode(unsigned number)
{
  // on->  switch  1->0 autoheater
  //               1->0->1->0 gasheater
  // off-> switch  1->0->1->0->1->0 off
  for(unsigned i=0; i<number; i++)
  {
    digitalWrite(SWITCHPIN, HIGH);
    delay(100);
    digitalWrite(SWITCHPIN, LOW);
  }
}
void switching_modes(bool option)// on-> switch 1->0 off-> 1->0->1->0->1->0
{
    if(option)
    {
      switch_To_mode(AUTOMODE);
    }
    else
    {
      switch_To_mode(GASMODE);
    }
}
void heater(Data* data) //on-> switch 1->0->1->0 off-> 1->0->1->0->1->0
{
  if( data->getSwitched())
  {
    if(data->getHeatSwitch())// trun on
    {
      switching_modes(data->getHeaterOption());
    }
    else //turn off
    {
      switch_To_mode(OFFMODE);
    }
  }
}
void Core1TaskFunction(void *parameter) 
{
  OBJ* obj=(OBJ*)parameter;
    //clock begin
  ESP32Time rtc(UTC_MINUS_1);
  rtc.setTime(1, obj->getData()->getMinutinit(), obj->getData()->getHourInit()-1, 17, 1, 2021);
  //          s   m   h+1 d   m   y   //could this be read from file?
  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);
   mySpi->begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
  ts.begin(*mySpi); /* Touchscreen init */
  ts.setRotation(1); /* Landscape orientation */
  //LCD init
  lcd.begin();          
  lcd.setRotation(1); 
  lcd.fillScreen(TFT_BLACK);
  delay(100);
  //background light pin
  pinMode(21, OUTPUT);//27
  digitalWrite(21, HIGH);//27 volt


#if defined USE_UI
  //lvgl init
  lv_init();
  lv_disp_draw_buf_init( &draw_buf, buf1, NULL, screenWidth * screenHeight / 13 );

  /*Display init*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init( &disp_drv );
  /*Display driver port of LVGL*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  /*touch driver port of LVGL*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );

  ui_init();        //LVGL UI init
#endif
  Serial.println( "Setup done" );
  DHT sensor(DHTPIN,DHTTYPE);
  pinMode(SWITCHPIN, OUTPUT);
  sensor.begin();

  lv_chart_series_t * ui_TmpChart_series_1 = lv_chart_add_series(ui_TmpChart, lv_color_hex(0x19ACBA),LV_CHART_AXIS_PRIMARY_Y);
  lv_coord_t ui_TmpChart_series_1_array[24] = { 0 };
  bool sdEnable=false;

  while(true)
  {
    mesureSensor(&sensor, obj->getData());
    obj->getData()->setHeatSwitch();
    #if defined USE_UI
      lv_timer_handler();
      //updata data from sliders, bar, and arc, lables
      // set sliders, bars, arc, and diagramm, lables
      updateScreen1(&rtc,ui_TmpChart_series_1_array, ui_TmpChart_series_1, obj);
      updateScreen2(ui_TmpChart_series_1_array, ui_TmpChart_series_1, obj);
      updateScreen3(obj);
      sdEnable=prepair_to_write(&rtc,obj->getData(),sdEnable);
      if(sdEnable && rtc.getSecond()!=0)
      {
        sdEnable=false;
      }
      delay(5);
    #else
      while(1);
    #endif
    heater(obj->getData());
  }
}
// the loop function runs over and over again until power down or reset
void loop() 
{
  //do not use!
}
