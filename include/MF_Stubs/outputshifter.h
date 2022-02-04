#ifndef _outputshifter_h
#define _outputshifter_h


void AddShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name);
void ClearShifters();
void OnInitShiftRegister();
void OnSetShiftRegisterPins();


#endif