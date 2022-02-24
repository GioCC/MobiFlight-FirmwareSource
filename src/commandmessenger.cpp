#include "MFBoards.h"
#include "mobiflight.h"
#include <Arduino.h>

CmdMessenger cmdMessenger = CmdMessenger(Serial);
unsigned long lastCommand;

// Prototypes for forward declarations
static void OnUnknownCommand();
// static void OnResetBoard();

// Callbacks define on which received commands we take action
void attachCommandCallbacks()
{
    // Attach callback methods
    cmdMessenger.attach(OnUnknownCommand);

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
    cmdMessenger.attach(kSetConfig, OnSetConfig);           // 3rd step, uploading new config block wise
    cmdMessenger.attach(kResetConfig, OnResetConfig);       // 2nd step for uploading a new config
    cmdMessenger.attach(kSaveConfig, OnSaveConfig);         // 4th step, not really required anymore, config is stored directly to EEPROM, changes in UI required as feedback is required for now
    cmdMessenger.attach(kActivateConfig, OnActivateConfig); // 5th step, reading config and activate
    cmdMessenger.attach(kSetName, OnSetName);               // 1st step, write name
    cmdMessenger.attach(kGenNewSerial, OnGenNewSerial);

#if MF_STEPPER_SUPPORT == 1
    cmdMessenger.attach(kResetStepper, OnResetStepper);
    cmdMessenger.attach(kSetZeroStepper, OnSetZeroStepper);
#endif

    cmdMessenger.attach(kTrigger, OnTrigger);
    //  cmdMessenger.attach(kResetBoard, OnResetBoard);               // why is this command coming from the UI additional to OnActivateConfig?

#if MF_LCD_SUPPORT == 1
    cmdMessenger.attach(kSetLcdDisplayI2C, OnSetLcdDisplayI2C);
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT
    cmdMessenger.attach(kSetShiftRegisterPins, OnSetOutputShifterPins);
#endif

#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Attached callbacks"));
#endif
}

// uint32_t getLastCommandMillis() {
//     return lastCommand;
// }

// void setLastCommandMillis(uint32_t time) {
//     lastCommand = time;
// }

// Called when a received command has no attached function
void OnUnknownCommand()
{
    lastCommand = millis();
    cmdMessenger.sendCmd(kStatus, F("n/a"));
}

// void OnResetBoard()
// {
//   resetConfig();        // was part of loadConfig(), but not needed on initial start up
//   resetBoard();
// }
