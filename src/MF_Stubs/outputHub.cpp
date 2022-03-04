//
// outputHub.cpp
//
// #include <new>

// #include <Arduino.h>
// #include "commandmessenger.h"
// #include "config.h"
// #include "stowManager.h"

// #include "outputHub.h"
// Included by outputHub.h:
// #include "MFIOdevice.h"
// #include "MFOutput.h"
// #include "MFOutputShifter.h"
// #include "MFSegments.h"
// #include "MFStepper.h"
// #include "MFServo.h"


// =============================================
//  General functions
// =============================================

// ---------------------------------------------------
//  Pin Output
// ---------------------------------------------------

// void AddOutput(uint8_t pin)
// {
//     MFOutput *MFO;
// 
//     Stowage.AddItem(&MFO);
// 
//     if(MFO) {
//         MFO->attach(pin);
//         #ifdef DEBUG
//         cmdMessenger.sendCmd(kStatus, F("Added Output"));
//     } else {
//         cmdMessenger.sendCmd(kStatus, F("MFoutput: Memory full"));
//         #endif
//     }
// }
// 
// void OnSetPin()
// {
//     // MFOutput *MFO;
//     int nOutput = cmdMessenger.readInt16Arg();
// 
//     // MFO = (MFOutput *)(Stowage.getNth((uint8_t)nOutput, kTypeOutput));
//    
//     // CURRENTLY INCORRECT:
//     // Here, "Pin" is the actual output pin, NOT the object index!!!
//     // Patch:
//     MFOutput MFOut;
//     MFOut.attach(nOutput);
// 
//     // if(MFO) {
//         int state = cmdMessenger.readInt16Arg();      // interpret string as boolean
//         // MFO->setval(state);       //analogWrite(pin, state);
//         MFOut.setval(state);
//         setLastCommandMillis(millis());
//     // }
// }

// ---------------------------------------------------
//  LEDSegment
// ---------------------------------------------------

// #if MF_SEGMENT_SUPPORT == 1
// void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness)
// {
//     MFSegments *MFS;
//     Stowage.AddItem(&MFS);
//     if(MFS) {
//         MFS->attach(dataPin, csPin, clkPin, numDevices, brightness);
//         #ifdef DEBUG
//         cmdMessenger.sendCmd(kStatus, F("Added LEDSegment"));
//     } else {
//         cmdMessenger.sendCmd(kStatus, F("LEDSegment: Memory full"));
//         #endif
//     }
// }
// 
// void OnInitSegments(void)
// {
//     OnSetModuleBrightness(); // Same function
// }
// 
// void OnSetSegments(void)
// {
//     MFSegments *MFS;
//     int module = cmdMessenger.readInt16Arg();
//     //MFS = static_cast<MFSegments *>(Stowage.getNth(module, kTypeLedSegment));
//     MFS = (MFSegments *)(Stowage.getNth((uint8_t)module, kTypeLedSegment));
//     if(MFS) {
//         int subModule   = cmdMessenger.readInt16Arg();
//         char *value     = cmdMessenger.readStringArg();
//         uint8_t points  = (uint8_t)cmdMessenger.readInt16Arg();
//         uint8_t mask    = (uint8_t)cmdMessenger.readInt16Arg();
//         MFS->setval(subModule, value, points, mask);
//         setLastCommandMillis(millis());
//     }
// }
// 
// void OnSetModuleBrightness(void)
// {
//     MFSegments *MFS;
//     int module = cmdMessenger.readInt16Arg();
//     MFS = (MFSegments *)(Stowage.getNth((uint8_t)module, kTypeLedSegment));
//     if(MFS) {
//         int subModule   = cmdMessenger.readInt16Arg();
//         int brightness  = cmdMessenger.readInt16Arg();
//         MFS->setBrightness(subModule, brightness);
//         setLastCommandMillis(millis());
//     }
// }
// #endif

// ---------------------------------------------------
//  Output ShiftReg
// ---------------------------------------------------

// #if MF_OUTPUT_SHIFTER_SUPPORT == 1
// 
// void AddOutShiftReg(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name)
// {
//     MFOutputShifter *MFS;
// 
//     Stowage.AddItem(&MFS);
// 
//     if(MFS) {
//         MFS->attach(latchPin, clockPin, dataPin, modules);
//         #ifdef DEBUG
//         cmdMessenger.sendCmd(kStatus, F("Added OutShiftReg"));
//     } else {
//         cmdMessenger.sendCmd(kStatus, F("OutShiftReg: Memory full"));
//         #endif
//     }
// }
// 
// void OnInitShiftRegister(void)
// {
//     MFOutputShifter *MFS;
//     uint8_t module = cmdMessenger.readInt16Arg();
//     MFS = (MFOutputShifter *)(Stowage.getNth((uint8_t)module, kTypeOutShiftReg));
//     if(MFS) {
//         MFS->clear();
//         setLastCommandMillis(millis());
//     }
// }
// 
// void OnSetShiftRegisterPins(void)
// {
//     MFOutputShifter *MFS;
//     int module = cmdMessenger.readInt16Arg();
//     MFS = (MFOutputShifter *)(Stowage.getNth((uint8_t)module, kTypeOutShiftReg));
//     if(MFS) {
//         char *pins  = cmdMessenger.readStringArg();
//         int  value  = cmdMessenger.readInt16Arg();
//         MFS->setPins(pins, value);
//         setLastCommandMillis(millis());
//     }
// }
// 
// #endif

