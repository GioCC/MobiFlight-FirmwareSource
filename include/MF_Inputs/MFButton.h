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

/////////////////////////////////////////////////////////////////////
/// \class MFButton MFButton.h <MFButton.h>
class MFButton
{
 
public:
    static void attachHandler(buttonEventHandler newHandler) { _handler = newHandler; }   

    static uint8_t getType(void) { return kTypeButton; }
    static uint8_t getSize(void) { return sizeof(MFButton); }

private:
    static buttonEventHandler   _handler;    

public:
    MFButton(void);
    MFButton(uint8_t pin, const char * name);
    void attach(uint8_t pin, const char * name);
    void update();
    void trigger(uint8_t state);
    const char *  _name;
    uint8_t       _pin;
    
private:
    bool                 _state;
    
};
#endif 