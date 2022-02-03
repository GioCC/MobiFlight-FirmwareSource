#include <Arduino.h>
#include "MFBoards.h"
#include "commandmessenger.h"

//uint8_t pinsRegistered[MODULE_MAX_PINS + 1];

// NOTE: in foresight of eleminating pin assignment checks
// (should be enforced by the Connector, allowing to spare some space in the firmware)
// all functions are neutralized i.e. they do nothing or return a constant value.
// This way, they can remain in the code (until the final deliberation)
// but they should be optimized away by the compiler 

// ************************************************************
// register used pins
// decide if needed for future use
// ************************************************************
bool isPinRegistered(uint8_t pin)
{
  return false;
  //return pinsRegistered[pin] != kTypeNotSet;
}

// this is currently not used anyway
bool isPinRegisteredForType(uint8_t pin, uint8_t type)
{
  return false;
  //return pinsRegistered[pin] == type;
}

void registerPin(uint8_t pin, uint8_t type)
{
  //pinsRegistered[pin] = type;
}

void clearRegisteredPins(uint8_t type)
{
//   for (int i = 0; i != MODULE_MAX_PINS + 1; ++i)
//     if (pinsRegistered[i] == type)
//       pinsRegistered[i] = kTypeNotSet;
}

void clearRegisteredPins()
{
//   for (int i = 0; i != MODULE_MAX_PINS + 1; ++i)
//     pinsRegistered[i] = kTypeNotSet;
}

