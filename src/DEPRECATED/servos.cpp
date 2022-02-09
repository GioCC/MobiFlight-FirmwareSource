#include <Arduino.h>
#include "MFBoards.h"
#include "commandmessenger.h"
#include "registerpin.h"
#include "config.h"
#include "MFServo.h"


#if MF_SERVO_SUPPORT == 1

MFServo servos[MAX_MFSERVOS];
uint8_t servosRegistered = 0;


void AddServo(int pin)
{
  if (servosRegistered == MAX_MFSERVOS)
    return;
  if (isPinRegistered(pin))
    return;

  servos[servosRegistered].attach(pin, true);
  registerPin(pin, kTypeServo);
  servosRegistered++;
}

void ClearServos()
{
  for (int i = 0; i != servosRegistered; i++)
  {
    servos[servosRegistered].detach();
  }
  clearRegisteredPins(kTypeServo);
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
  setLastCommandMillis(millis());
}

void updateServos()
{
  for (int i = 0; i != servosRegistered; i++)
  {
    servos[i].update();
  }
}


#endif