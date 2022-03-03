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
#include "MFIOdevice.h"   // For constants and documentation only!

class MFOutput  //: public MFIOdevice
{
public:
    static uint8_t getType(void) { return kTypeOutput; }

    MFOutput(void);
    
    void    attach(uint8_t pin);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(uint8_t value);
    void    update(void) {};  // Stub Required for emulated polymorphism
    void    powerSave(uint8_t state);

private:
    uint8_t       _pin;
    uint8_t       _value;
};
#endif 