#pragma once
#ifndef DATA_H
#define DATA_H

#define WANTEDTMP 2 //id 2
#define OFFSET 3 //id 3
#define PROG 4//id 4
#define MAXPRG 5// id 5
#define SWITCH 6 //id 6
#define HMDOFFSET 7
#define TMP 9// id 9

class Data
{
  float wantedTmp;
  float offset, hmdoffset;
	float temp;
	float humadity;
	unsigned prog;
	unsigned maxprog;
	bool HeatSwitch;

  bool changed_tmp, changed_wantedTmp, changed_prog;
	unsigned getMaxProg();
// to do: hmd offset 
public:
	Data(float offset=-3.4, float tmp=20.0);
  void setSensor(float, float);
  void ChangeWantedTmp(float);
  void setWantedTmp(float);
  float getWantedTmp();
	float getTemp();
	float getHMD();
  float getData(int id);
  float getOffset();

  bool setHmdOffset(float);
  bool setData(float, int);
  void setProg(int);
  void ChangeProg(int);
	void setMaxProg(unsigned);
	void setHeatSwitch(bool);
  void setOffset(float);

	unsigned getProg();
	bool getHeatSwitch();

  void setChanged_tmp(bool);
  void setChanged_wantedTmp(bool);
  void setChanged_prog(bool);
  bool getChanged_tmp();
  bool getChanged_wantedTmp();
  bool getChanged_prog();
};




#endif // !TEMP_H
