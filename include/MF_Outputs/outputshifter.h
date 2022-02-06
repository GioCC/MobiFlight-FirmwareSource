#ifndef _outputshifter_h
#define _outputshifter_h
#include "MFOutputShifter.h"

// ==== Inherited ====  (-> MF_device.cpp)
// OnInitShiftRegister
void MFOutputShifter::clear();
// -
void MFOutputShifter::powerSave(bool state) {};
// ClearShifters
void MFOutputShifter::detach();

// ==== Specialized ====
// setup
void AddShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name = "Shifter");
// setval
void OnSetShiftRegisterPins(char* pins, uint8_t value);

// ==== Additional ====
// -


#endif