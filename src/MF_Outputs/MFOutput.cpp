// MFOutput.cpp
//
// Copyright (C) 2013-2014

#include "MFOutput.h"

MFOutput::MFOutput(void)
{
    _pin = 0xFF;
    _value = false;
}

void MFOutput::attach(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
    setval(false);
}

void MFOutput::onReset(uint8_t action)
{
    if(action == ONRESET_DEFAULT) {
        setval(0);
    }
}

void MFOutput::powerSave(uint8_t state)
{
    if (state) {
        analogWrite(_pin, 0);
    } else {
        setval(_value);
    }
}

void MFOutput::setval(uint8_t value)
{
    _value = value;
    analogWrite(_pin, _value);
}

void MFOutput::detach(void)
{
    pinMode(_pin, INPUT);
    _pin = 0xFF;
}

// MFoutput.cpp