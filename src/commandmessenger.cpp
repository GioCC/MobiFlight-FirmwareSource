#include <Arduino.h>
#include "MFBoards.h"
#include "config.h"
#include "inputHub.h"
#include "outputHub.h"
#include "commandmessenger.h"
#include "mobiflight.h"

#include "output.h"
#if MF_SEGMENT_SUPPORT == 1
#include "segment.h"
#endif
#if MF_STEPPER_SUPPORT == 1
#include "stepper.h"
#endif
#if MF_SERVO_SUPPORT == 1
#include "servos.h"
#endif
#if MF_LCD_SUPPORT == 1
#include "lcddisplay.h"
#endif
#if MF_OUTPUT_SHIFTER_SUPPORT == 1
#include "outputshifter.h"
#endif


CmdMessenger cmdMessenger = CmdMessenger(Serial);
unsigned long lastCommand;

// Prototypes for forward declarations
void OnTrigger();
void OnUnknownCommand();


// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  cmdMessenger.attach(OnUnknownCommand);    // Attach all callback methods

#if MF_SEGMENT_SUPPORT == 1
  cmdMessenger.attach(kInitModule, OnInitModule);
  cmdMessenger.attach(kSetModule, OnSetModule);
  cmdMessenger.attach(kSetModuleBrightness, OnSetModuleBrightness);
#endif

  cmdMessenger.attach(kSetPin, OnSetPin);

#if MF_STEPPER_SUPPORT == 1
  cmdMessenger.attach(kSetStepper, OnSetStepper);
#endif

#if MF_SERVO_SUPPORT == 1
  cmdMessenger.attach(kSetServo, OnSetServo);
#endif

  cmdMessenger.attach(kGetInfo, OnGetInfo);
  cmdMessenger.attach(kGetConfig, OnGetConfig);
  cmdMessenger.attach(kSetConfig, OnSetConfig);
  cmdMessenger.attach(kResetConfig, OnResetConfig);
  cmdMessenger.attach(kSaveConfig, OnSaveConfig);
  cmdMessenger.attach(kActivateConfig, OnActivateConfig);
  cmdMessenger.attach(kSetName, OnSetName);
  cmdMessenger.attach(kGenNewSerial, OnGenNewSerial);

#if MF_STEPPER_SUPPORT == 1
  cmdMessenger.attach(kResetStepper, OnResetStepper);
  cmdMessenger.attach(kSetZeroStepper, OnSetZeroStepper);
#endif

  cmdMessenger.attach(kTrigger, OnTrigger);
  cmdMessenger.attach(kResetBoard, OnResetBoard);

#if MF_LCD_SUPPORT == 1
  cmdMessenger.attach(kSetLcdDisplayI2C, OnSetLcdDisplayI2C);
#endif

#if MF_SHIFTER_SUPPORT == 1
  cmdMessenger.attach(kSetShiftRegisterPins, OnSetShiftRegisterPins);
#endif

#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Attached callbacks"));
#endif
}

uint32_t getLastCommandMillis() {
  return lastCommand;
}

void setLastCommandMillis(uint32_t time) {
  lastCommand = time;
}

// Called when a received command has no attached function
void OnUnknownCommand()
{
  lastCommand = millis();
  cmdMessenger.sendCmd(kStatus, F("n/a"));
}

void OnTrigger()
{
    //   Button::OnTrigger();
    // #if MF_INPUT_SHIFTER_SUPPORT == 1
    //   InputShifter::OnTrigger();
    // #endif
    RetriggerAll();
}