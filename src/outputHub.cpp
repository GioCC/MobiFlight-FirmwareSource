//
// outputHub.cpp
//
#include <new>

#include <Arduino.h>
#include "outputHub.h"

#define MF_SERVO_DELAY_MS 		   5 // Time between servo updates

// =============================================
//  Variables
// =============================================

#if MF_SERVO_SUPPORT == 1
uint32_t lastServoUpdate = 0;
#endif

MFOutput *outputs[MAX_OUTPUTS];
uint8_t outputsRegistered = 0;

#if MF_SEGMENT_SUPPORT == 1
MFSegments ledSegments[MAX_LEDSEGMENTS];
uint8_t ledSegmentsRegistered = 0;
#endif

#if MF_STEPPER_SUPPORT == 1
MFStepper steppers[MAX_STEPPERS]; //
uint8_t steppersRegistered = 0;
#endif

#if MF_SERVO_SUPPORT == 1
MFServo servos[MAX_MFSERVOS];
uint8_t servosRegistered = 0;
#endif

#if MF_LCD_SUPPORT == 1
MFLCDDisplay lcd_I2C[MAX_MFLCD_I2C];
uint8_t lcd_12cRegistered = 0;
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
MFOutputShifter *outputShifters[MAX_OUTPUT_SHIFTERS];
uint8_t outputShifterRegistered = 0;
#endif


// =============================================
//  General functions
// =============================================

void initOutputTimings(uint32_t time)
{
#if MF_SERVO_SUPPORT == 1
  lastServoUpdate   = time;
#endif

}

void SetPowerSavingMode(bool state)
{
#if MF_SEGMENT_SUPPORT == 1
  PowerSaveLedSegment(state);
#endif

#ifdef DEBUG
  if (state)
    cmdMessenger.sendCmd(kStatus, F("On"));
  else
    cmdMessenger.sendCmd(kStatus, F("Off"));
#endif
  //PowerSaveOutputs(state);
}



// ---------------------------------------------------
//  Pin Output
// ---------------------------------------------------
void AddOutput(uint8_t pin)
{
  if (outputsRegistered == MAX_OUTPUTS)
    return;
  if (!FitInMemory(sizeof(MFOutput)))
  {
    // Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("Output does not fit in Memory"));
    return;
  }
  outputs[outputsRegistered] = new (allocateMemory(sizeof(MFOutput))) MFOutput(pin);
  outputsRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added output"));
#endif
}

void ClearOutputs()
{
  outputsRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared outputs"));
#endif
}

void OnSetPin()
{
  // Read led state argument, interpret string as boolean
  int pin = cmdMessenger.readInt16Arg();
  int state = cmdMessenger.readInt16Arg();
  // Set led
  analogWrite(pin, state);        // why does the UI sends the pin number and not the x.th output number like other devices?
//  outputs[pin]->set(state);
  lastCommand = millis();
}

// ---------------------------------------------------
//  Output ShiftReg
// ---------------------------------------------------

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
void AddOutputShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules)
{
  if (outputShifterRegistered == MAX_OUTPUT_SHIFTERS)
    return;
  if (!FitInMemory(sizeof(MFOutputShifter)))
  {
    // Error Message to Connector
    cmdMessenger.sendCmd(kStatus, F("OutputShifter does not fit in Memory"));
    return;
  }
  outputShifters[outputShifterRegistered] = new (allocateMemory(sizeof(MFOutputShifter))) MFOutputShifter;
  outputShifters[outputShifterRegistered]->attach(latchPin, clockPin, dataPin, modules);
  outputShifters[outputShifterRegistered]->clear();
  outputShifterRegistered++;

#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added Output Shifter"));
#endif
}

void ClearOutputShifters()
{
  for (int i = 0; i != outputShifterRegistered; i++)
  {
    outputShifters[i]->detach();
  }

  outputShifterRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared Output Shifter"));
#endif
}

void OnInitOutputShifter()
{
  int module = cmdMessenger.readInt16Arg();
  outputShifters[module]->clear();
  lastCommand = millis();
}

void OnSetOutputShifterPins()
{
  int module = cmdMessenger.readInt16Arg();
  char *pins = cmdMessenger.readStringArg();
  int value = cmdMessenger.readInt16Arg();
  outputShifters[module]->setPins(pins, value);
  lastCommand = millis();
}
#endif

// ---------------------------------------------------
//  LEDSegment
// ---------------------------------------------------

#if MF_SEGMENT_SUPPORT == 1
void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness)
{
  if (ledSegmentsRegistered == MAX_LEDSEGMENTS)
    return;

  ledSegments[ledSegmentsRegistered].attach(dataPin, csPin, clkPin, numDevices, brightness); // lc is our object
  ledSegmentsRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added Led Segment"));
#endif
}

