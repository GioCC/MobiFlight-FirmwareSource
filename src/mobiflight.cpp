/**
 * Includes Core Arduino functionality 
 **/
char foo;
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "mobiflight.h"
#include <MFBoards.h>
#include "allocateMem.h"
#include "config.h"
#include "commandmessenger.h"

// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION STR_VALUE(BUILD_VERSION)

//#define DEBUG 1

#include "MFEEPROM.h"

// #if MF_STEPPER_SUPPORT == 1
// #include <AccelStepper.h>
// #include <MFStepper.h>
// #endif

// #if MF_SERVO_SUPPORT == 1
// #include <Servo.h>
// #include <MFServo.h>
// #endif

// #if MF_LCD_SUPPORT == 1
// #include <LiquidCrystal_I2C.h>
// #include <MFLCDDisplay.h>
// #endif

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds


// Setup function
void setup()
{
  Serial.begin(115200);
#if MF_DIGIN_MUX_SUPPORT == 1
  MFDigInMux::setMux(&MUX);
#endif
  attachCommandCallbacks();
  attachEventCallbacks();
  cmdMessenger.printLfCr();
  resetBoard();
  // Time Gap between Inputs, do not read at the same loop
#if MF_DIGIN_MUX_SUPPORT == 1
  lastDigInMuxUpdate 	= millis() + 8;
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
  lastInputShifterUpdate = millis() + 6;
#endif
#if MF_ANALOG_SUPPORT == 1
  lastAnalogAverage = millis() + 4;
  lastAnalogRead    = millis() + 4;
#endif
  lastButtonUpdate  = millis();
  lastEncoderUpdate = millis() + 2;
#if MF_SERVO_SUPPORT == 1
  lastServoUpdate   = millis();
#endif
}

void updatePowerSaving()
{
  if (!powerSavingMode && ((millis() - lastCommand) > (POWER_SAVING_TIME * 1000)))
  {
    // enable power saving
    powerSavingMode = true;
    SetPowerSavingMode(true);
  }
  else if (powerSavingMode && ((millis() - lastCommand) < (POWER_SAVING_TIME * 1000)))
  {
    // disable power saving
    powerSavingMode = false;
    SetPowerSavingMode(false);
  }
}

// Loop function
void loop()
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
  updatePowerSaving();

  // if config has been reset
  // and still is not activated
  // do not perform updates
  // to prevent mangling input for config (shared buffers)
  if (!getStatusConfig())
    return;

  MUX.nextChannel();

  readButtons();
  readEncoder();

#if MF_INPUT_SHIFTER_SUPPORT == 1
  readInputShifters();
#endif

#if MF_DIGIN_MUX_SUPPORT == 1
  readDigInMux();
#endif

#if MF_ANALOG_SUPPORT == 1
  readAnalog();
#endif

  // segments do not need update
#if MF_STEPPER_SUPPORT == 1
  updateSteppers();
#endif

#if MF_SERVO_SUPPORT == 1
  updateServos();
#endif
}

// Mobiflight.cpp

