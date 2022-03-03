// MFButton.h
//
/// \mainpage MF Button module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#pragma once

#include <Arduino.h>
#include "MFIOdevice.h"   // For constants and documentation only!

extern "C" {
  // callback functions always follow the signature: void cmd(void);
  typedef void (*buttonEventHandler) (byte, uint8_t, const char *);
};

enum
{
  btnOnPress,
  btnOnRelease,
};


class MFButton  
//: public MFIOdevice
{
public:
    static uint8_t  getType(void) { return kTypeButton; }
    static void     attachHandler(buttonEventHandler newHandler) { _handler = newHandler; };   

    MFButton(void);
    
    
    void    attach(uint8_t pin, const char * name = "Button");
    void    detach(void) {};            // Stub required for emulated polymorphism
    void    reset(uint8_t action);
    void    update();
 
    void    trigger(uint8_t state);     // could be private
    
private:
    static buttonEventHandler   _handler;    
    
    const char *  _name;
    uint8_t       _pin;
    bool          _state;
 
};
// MFButton.h 