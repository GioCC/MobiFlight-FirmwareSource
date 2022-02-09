// MFStepper.h
//
/// \mainpage MF Stepper module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#ifndef MFStepper_h
#define MFStepper_h

#include <stdlib.h>
#include <AccelStepper.h>
#include "MFButton.h"

#include <Arduino.h>

class MFStepper
{
public:
    static uint8_t getType(void) { return kTypeStepper; }
    static uint8_t getSize(void) { return sizeof(MFStepper); }
    
    MFStepper();
    void setup(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t pin3 = 3, uint8_t pin4 = 4, uint8_t btnPin1 = 0);
    
    void onReset();
    void update();
    void powerSave(uint8_t state);
    void detach();

    void setval(long absolute);
    
    void setMaxSpeed(float speed);
    void setAcceleration(float acceleration);
    void setZero();
    uint8_t getZeroPin();

private:
    AccelStepper _stepper;
    uint8_t     _zeroPin;
    long        _targetPos;
    struct {
        uint8_t _initialized    :1;
        uint8_t _resetting      :1;
        uint8_t _zeroPinState   :1;
    };

    void checkZeroPin(void);
    void setZeroInReset(void);

};
#endif