#include <Arduino.h>
#include "MFBoards.h"
#include "config.h"
#include "inputHub.h"
#include "outputHub.h"
#include "commandmessenger.h"
#include "mobiflight.h"


CmdMessenger cmdMessenger = CmdMessenger(Serial);
unsigned long lastCommand;

// Prototypes for forward declarations
static void OnTrigger();
static void OnUnknownCommand();
static void OnResetBoard();


// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
  cmdMessenger.attach(OnUnknownCommand);    // Attach all callback methods

  cmdMessenger.attach(kGetInfo, OnGetInfo);
  cmdMessenger.attach(kGetConfig, OnGetConfig);
  cmdMessenger.attach(kSetConfig, OnSetConfig);
  cmdMessenger.attach(kResetConfig, OnResetConfig);
  cmdMessenger.attach(kSaveConfig, OnSaveConfig);
  cmdMessenger.attach(kActivateConfig, OnActivateConfig);
  cmdMessenger.attach(kSetName, OnSetName);
  cmdMessenger.attach(kGenNewSerial, OnGenNewSerial);
  cmdMessenger.attach(kTrigger, OnTrigger);
  cmdMessenger.attach(kResetBoard, OnResetBoard);

  cmdMessenger.attach(kSetPin, OnSetPin);

#if MF_SEGMENT_SUPPORT == 1
  cmdMessenger.attach(kInitModule, OnInitSegments);
  cmdMessenger.attach(kSetModule, OnSetSegments);
  cmdMessenger.attach(kSetModuleBrightness, OnSetModuleBrightness);
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
  cmdMessenger.attach(kSetShiftRegisterPins, OnSetShiftRegisterPins);
#endif

#if MF_SERVO_SUPPORT == 1
  cmdMessenger.attach(kSetServo, OnSetServo);
#endif

#if MF_STEPPER_SUPPORT == 1
  cmdMessenger.attach(kSetStepper, OnSetStepper);
  cmdMessenger.attach(kResetStepper, OnResetStepper);
  cmdMessenger.attach(kSetZeroStepper, OnSetZeroStepper);
#endif

#if MF_LCD_SUPPORT == 1
  cmdMessenger.attach(kSetLcdDisplayI2C, OnSetLcdDisplayI2C);
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
    resetDevices();
}

void OnResetBoard()
{
    resetBoard();
}
