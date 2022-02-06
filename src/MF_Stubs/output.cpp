#include <Arduino.h>
#include "MFBoards.h"
#include "commandmessenger.h"
#include "registerpin.h"
#include "config.h"
#include "MFOutput.h"


MFOutput outputs[MAX_OUTPUTS];
uint8_t outputsRegistered = 0;


void AddOutput(uint8_t pin, char const *name)
{
  if (outputsRegistered == MAX_OUTPUTS)
    return;
  if (isPinRegistered(pin))
    return;

  outputs[outputsRegistered] = MFOutput(pin);
  registerPin(pin, kTypeOutput);
  outputsRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added output"));
#endif
}

void detach()
{
  clearRegisteredPins(kTypeOutput);
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
  analogWrite(pin, state);
  setLastCommandMillis(millis());
}

void PowerSaveOutputs(bool state)
{
  for (int i = 0; i != outputsRegistered; ++i)
  {
    outputs[i].powerSavingMode(state);
  }
}