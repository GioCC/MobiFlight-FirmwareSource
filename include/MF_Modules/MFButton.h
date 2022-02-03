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
  typedef void (*buttonEvent) (byte, uint8_t, const char *);
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
    MFButton(void);
    MFButton(uint8_t pin, const char * name);
    static void attachHandler(buttonEvent newHandler);    
    void attach(uint8_t pin, const char * name);
    void update();
    void trigger(uint8_t state);
    const char *  _name;
    uint8_t       _pin;
    
private:
    static buttonEvent   _handler;    
    bool          _state;
    
};
#endif 