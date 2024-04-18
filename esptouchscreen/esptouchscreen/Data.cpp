#include "Data.h"

Data::Data()
{
  wanted_tmp=19.5, hmd=0.0, tmp=0.0, tmp_carryable=0.0, offset=0.0, carryableOffset=0.0;
  wifiwtmp=false;
  sdoffset=false;
  for(unsigned n=0; n<PROGNUMBER; n++)
  {
    for(unsigned i=0; i<SIZEWTMP; i++)
    {
       prog[n][i]=wanted_tmp;
    }
  }
  current_prog=0;
  progHour_tmp_changed=false;
  Heat_switch=false;
  prevHeat_switch=false;
  heat_option=false;
  wifiprognumber=false;
  hourinit=0, minuteinit=0;
  current_progHour=0;
  switched=false;
  wtmp=false;
  prognumber=false;
  WhyNeedThis=false;
}
void Data::setOffset(float param)
{
  offset=param;
}
void Data::setWantedTmp(float param)
{
  if(param!=wanted_tmp)
  {
    wanted_tmp=param;
    setHaveToSendWtmp(true);
  }
}

void Data::setTmp(float param)
{
  tmp=param+getOffset();
}

void Data::setTmpCarryable(float param)
{
  tmp_carryable=param;
}

void Data::setCurrent_Prog_Hour(unsigned param, unsigned index)
{
    prog[getCurrent_progNumber()][index]=param;
}


void Data::setHmd(float param)
{
  hmd=param;
}

void Data::setCurrent_progNumber(float param)
{
   current_prog=(unsigned)param;
}

void Data::setHeatSwitch()
{
  if((getTmp()<=getWantedTmp()-1.0 &&  getTmp()!= 0.0 )|| getTmpCarryasble() <=getWantedTmp()-1.0)
  {
    Heat_switch=true;
  }
  else if((getTmp()>= getWantedTmp()+0.5 &&  getTmp()!= 0.0 ) || getTmpCarryasble()>= getWantedTmp()+0.5)
  {
    Heat_switch=false;
  }
  setSwitched();
}

void Data::setcarryableOffset(float param)
{
  carryableOffset=param;
}

float Data::getOffset()
{
  return offset;
}
float Data::getWantedTmp()
{
	return wanted_tmp;
}

float Data::getHmd()
{
	return hmd;
}

float Data::getTmp()
{
	return tmp;
}

float Data::getTmpCarryasble()
{
	return  tmp_carryable;
}

float Data::getCurrent_Prog_Hour(unsigned index)
{
	return prog[getCurrent_progNumber()][index];
}

unsigned Data::getCurrent_progNumber()
{
	return current_prog;
}

bool Data::getHeatSwitch()
{
	return Heat_switch;
}

float Data::getCarryableOffset()
{
  return carryableOffset;
}

float  Data::getData(int id)
{
  switch(id)
  {
    case WANTEDTMP: return getWantedTmp();
    case OFFSET: return getCarryableOffset();
    case PROG: return getCurrent_progNumber();
    case MAXPRG: return PROGNUMBER;
    case SWITCH: return getHeatSwitch();
    default: 0.0;
  }
  return 0.0;
}

bool Data::setData(float param, int id)
{
   switch(id)
  {
    case WANTEDTMP: if(param<=35 && param>=0){setWantedTmp(param);wtmpChangedThroughtWiFi(true);return true;}
    case OFFSET: if(param<=10 && param>=0){setcarryableOffset(param); return true;}
    case PROG :if(param<=PROGNUMBER && param>=0){setCurrent_progNumber((unsigned)param); setProgNumberChanged(true);return true; }
    case MAXPRG: break;//setMaxProg(param); break;
    case SWITCH: if(param<=1 && param>=0) {setHeatSwitch(); return true;}
    case TMP: if(param<=50 && param>=-50) {setTmpCarryable(param); return true;}
    default: return false;
  }
  return false;

}

void Data::ProgstringProcesseing(String param)
{
  // 24 float-> 3char/number-> space between numbers.
  unsigned size=param.length()+1;
  char data[size];
  param.toCharArray(data, size);
  unsigned array[24+1]={0};
  array[0]=-1;
  String tmp;
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
      prog[i][elem-1]=tmp.toFloat();
    }
  }
}
void Data::offsetstringProcessing(String param)
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
  tmp=param.substring(array[0]+1, array[1]+1);
  tmpf[1]=tmp.toFloat();
  tmp=param.substring(array[1]+1, array[2]+1);
  tmpf[2]=tmp.toFloat();
  setCurrent_progNumber(tmpf[0]-1);
  setProgNumberChanged(true);
  setOffset(tmpf[1]);
  setoffsetchanged(true);
  setcarryableOffset(tmpf[2]);
}

void Data::clockinit(String param)
{
  unsigned size=param.length()+1;
  char data[size];
  int array[2]={0};
  param.toCharArray(data, size);
  for(unsigned i=0, n=0; i<size, n<3; i++)
  {
    if(data[i]==' ' || data[i]=='\n' || data[i]=='\t' )
    {
      array[n]=i;
      n++;
    }
  }
  String tmp=param.substring(0, array[0]);
  hourinit=tmp.toInt();
  if(hourinit<0 || hourinit>23)
  {
    hourinit=0;
  }
  tmp=param.substring(array[0]+1, array[2]+1);
  minuteinit=tmp.toInt();
  if(minuteinit<0 || minuteinit>59)
  {
    minuteinit=0;
  }
}

unsigned Data::getHourInit()
{
  return hourinit;
}

unsigned Data::getMinutinit()
{
  return minuteinit;
}

void Data::setWtmpChanged(bool param)
{
  wtmp=param;
}
bool Data::getWtmpChanged()
{
  return wtmp;
}
void Data::setoffsetchanged(bool param)
{
  sdoffset=param;
}
bool Data::getoffsetchaged()
{
  return sdoffset;
}

void Data::setHeaterOption(bool param)
{
  heat_option=param;
}
bool Data::getHeaterOption()
{
  return heat_option;
}

void Data::setProgNumberChanged(bool param)
{
  prognumber=param;
}
bool Data::getProgNumberChanged()
{
  return prognumber;
}
void Data::setprogHour_tmp_changed(bool param)
{
  progHour_tmp_changed=param;
}
bool Data::getprogHour_tmp_changed()
{
  return progHour_tmp_changed;
}
float*Data::getCurrentprog()
{
  return prog[getCurrent_progNumber()];
}

void Data::setcurrent_progHour(unsigned param)
{
    current_progHour=param;
}

unsigned  Data::getcurrent_progHour()
{
  return current_progHour;
}

float* Data::getprogByindex(unsigned param)
{
  return prog[param];
}

void Data::setSwitched(bool param)
{
  switched=param;
}

void Data::setSwitched()
{
  if(prevHeat_switch!=getHeatSwitch())
  {
    switched=true;
    prevHeat_switch=getHeatSwitch();
  }
}
bool Data::getSwitched()
{
  return switched;
}

bool Data::wtmpSatThroughtWiFi()
{
  return wifiwtmp;
}
void Data::wtmpChangedThroughtWiFi(bool param)
{
  wifiwtmp=param;
}

void  Data::setWifiprognumber(bool param)
{
   wifiprognumber=param;
}
bool  Data::getWifiprognumber()
{
  return  wifiprognumber;
}

bool Data::Shouldwtmpsending()
{
  return WhyNeedThis;
}
void Data::setHaveToSendWtmp(bool param)
{
 WhyNeedThis=param;
}
