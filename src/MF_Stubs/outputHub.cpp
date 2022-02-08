//
// outputHub.cpp
//

#include <new>

#include <Arduino.h>
#include "commandmessenger.h"
#include "config.h"
#include "stowManager.h"

#include "MFIOdevice.h"
#include "MFOutput.h"
#include "MFOutputShifter.h"
#include "MFSegments.h"
#include "MFStepper.h"
#include "MFServo.h"
#include "outputHub.h"

// =============================================
//  General functions
// =============================================

// ---------------------------------------------------
//  Pin Output
// ---------------------------------------------------
void AddOutput(uint8_t pin)
{
    MFOutput *MFO;

    Stowage.AddItem(&MFO);

    if(MFO) {
        MFO->setup(pin);

        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added Output"));
    } else {
        cmdMessenger.sendCmd(kStatus, F("MFoutput: Memory full"));
        #endif
    }
}

void OnSetPin()
{
    MFOutput *MFO;
    int nOutput = cmdMessenger.readInt16Arg();
    MFO = (MFOutput *)(Stowage.getNth((uint8_t)nOutput, kTypeLedSegment));

    if(MFO) {
        int state = cmdMessenger.readInt16Arg();      // interpret string as boolean
        MFO->setval(state);       //analogWrite(pin, state);
        setLastCommandMillis(millis());
    }
}

// ---------------------------------------------------
//  LEDSegment
// ---------------------------------------------------

#if MF_SEGMENT_SUPPORT == 1
void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness)
{
    MFSegments *MFS;

    Stowage.AddItem(&MFS);

    if(MFS) {
        MFS->setup(dataPin, csPin, clkPin, numDevices, brightness);
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added LEDSegment"));
    } else {
        cmdMessenger.sendCmd(kStatus, F("LEDSegment: Memory full"));
        #endif
    }
}

void OnInitSegments(void)
{
    OnSetModuleBrightness();
}

void OnSetSegments(void)
{
    MFSegments *MFS;
    int module = cmdMessenger.readInt16Arg();
    //MFS = static_cast<MFSegments *>(Stowage.getNth(module, kTypeLedSegment));
    MFS = (MFSegments *)(Stowage.getNth((uint8_t)module, kTypeLedSegment));
    if(MFS) {
        int subModule   = cmdMessenger.readInt16Arg();
        char *value     = cmdMessenger.readStringArg();
        uint8_t points  = (uint8_t)cmdMessenger.readInt16Arg();
        uint8_t mask    = (uint8_t)cmdMessenger.readInt16Arg();
        MFS->setval(subModule, value, points, mask);
        setLastCommandMillis(millis());
    }
}

void OnSetModuleBrightness(void)
{
    MFSegments *MFS;
    int module = cmdMessenger.readInt16Arg();
    MFS = (MFSegments *)(Stowage.getNth((uint8_t)module, kTypeLedSegment));
    if(MFS) {
        int subModule   = cmdMessenger.readInt16Arg();
        int brightness  = cmdMessenger.readInt16Arg();
        MFS->setBrightness(subModule, brightness);
        setLastCommandMillis(millis());
    }
}
#endif

// ---------------------------------------------------
//  Output ShiftReg
// ---------------------------------------------------

#if MF_OUTPUT_SHIFTER_SUPPORT == 1

void AddShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name)
{
    MFOutputShifter *MFS;

    Stowage.AddItem(&MFS);

    if(MFS) {
        MFS->setup(latchPin, clockPin, dataPin, modules);
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added OutShiftReg"));
    } else {
        cmdMessenger.sendCmd(kStatus, F("OutShiftReg: Memory full"));
        #endif
    }
}

void OnInitShiftRegister(void)
{
    MFOutputShifter *MFS;
    uint8_t module = cmdMessenger.readInt16Arg();
    MFS = (MFOutputShifter *)(Stowage.getNth((uint8_t)module, kTypeOutShiftReg));
    if(MFS) {
        MFS->clear();
        setLastCommandMillis(millis());
    }
}

void OnSetShiftRegisterPins(void)
{
    MFOutputShifter *MFS;
    int module = cmdMessenger.readInt16Arg();
    MFS = (MFOutputShifter *)(Stowage.getNth((uint8_t)module, kTypeOutShiftReg));
    if(MFS) {
        char *pins  = cmdMessenger.readStringArg();
        int  value  = cmdMessenger.readInt16Arg();
        MFS->setPins(pins, value);
        setLastCommandMillis(millis());
    }
}

#endif

// ---------------------------------------------------
//  Servo
// ---------------------------------------------------

#if MF_SERVO_SUPPORT == 1

void UpdateServos(void)
{
    //TODO
}

#endif

// ---------------------------------------------------
//  Stepper
// ---------------------------------------------------

#if MF_STEPPER_SUPPORT == 1

void UpdateSteppers(void)
{
    //TODO
}

#endif

// ---------------------------------------------------
//  LCD
// ---------------------------------------------------

#if MF_LCD_SUPPORT == 1
#endif

// outputHub.cpp
