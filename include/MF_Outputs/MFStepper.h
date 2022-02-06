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
    MFStepper();
    void attach(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t pin3 = 3, uint8_t pin4 = 4, uint8_t btnPin1 = 0);
    void detach();
    void update();
    void reset();
    void moveTo(long absolute);
    void setMaxSpeed(float speed);
    void setAcceleration(float acceleration);
    void setZero();
    uint8_t getZeroPin();

private:
    bool    _initialized;
    bool    _resetting;
    AccelStepper *_stepper;
    uint8_t _zeroPin;
    uint8_t _zeroPinState;
    long    _targetPos;

    void checkZeroPin(void);
    void setZeroInReset(void);

};
#endif