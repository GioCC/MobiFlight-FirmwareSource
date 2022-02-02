#include <Arduino.h>
#include "MFBoards.h"
#include "commandmessenger.h"
#include "registerpin.h"
#include "MFStepper.h"


#if MF_STEPPER_SUPPORT == 1

MFStepper *steppers[MAX_STEPPERS];
uint8_t steppersRegistered = 0;


void AddStepper(int pin1, int pin2, int pin3, int pin4, int btnPin1)
{
  if (steppersRegistered == MAX_STEPPERS)
    return;
  if (isPinRegistered(pin1) || isPinRegistered(pin2) || isPinRegistered(pin3) || isPinRegistered(pin4) || (btnPin1 > 0 && isPinRegistered(btnPin1)))
  {
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Conflict with stepper"));
#endif
    return;
  }

  steppers[steppersRegistered] = new MFStepper(pin1, pin2, pin3, pin4, btnPin1); // is our object
  steppers[steppersRegistered]->setMaxSpeed(STEPPER_SPEED);
  steppers[steppersRegistered]->setAcceleration(STEPPER_ACCEL);

  registerPin(pin1, kTypeStepper);
  registerPin(pin2, kTypeStepper);
  registerPin(pin3, kTypeStepper);
  registerPin(pin4, kTypeStepper);
  // autoreset is not released yet
  if (btnPin1 > 0)
  {
    registerPin(btnPin1, kTypeStepper);
    // this triggers the auto reset if we need to reset
    steppers[steppersRegistered]->reset();
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
    delete steppers[steppersRegistered];
  }
  clearRegisteredPins(kTypeStepper);
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
  steppers[stepper]->moveTo(newPos);
  setLastCommandMillis(millis());
}

void OnResetStepper()
{
  int stepper = cmdMessenger.readInt16Arg();

  if (stepper >= steppersRegistered)
    return;
  steppers[stepper]->reset();
  setLastCommandMillis(millis());
}

void OnSetZeroStepper()
{
  int stepper = cmdMessenger.readInt16Arg();

  if (stepper >= steppersRegistered)
    return;
  steppers[stepper]->setZero();
  setLastCommandMillis(millis());
}

void updateSteppers()
{
  for (int i = 0; i != steppersRegistered; i++)
  {
    steppers[i]->update();
  }
}


#endif

