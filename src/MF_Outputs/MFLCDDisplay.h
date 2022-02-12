// MFLCDDisplay.h
//
/// \mainpage MFLCDDisplay module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (info@mobiflight.com) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#ifndef MFLCDDisplay_h
#define MFLCDDisplay_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "MFIOdevice.h"
//#include "config.h"

class MFLCDDisplay: MFIOdevice
{
public:
    static uint8_t getType(void) { return kTypeLcdDisplayI2C; }
    static uint8_t getSize(void) { return sizeof(MFLCDDisplay); }

    MFLCDDisplay(void);
    void attach(byte address, byte cols, byte lines);

    void onReset(void);
    void powerSave(bool state);
    void detach(void);

    void setval(const char *string);
    
    void test();

private:
    
    LiquidCrystal_I2C _lcdDisplay;
    byte _address;
    byte _cols;
    byte _lines;

    void _printCentered(const char *str, uint8_t line);
};
#endif