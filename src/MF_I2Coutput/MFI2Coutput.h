//
// MFI2Coutput.h
//
// (C) MobiFlight Project 2023
//

#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class MFI2Coutput
{
public:
    MFI2Coutput() { _initialized = false; }
    void display(const char *string);
    void attach(byte address, byte cols, byte lines);
    void detach() { _initialized = false; }
    void test() {}
    void powerSavingMode(bool state) {}

private:
    bool _initialized;
    byte _address;
    byte _size;
};

// MFLCDDisplay.h