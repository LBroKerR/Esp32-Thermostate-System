#pragma once
#ifndef OBJ_H
#define OBJ_H
#include "Data.h"
#include "serverWifi.h"

class OBJ
{
    Data *adat;
    serverWifi *wifi;
public:

    OBJ();
    ~OBJ();
    Data* getData();
    serverWifi* getWifi();
    
};

#endif // !OBJ_H