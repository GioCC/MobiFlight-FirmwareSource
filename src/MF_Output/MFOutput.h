// MFOutput.h
//
/// \mainpage MF Button module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#pragma once

#include <Arduino.h>
#include "config.h"

class MFOutput  
{
public:
    static constexpr uint8_t  getType(void) { return kTypeOutput; }

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

// MFOutput.h