// ---------------------------------------------------
//  Servo
// ---------------------------------------------------

// #if MF_SERVO_SUPPORT == 1
// 
// void AddServo(int pin)
// {
//     MFServo *MFS;
// 
//     Stowage.AddItem(&MFS);
// 
//     if(MFS) {
//         MFS->attach(pin, true);
//         #ifdef DEBUG
//         cmdMessenger.sendCmd(kStatus, F("Added Servo"));
//     } else {
//         cmdMessenger.sendCmd(kStatus, F("Servo: Memory full"));
//         #endif
//     }
// }
// 
// void OnSetServo(void)
// {
//     MFServo *MFS;
//     int nServo = cmdMessenger.readInt16Arg();
//     MFS = (MFServo *)(Stowage.getNth((uint8_t)nServo, kTypeServo));
//     if(MFS) {
//         int  value  = cmdMessenger.readInt16Arg();
//         MFS->setval(value);
//         setLastCommandMillis(millis());
//     }
// }
// #endif

// ---------------------------------------------------
//  Stepper
// ---------------------------------------------------

// #if MF_STEPPER_SUPPORT == 1
// 
// void AddStepper(int pin1, int pin2, int pin3, int pin4, int zeroPin)
// {
//     MFStepper *MFS;
//     Stowage.AddItem(&MFS);
//     if(MFS) {
//         MFS->attach(pin1, pin2, pin3, pin4, zeroPin);
//         MFS->setMaxSpeed(STEPPER_SPEED);        //TODO move to constructor?
//         MFS->setAcceleration(STEPPER_ACCEL);    //TODO move to constructor?
//         if (zeroPin > 0)                        //TODO move to constructor?
//         {
//             MFS->reset(ONRESET_DEFAULT);
//         }
//         #ifdef DEBUG
//         cmdMessenger.sendCmd(kStatus, F("Added Stepper"));
//     } else {
//         cmdMessenger.sendCmd(kStatus, F("Stepper: Memory full"));
//         #endif
//     }
// }
// 
// void _OnStepperEvent(uint8_t evt) 
// {
//     MFStepper *MFS;
//     int nStepper = cmdMessenger.readInt16Arg();
//     MFS = (MFStepper *)(Stowage.getNth((uint8_t)nStepper, kTypeStepper));
//     if(MFS) {
//         if(1 == evt) {
//             int  value  = cmdMessenger.readInt32Arg();
//             MFS->setval(value);
//         } else
//         if(2 == evt) {
//             MFS->reset(ONRESET_DEFAULT);
//         } else
//         if(3 == evt) {
//             MFS->setZero();
//         } else
//         {
//             evt = 0;
//         }
//         if(evt) setLastCommandMillis(millis());
//     }
// }
// 
// void OnSetStepper(void)
// {
//     _OnStepperEvent(1);
// }
// 
// void OnResetStepper(void)
// {
//     _OnStepperEvent(2);
// }
// 
// void OnSetZeroStepper(void)
// {
//     _OnStepperEvent(3);
// }
// #endif

// ---------------------------------------------------
//  LCD
// ---------------------------------------------------

// #if MF_LCD_SUPPORT == 1
// void AddLcdDisplay(uint8_t I2Caddress, uint8_t cols, uint8_t lines, char const *name)
// {
//     MFLCDDisplay *MFL;
//     Stowage.AddItem(&MFL);
//     // lcd_I2C[lcd_12cRegistered].attach(address, cols, lines);
//     // lcd_12cRegistered++;
//     if(MFL) {
//         MFL->attach(I2Caddress, cols, lines);
//         #ifdef DEBUG
//         cmdMessenger.sendCmd(kStatus, F("Added LCD display"));
//     } else {
//         cmdMessenger.sendCmd(kStatus, F("LCD display: Memory full"));
//         #endif
//     }
// }
// 
// void OnSetLcdDisplayI2C(void) 
// {
//     MFLCDDisplay *MFL;
//     int nLCD = cmdMessenger.readInt16Arg();
//     //MFS = static_cast<MFSegments *>(Stowage.getNth(module, kTypeLedSegment));
//     MFL = (MFLCDDisplay *)(Stowage.getNth((uint8_t)nLCD, kTypeLcdDisplayI2C));
//     if(MFL) {
//         char *output    = cmdMessenger.readStringArg();
//         MFL->setval(output);
//         setLastCommandMillis(millis());
//     }
// }
// 
// #endif

// outputHub.cpp
