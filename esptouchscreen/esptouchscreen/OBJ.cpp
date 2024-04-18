#include "OBJ.h"

    OBJ::OBJ()
    {
        adat=new Data;
        wifi=new serverWifi;
    }
    OBJ::~OBJ()
    {
        delete adat;
        delete wifi;
    }
    Data* OBJ::getData()
    {
        return adat;
    }
    serverWifi* OBJ::getWifi()
    {
        return wifi;
    }