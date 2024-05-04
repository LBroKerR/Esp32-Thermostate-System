#pragma once
#ifndef DATA_H
#define DATA_H

#define PROGNUMBER 5
#define SIZEWTMP 24 

#define WANTEDTMP 2 //id 2
#define OFFSET 3 //id 3
#define PROG 4//id 4
#define MAXPRG 5// id 5
#define SWITCH 6 //id 6
#define TMP 9// id 9

#include <Arduino.h>

class Data
{

	float wanted_tmp, hmd, tmp, tmp_carryable, prog[PROGNUMBER][SIZEWTMP], offset, carryableOffset;
	unsigned current_prog;
  bool heat_option;//false means to use autoheating/ true means to use gasheater
  unsigned current_progHour;
	bool Heat_switch, switched, prevHeat_switch;// to do heatswitch_changed?
  bool wifiwtmp, wtmp,sdoffset, wifiprognumber, progHour_tmp_changed,prognumber, ShouldSendWtmp;
  unsigned hourinit, minuteinit; 

public:
	Data();
  
  void setOffset(float);
  void setcarryableOffset(float);
	void setWantedTmp(float);
	void setTmp(float);
	void setTmpCarryable(float);
	void setCurrent_Prog_Hour(unsigned, unsigned);
	void setHmd(float);
	void setCurrent_progNumber(float);
	void setHeatSwitch();
  bool setData(float, int);
  void setWtmpChanged(bool param);
  void setoffsetchanged(bool param);
  void setHeaterOption(bool param);
  void setProgNumberChanged(bool param);
  void setprogHour_tmp_changed(bool);
  void setcurrent_progHour(unsigned param);

  float getCarryableOffset();
  float getOffset();
	float getWantedTmp();
	float getHmd();
	float getTmp();
	float getTmpCarryasble();
	float getCurrent_Prog_Hour(unsigned);//indexhez tartozó értéket
  float*getCurrentprog();
	unsigned getCurrent_progNumber();
	bool getHeatSwitch();
  unsigned getHourInit();
  unsigned getMinutinit();
  float getData(int);
  bool getWtmpChanged();
  bool getoffsetchaged();
  bool getHeaterOption();
  bool getProgNumberChanged();
  bool getprogHour_tmp_changed();
  unsigned getcurrent_progHour();//index
  float* getprogByindex(unsigned param);

  void ProgstringProcesseing(String);
  void offsetstringProcessing(String);
  void clockinit(String);

  void setSwitched(bool param);
  void setSwitched();
  bool getSwitched();
  
  bool wtmpSatThroughtWiFi();
  void wtmpChangedThroughtWiFi(bool param);

  void setWifiprognumber(bool param);
  bool getWifiprognumber();

  bool Shouldwtmpsending();
  void setHaveToSendWtmp(bool);

};
#endif // !DATA_H
