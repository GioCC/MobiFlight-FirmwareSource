// MFButton.h
//
/// \mainpage MF Button module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#ifndef MFOutput_h
#define MFOutput_h

#include <Arduino.h>

class MFOutput
{
public:
    static uint8_t getType(void) { return kTypeOutput; }
    static uint8_t getSize(void) { return sizeof(MFOutput); }

    MFOutput(void);
    void setup(uint8_t pin);

    void onReset(void);
    void powerSave(uint8_t state);
    void detach(void);
    
    void setval(uint8_t value);

private:
    uint8_t       _pin;
    uint8_t       _value;
};
#endif 