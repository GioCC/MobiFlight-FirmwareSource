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
#include "MFIOdevice.h"   // For constants and documentation only!
//#include "config.h"

class MFLCDDisplay  //: public MFIOdevice
{
public:
    static uint8_t getType(void) { return kTypeLcdDisplayI2C; }

    MFLCDDisplay(void);
    
    void    attach(byte address, byte cols, byte lines);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(const char *string);
    void    update(void) {};  // Stub Required for emulated polymorphism
    void    powerSave(bool state);
    void    test();

private:
    
    LiquidCrystal_I2C _lcdDisplay;
    byte _address;
    byte _cols;
    byte _lines;

    void _printCentered(const char *str, uint8_t line);
};
#endif