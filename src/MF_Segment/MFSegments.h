// MFSegments.h
//
/// \mainpage MF LED display module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#pragma once

#include <Arduino.h>
#include <LedControl.h>
#include "config.h"


class MFSegments  
{
public:
    static constexpr uint8_t  getType(void) { return kTypeLedSegment; }

    MFSegments(void);

    void    attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    void    update(void) {};  // Stub required for emulated polymorphism
    void    powerSave(bool state);
    
    void    setBrightness(byte module, byte value);
    void    test(void);

private:
    LedControl  _ledControl;
    byte        _moduleCount;
};

// MFSegments.h
