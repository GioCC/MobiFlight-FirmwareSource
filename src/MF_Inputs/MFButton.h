// MFButton.h
//
/// \mainpage MF Button module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Sebastian Moebius (mobiflight@moebiuz.de) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2013-2014 Sebastian Moebius

#ifndef MFButton_h
#define MFButton_h

#include <Arduino.h>
#include "MFIOdevice.h"

extern "C"
{
  // callback functions always follow the signature: void cmd(void);
  typedef void (*buttonEventHandler) (byte, uint8_t, const char *);
};

enum
{
  btnOnPress,
  btnOnRelease,
};


class MFButton: public MFIOdevice
{
private:
    static buttonEventHandler   _handler;    
    
    const char *  _name;
    uint8_t       _pin;
    bool          _state;
 
public:
    static uint8_t getType(void) { return kTypeButton; }
    static uint8_t getSize(void) { return sizeof(MFButton); }
    static void attachHandler(buttonEventHandler newHandler) { _handler = newHandler; }   

    MFButton(void);
    //MFButton(uint8_t pin, const char * name);
    
    void attach(uint8_t pin, const char * name = "Button");
 
    void update();
    void onReset(void);
 
    void trigger(uint8_t state);    // could be private
    
};
#endif 