void ClearLedSegments()
{
  for (int i = 0; i != ledSegmentsRegistered; i++)
  {
    ledSegments[i].detach();
  }
  ledSegmentsRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared segments"));
#endif
}

void PowerSaveLedSegment(bool state)
{
  for (int i = 0; i != ledSegmentsRegistered; ++i)
  {
    ledSegments[i].powerSavingMode(state);
  }

  for (int i = 0; i != outputsRegistered; ++i)
  {
    outputs[i]->powerSavingMode(state);
  }
}

void OnInitModule()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg();
  ledSegments[module].setBrightness(subModule, brightness);
  lastCommand = millis();
}

void OnSetModule()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  char *value = cmdMessenger.readStringArg();
  uint8_t points = (uint8_t)cmdMessenger.readInt16Arg();
  uint8_t mask = (uint8_t)cmdMessenger.readInt16Arg();
  ledSegments[module].display(subModule, value, points, mask);
  lastCommand = millis();
}

void OnSetModuleBrightness()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg();
  ledSegments[module].setBrightness(subModule, brightness);
  lastCommand = millis();
}
#endif

// ---------------------------------------------------
//  Servo
// ---------------------------------------------------

#if MF_SERVO_SUPPORT == 1
void AddServo(int pin)
{
  if (servosRegistered == MAX_MFSERVOS)
    return;
  servos[servosRegistered].attach(pin, true);
  servosRegistered++;
}

void ClearServos()
{
  for (int i = 0; i != servosRegistered; i++)
  {
    servos[i].detach();
  }
  servosRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared servos"));
#endif
}

void OnSetServo()
{
  int servo = cmdMessenger.readInt16Arg();
  int newValue = cmdMessenger.readInt16Arg();
  if (servo >= servosRegistered)
    return;
  servos[servo].moveTo(newValue);
  lastCommand = millis();
}

void updateServos()
{
  if (millis() - lastServoUpdate <= MF_SERVO_DELAY_MS)
    return;
  lastServoUpdate = millis();

  for (int i = 0; i != servosRegistered; i++)
  {
    servos[i].update();
  }
}
#endif

// ---------------------------------------------------
//  Stepper
// ---------------------------------------------------

#if MF_STEPPER_SUPPORT == 1
void AddStepper(int pin1, int pin2, int pin3, int pin4, int btnPin1)
{
  if (steppersRegistered == MAX_STEPPERS)
    return;
  steppers[steppersRegistered].attach(pin1, pin2, pin3, pin4, btnPin1);
  steppers[steppersRegistered].setMaxSpeed(STEPPER_SPEED);
  steppers[steppersRegistered].setAcceleration(STEPPER_ACCEL);
  // autoreset is not released yet
  if (btnPin1 > 0)
  {
    // this triggers the auto reset if we need to reset
    steppers[steppersRegistered].reset();
  }

  // all set
  steppersRegistered++;

#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added stepper"));
#endif
}

void ClearSteppers()
{
  for (int i = 0; i != steppersRegistered; i++)
  {
    steppers[i].detach();
  }
  steppersRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared steppers"));
#endif
}

void OnSetStepper()
{
  int stepper = cmdMessenger.readInt16Arg();
  long newPos = cmdMessenger.readInt32Arg();

  if (stepper >= steppersRegistered)
    return;
  steppers[stepper].moveTo(newPos);
  lastCommand = millis();
}

void OnResetStepper()
{
  int stepper = cmdMessenger.readInt16Arg();

  if (stepper >= steppersRegistered)
    return;
  steppers[stepper].reset();
  lastCommand = millis();
}

void OnSetZeroStepper()
{
  int stepper = cmdMessenger.readInt16Arg();

  if (stepper >= steppersRegistered)
    return;
  steppers[stepper].setZero();
  lastCommand = millis();
}

void updateSteppers()
{
  for (int i = 0; i != steppersRegistered; i++)
  {
    steppers[i].update();
  }
}
#endif

// ---------------------------------------------------
//  LCD
// ---------------------------------------------------

#if MF_LCD_SUPPORT == 1
void AddLcdDisplay(uint8_t address, uint8_t cols, uint8_t lines)
{
  if (lcd_12cRegistered == MAX_MFLCD_I2C)
    return;
  lcd_I2C[lcd_12cRegistered].attach(address, cols, lines);
  lcd_12cRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
}

void ClearLcdDisplays()
{
  for (int i = 0; i != lcd_12cRegistered; i++)
  {
    lcd_I2C[i].detach();
  }
  lcd_12cRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared lcdDisplays"));
#endif
}

void OnSetLcdDisplayI2C()
{
  int address = cmdMessenger.readInt16Arg();
  char *output = cmdMessenger.readStringArg();
  lcd_I2C[address].display(output);
  lastCommand = millis();
}
#endif

// outputHub.cpp
