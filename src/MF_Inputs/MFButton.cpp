// MFButton.cpp
//
// Copyright (C) 2013-2014

#include "MFButton.h"

buttonEventHandler   MFButton::_handler = NULL;

MFButton::MFButton(void)
{ 
    setup(0xFF, "Button");
}

// MFButton::MFButton(uint8_t pin, const char * name)
// { 
//     setup(pin, name);
// }

void MFButton::setup(uint8_t pin, const char * name)
{   
  _pin  = pin;
  _name = name;
  _state = 1;
  pinMode(_pin, INPUT_PULLUP);     // set pin to input
}

void MFButton::update()
{
    uint8_t newState = (uint8_t) digitalRead(_pin);
    if (newState!=_state) {     
      _state = newState;
      trigger(_state);      
    }
}

void MFButton::trigger(uint8_t state)
{
    if(_handler) {
        if (_state==LOW) {
            (*_handler)(btnOnPress,   _pin, _name);
        } else {
            (*_handler)(btnOnRelease, _pin, _name);
        }
    }
}

void MFButton::onReset(void)
{
    if(_handler) {
        (*_handler)(btnOnRelease, _pin, _name);
        (*_handler)(btnOnPress,   _pin, _name);
    }
}
