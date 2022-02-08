// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION STR_VALUE(BUILD_VERSION)

//#define DEBUG 1


#include <Arduino.h>
#include "mobiflight.h"
//#include "allocateMem.h"
#include "MFEEPROM.h"
#include "commandmessenger.h"
#include "MFBoards.h"
#include "config.h"
#include "inputHub.h"
#include "outputHub.h"

// #include "output.h"
// #if MF_SEGMENT_SUPPORT == 1
// #include "segment.h"
// #endif
// #if MF_STEPPER_SUPPORT == 1
// #include "stepper.h"
// #endif
// #if MF_SERVO_SUPPORT == 1
// #include "servos.h"
// #endif


#define MF_BUTTON_DEBOUNCE_MS         10    // time between updating the buttons
#define MF_SERVO_DELAY_MS             5     // time between servo updates
#define MF_ANALOGAVERAGE_DELAY_MS     10    // time between updating the analog average calculation
#define MF_ANALOGREAD_DELAY_MS        50    // time between sending analog values
#define MF_ENCODER_DEBOUNCE_MS        1     // time between updating encoders

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds

uint32_t lastButtonUpdate= 0;
uint32_t lastEncoderUpdate = 0;
#if MF_SERVO_SUPPORT == 1
uint32_t lastServoUpdate = 0;
#endif
#if MF_ANALOG_SUPPORT == 1
uint32_t lastAnalogAverage = 0;
uint32_t lastAnalogRead = 0;
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
uint32_t lastInputShifterUpdate = 0;
#endif

// ************************************************************
//  General I/O handling functions
// ************************************************************

void UpdateAll(void)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
        in->update();
    }
}

void UpdateAll(uint8_t type)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext(type))) != NULL) {
        in->update();
    }
}

void RetriggerAll(void)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
        in->onReset();
    }
}

void SetPowerSave(uint8_t mode)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
        in->powerSave(mode);
    }
}


// ************************************************************
// Power save management
// ************************************************************
void SetPowerSavingMode(bool state)
{
  // disable the lights ;)
  powerSavingMode = state;

  SetPowerSave(state);

#ifdef DEBUG
  if (state)
    cmdMessenger.sendCmd(kStatus, F("On"));
  else
    cmdMessenger.sendCmd(kStatus, F("Off"));
#endif
}

void updatePowerSaving()
{
  if (!powerSavingMode && ((millis() - getLastCommandMillis()) > (POWER_SAVING_TIME * 1000)))
  {
    SetPowerSavingMode(true);   // enable power saving
  }
  else if (powerSavingMode && ((millis() - getLastCommandMillis()) < (POWER_SAVING_TIME * 1000)))
  {
    SetPowerSavingMode(false);  // disable power saving
  }
}

// ************************************************************
// Reset Board
// ************************************************************

/*
void OnResetBoard()
{
  resetConfig();        // was part of loadConfig(), but not needed on initial start up
  ResetBoard();
}
*/

// ************************************************************
// Setup
// ************************************************************
void setup()
{
  Serial.begin(115200);
  attachCommandCallbacks();
  cmdMessenger.printLfCr();
  ResetBoard();

  // Time Gap between Inputs, do not read at the same loop
#if MF_SERVO_SUPPORT == 1
  lastServoUpdate = millis();
#endif

  lastButtonUpdate = millis();
  lastEncoderUpdate = millis();           // encoders will be updated every 1ms

#if MF_ANALOG_SUPPORT == 1
  lastAnalogAverage = millis() + 4;
  lastAnalogRead = millis() + 4;
#endif

#if MF_INPUTSHIFTER_SUPPORT == 1
  lastInputShifterUpdate = millis() + 6;
#endif
#if MF_SERVO_SUPPORT == 1
  lastServoUpdate = millis() + 8;

#endif
}

// ************************************************************
// Loop function
// ************************************************************
void loop()
{
  // Process incoming serial data, and perform callbacks
  cmdMessenger.feedinSerialData();
  updatePowerSaving();

  // if config has been reset and still is not activated
  // do not perform updates
  // to prevent mangling input for config (shared buffers)
  if (getStatusConfig())
  {
    if (millis() - lastButtonUpdate >= MF_BUTTON_DEBOUNCE_MS)
    {
      lastButtonUpdate = millis();
      //Button::read();
      UpdateAll(kTypeButton);
    }
    if (millis() - lastEncoderUpdate >= MF_ENCODER_DEBOUNCE_MS)
    {
      lastEncoderUpdate = millis();
      //Encoder::read();
      UpdateAll(kTypeEncoder);
    }
#if MF_STEPPER_SUPPORT == 1
    //updateSteppers();
    UpdateAll(kTypeServo);
#endif
#if MF_SERVO_SUPPORT == 1
    if (millis() - lastServoUpdate >= MF_SERVO_DELAY_MS)
    {
        lastServoUpdate = millis();
        //updateServos();
        UpdateAll(kTypeServo);
    }
#endif
#if MF_ANALOG_SUPPORT == 1
    if (millis() - lastAnalogRead >= MF_ANALOGREAD_DELAY_MS)
    {
      lastAnalogRead = millis();
      //Analog::read();
      UpdateAll(kTypeAnalogInput);
    }
    if (millis() - lastAnalogAverage >= MF_ANALOGAVERAGE_DELAY_MS)
    {
      lastAnalogAverage = millis();
      //Analog::readAverage();
      UpdateAnalogAvg();
    }
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
    if (millis() - lastInputShifterUpdate >= MF_ENCODER_DEBOUNCE_MS)
    {
      lastInputShifterUpdate = millis();
      //InputShifter::read();     // <update
      UpdateAll(kTypeInShiftReg);
    }
#endif
    // lcds, outputs, segments do not need update
  }
}
