// MFStepper.h
//
/// \mainpage MF Stepper module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#pragma once

#include <stdlib.h>
#include <Arduino.h>
#include <AccelStepper.h>
#include "config.h"

class MFStepper  
{
public:
    static constexpr uint8_t  getType(void) { return kTypeStepper; }
    
    MFStepper();

    void    attach(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t pin3 = 3, uint8_t pin4 = 4, uint8_t btnPin1 = 0);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(long absolute);
    void    update(void);
    void    powerSave(uint8_t state);
   
    void    setMaxSpeed(float speed);
    void    setAcceleration(float acceleration);
    void    setZero();
    uint8_t getZeroPin();

private:
    AccelStepper _stepper;
    struct {
        uint8_t _initialized    :1;
        uint8_t _resetting      :1;
        uint8_t _zeroPinState   :1;
    };
    uint8_t     _zeroPin;
    long        _targetPos;

    void checkZeroPin(void);
    void setZeroInReset(void);

};

// MFStepper.h
