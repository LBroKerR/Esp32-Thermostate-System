#include "Data.h"


unsigned Data::getMaxProg()
{
	return maxprog;
}

Data::Data(float param, float tmp)
{
  setOffset(param);
  setWantedTmp(tmp);
	prog = 0;
	setMaxProg(0);
	HeatSwitch = false;
	temp = 0.0;
	humadity = 0.0;
  hmdoffset=0.0;
  changed_tmp=false;
  changed_wantedTmp=false; 
  changed_prog=false;
}
 void Data::setSensor(float tmp, float hmd)
{
  if(temp!=tmp)
  {
    changed_tmp=true;
  }
    temp=tmp;
    humadity=hmd;
}
void Data::ChangeWantedTmp(float param)
{
    if(param!=0)
    {
      wantedTmp+=param;
      changed_wantedTmp=true;
    }
}
void Data::setWantedTmp(float param)
{
  wantedTmp=param;
}
float Data::getWantedTmp()
{
    return wantedTmp;
}
float Data::getTemp()
{
	return temp+offset;
}

float Data::getHMD()
{
	return humadity+hmdoffset;
}
void Data::setProg(int param)
{
  prog=param;
}
void Data::ChangeProg(int param)
{
	if (param==1)
	{
    if(getMaxProg()==getProg())
    {
      prog=1;
    }
    else
    {
      prog++;
    }
    changed_prog=true;
  }
  else if(param==-1)
  {
    if(getProg()==0)
    {
      prog=getMaxProg();
    }
    else
    {
        prog--;
    }
    changed_prog=true;
  }
}
bool  Data::setHmdOffset(float param)
{
  hmdoffset=param;
}
void Data::setMaxProg(unsigned param)
{
	maxprog = param;
}

void Data::setHeatSwitch( bool param)
{
  HeatSwitch=param;
}

unsigned Data::getProg()
{
	return prog;
}

bool Data::getHeatSwitch()
{
	return HeatSwitch;
}
void Data::setOffset(float param)
{
  offset=param;
}
float Data::getOffset()
{
  return offset;
}
float  Data::getData(int id)
{
  switch(id)
  {
    case WANTEDTMP: return getWantedTmp();
    case OFFSET: return getOffset();
    case PROG: return getProg()-1;
    case MAXPRG: return getMaxProg();
    case SWITCH: return getHeatSwitch();
    case TMP: return getTemp();
    default: return 0.0;
  }
  return 0.0;
}
bool Data::setData(float param, int id)
{
  if(id<0)
  {
    id*=-1;
  }
  switch(id)
  {
    case WANTEDTMP: if(param<=35 && param>=0){setWantedTmp(param);return true;} 
    case OFFSET: if(param<=10 && param>=0){setOffset(param);return true;} 
    case PROG: if(param<=getMaxProg() && param>=0){setProg(param+1);return true;} 
    case MAXPRG: if(param<=5 && param>=0){setMaxProg(param);return true;} 
    case SWITCH: if(param<=1 && param>=0){setHeatSwitch((bool)param);return true;}
    case HMDOFFSET: if(param<=10 && param>=0){setHmdOffset(param);return true;}
    default: return false;
  }
  return false;
}

  void Data::setChanged_tmp(bool param)
  {
    changed_tmp=param;
  }
  void Data::setChanged_wantedTmp(bool param)
  {
    changed_wantedTmp=param;
  }
  void Data::setChanged_prog(bool param)
  {
    changed_prog=param;
  }

  bool  Data::getChanged_tmp()
  {
    return changed_tmp;
  }
  bool  Data::getChanged_wantedTmp()
  {
    return changed_wantedTmp;
  }
  bool  Data::getChanged_prog()
  {
    return changed_prog;
  }
