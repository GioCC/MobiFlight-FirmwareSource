#include <Arduino.h>
#include "MFBoards.h"
#include "commandmessenger.h"
#include "MFOutputShifter.h"

#if MF_OUTPUT_SHIFTER_SUPPORT == 1

MFOutputShifter shiftregisters[MAX_OUTPUT_SHIFTERS];
uint8_t shiftregisterRegistered = 0;


void AddShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name)
{
  if (shiftregisterRegistered == MAX_OUTPUT_SHIFTERS)
    return;
  shiftregisters[shiftregisterRegistered].attach(latchPin, clockPin, dataPin, modules);
  shiftregisters[shiftregisterRegistered].clear();
  shiftregisterRegistered++;

#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added Shifter"));
#endif
}

void ClearShifters()
{
  for (int i = 0; i != shiftregisterRegistered; i++)
  {
    shiftregisters[shiftregisterRegistered].detach();
  }

  shiftregisterRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared Shifter"));
#endif
}

void OnInitShiftRegister()
{
  int module = cmdMessenger.readInt16Arg();
  shiftregisters[module].clear();
  setLastCommandMillis(millis());
}

void OnSetShiftRegisterPins()
{
  int module = cmdMessenger.readInt16Arg();
  char *pins = cmdMessenger.readStringArg();
  int value = cmdMessenger.readInt16Arg();
  shiftregisters[module].setPins(pins, value);
  setLastCommandMillis(millis());
}

#endif