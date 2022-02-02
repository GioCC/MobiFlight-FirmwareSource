#include <Arduino.h>
#include "MFBoards.h"
#include "commandmessenger.h"

uint8_t pinsRegistered[MODULE_MAX_PINS + 1];


// ************************************************************
// register used pins
// decide if needed for future use
// ************************************************************
bool isPinRegistered(uint8_t pin)
{
  return pinsRegistered[pin] != kTypeNotSet;
}

bool isPinRegisteredForType(uint8_t pin, uint8_t type)
{
  return pinsRegistered[pin] == type;
}

void registerPin(uint8_t pin, uint8_t type)
{
  pinsRegistered[pin] = type;
}

void clearRegisteredPins(uint8_t type)
{
  for (int i = 0; i != MODULE_MAX_PINS + 1; ++i)
    if (pinsRegistered[i] == type)
      pinsRegistered[i] = kTypeNotSet;
}

void clearRegisteredPins()
{
  for (int i = 0; i != MODULE_MAX_PINS + 1; ++i)
    pinsRegistered[i] = kTypeNotSet;
}

