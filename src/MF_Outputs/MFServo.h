// MFServo.h
//
/// \mainpage MF Servo module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#ifndef MFServo_h
#define MFServo_h

#include <Arduino.h>
#include <Servo.h>
#include "MFIOdevice.h"   // For constants and documentation only!


class MFServo  //: public MFIOdevice
{
public:
    static uint8_t getType(void) { return kTypeServo; }

    MFServo();		

    void    attach(uint8_t pin = 1, bool enable = true);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(int absolute);
    void    update(void);
    void    powerSave(uint8_t state);

    void    setExternalRange(int min, int max);
    void    setInternalRange(int min, int max);

private:
    uint8_t _pin;
    int		_mapRange[4];
    bool    _initialized;
    Servo   _servo;
    long    _targetPos;
    long    _currentPos;    
    int     speed;
};
#endif 
