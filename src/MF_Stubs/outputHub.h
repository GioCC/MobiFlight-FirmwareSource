//
// outputHub.h
//

#ifndef _OUTPUTHUB_H
#define _OUTPUTHUB_H

#include <stdint.h>

#include <Arduino.h>
#include "MFBoards.h"
#include "StowManager.h"
#include "commandmessenger.h"
#include "config.h"

#include "MFOutput.h"
#include "MFSegments.h"
#include "MFOutputShifter.h"
#include "MFServo.h"
#include "MFStepper.h"
#include "MFLCDDisplay.h"

extern StowManager  Stowage;
extern CmdMessenger cmdMessenger;


// Outputs
void AddOutput(uint8_t pin);
void OnSetPin(void);

// LED Segments
#if MF_SEGMENT_SUPPORT == 1
void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness);
void OnInitSegments(void);
void OnSetSegments(void);
void OnSetModuleBrightness(void);
#endif

// Output Shift Registers
#if MF_OUTPUT_SHIFTER_SUPPORT == 1
void AddShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name);
void OnInitShiftRegister(void);
void OnSetShiftRegisterPins(void);
#endif

// Servos
#if MF_SERVO_SUPPORT == 1
void AddServo(int pin);
void OnSetServo(void);
#endif

// Steppers
#if MF_STEPPER_SUPPORT == 1
void AddStepper(int pin1, int pin2, int pin3, int pin4, int zeroPin);
void OnSetStepper(void);
void OnResetStepper(void);
void OnSetZeroStepper(void);
#endif

// LCDs
#if MF_LCD_SUPPORT == 1
#endif



// Device add functions

#endif // _OUTPUTHUB_H

