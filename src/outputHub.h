//
// outputHub.h
//

#ifndef _OUTPUTHUB_H
#define _OUTPUTHUB_H

#include <stdint.h>

#include <Arduino.h>
#include "MFBoards.h"
#include "mobiflight.h"

#include "MFOutput.h"
#include "MFSegments.h"
#include "MFOutputShifter.h"
#include "MFServo.h"
#include "MFStepper.h"
#include "MFLCDDisplay.h"

// Exported variables
extern uint32_t lastServoUpdate;
extern CmdMessenger cmdMessenger;

// General functions

void SetPowerSavingMode(bool state);

// Device management functions

// Outputs
void AddOutput(uint8_t pin = 1);
void ClearOutputs();
void OnSetPin();

// Output Shift Registers
#if MF_OUTPUT_SHIFTER_SUPPORT == 1
void AddOutputShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules);
void ClearOutputShifters();
void OnInitOutputShifter();
void OnSetOutputShifterPins();
#endif

// LED Segments
#if MF_SEGMENT_SUPPORT == 1
void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness);
void ClearLedSegments();
void PowerSaveLedSegment(bool state);
void OnInitModule();
void OnSetModule();
void OnSetModuleBrightness();
#endif

// Steppers
#if MF_STEPPER_SUPPORT == 1
void AddStepper(int pin1, int pin2, int pin3, int pin4, int btnPin1);
void ClearSteppers();
void updateSteppers();
void OnSetStepper();
void OnResetStepper();
void OnSetZeroStepper();
#endif

// Servos
#if MF_SERVO_SUPPORT == 1
void AddServo(int pin);
void ClearServos();
void updateServos();
void OnSetServo();
#endif

// LCDs
#if MF_LCD_SUPPORT == 1

void AddLcdDisplay(uint8_t address = 0x24, uint8_t cols = 16, uint8_t lines = 2);
void ClearLcdDisplays();
void OnSetLcdDisplayI2C();
#endif

#endif // _OUTPUTHUB_H

