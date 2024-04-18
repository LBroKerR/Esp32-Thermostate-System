#include "Button.h"
#include <Arduino.h>
const unsigned Buttons::getBTPl()
{
    return BTplus;
}

const unsigned Buttons::getBTM()
{
    return BTminus;
}

const unsigned Buttons::getBTpr()
{
    return BTprog;
}

float Buttons::tempchange(float step)
{
    if (digitalRead(getBTPl()))
    {
        delay(500);
        return step;
    }
    else if (digitalRead(getBTM()))
    {
        delay(500);
        return -step;
    }
    return 0.0;
}

int Buttons::progchange()
{
    while(digitalRead(getBTpr()))
    {
        if (digitalRead(getBTPl()))
        {
            delay(500);
            return 1;
        }
        else if (digitalRead(getBTM()))
        {
            delay(500);
            return -1;
        }

    }
    return 0;
}

Buttons::Buttons(unsigned const pinPl, unsigned const pinM, unsigned const pinPr)
{
    BTplus = pinPl;
    BTminus = pinM;
    BTprog = pinPr;
    pinMode(BTplus, INPUT);
    pinMode(BTminus, INPUT);
    pinMode(BTprog, INPUT);
}



