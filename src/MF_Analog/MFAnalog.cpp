//
// MFAnalog.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "MFAnalog.h"

analogEventHandler MFAnalog::_handler = NULL;

void MFAnalog::attach(uint8_t pin, uint8_t sensitivity, const char *name)
{
    _sensitivity = sensitivity;
    _pin         = pin;
    _name        = name;
    pinMode(_pin, INPUT_PULLUP); // set pin to input. Could use OUTPUT for analog, but shows the intention :-)
    // Fill averaging buffers with initial reading
    for (uint8_t i = 0; i < ADC_MAX_AVERAGE; i++) {
        updateAverage();
    }
    // and set initial value from buffers
    _lastValue = ADC_Average_Total >> ADC_MAX_AVERAGE_LOG2;
}

bool MFAnalog::valueHasChanged(int16_t newValue)
{
    return (abs(newValue - _lastValue) >= _sensitivity);
}

void MFAnalog::update()
{
    int16_t newValue = ADC_Average_Total >> ADC_MAX_AVERAGE_LOG2;
    if (valueHasChanged(newValue)) {
        _lastValue = newValue;
        if (_handler) {
            (*_handler)(_lastValue, _pin, _name);
        }
    }
}

void MFAnalog::updateAverage()
{                                                           // read ADC and calculate floating average, call it every ~10ms
    ADC_Average_Total -= ADC_Buffer[(ADC_Average_Pointer)]; // subtract oldest value to save the newest value
    ADC_Buffer[ADC_Average_Pointer] = analogRead(_pin);     // store read in, must be subtracted in next loop
    ADC_Average_Total += ADC_Buffer[ADC_Average_Pointer];   // add read in for floating average
    ADC_Average_Pointer++;                                  // prepare for next loop
    ADC_Average_Pointer &= (ADC_MAX_AVERAGE - 1);           // limit max. values for floating average
}

// MFAnalog.cpp