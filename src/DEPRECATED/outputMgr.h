
#ifndef _OUTPUTMGR_H
#define _OUTPUTMGR_H

#include "MFBoards.h"
#include <stdint.h>


// void AddOutput(uint8_t pin, char const *name);
// void ClearOutputs();
// void OnSetPin();
// void PowerSaveOutputs(bool state);

// void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness);
// void ClearLedSegments();
// void PowerSaveLedSegment(bool state);
// void OnInitModule();
// void OnSetModule();
// void OnSetModuleBrightness();

void AddServo(int pin);
void ClearServos();
void OnSetServo();
void updateServos();

void AddStepper(int pin1, int pin2, int pin3, int pin4, int btnPin1);
void ClearSteppers();
void OnSetStepper();
void OnResetStepper();
void OnSetZeroStepper();
void updateSteppers();


void AddLcdDisplay(uint8_t address, uint8_t cols, uint8_t lines, char const *name);
void ClearLcdDisplays();
void OnSetLcdDisplayI2C();

void AddShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name);
void ClearShifters();
void OnInitShiftRegister();
void OnSetShiftRegisterPins();


#endif