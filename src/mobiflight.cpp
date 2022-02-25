/**
 * Includes Core Arduino functionality
 **/
char foo;
#include <Arduino.h>
#include <MFBoards.h>
#include "allocateMem.h"
#include "commandmessenger.h"
#include "config.h"
#include "mobiflight.h"

// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION STR_VALUE(BUILD_VERSION)

//#define DEBUG2MSG 1

#include "MFEEPROM.h"

#define MF_BUTTON_DEBOUNCE_MS 10     // time between button updates
#define MF_ENCODER_DEBOUNCE_MS 1     // time between encoder updates
#define MF_INSHIFTER_POLL_MS 10      // time between input shift reg updates
#define MF_INMUX_POLL_MS 10          // time between dig input mux updates
#define MF_ANALOGREAD_DELAY_MS 50    // time between analog input updates
#define MF_ANALOGAVERAGE_DELAY_MS 10 // time between analog average updates
#define MF_SERVO_DELAY_MS 5          // Time between servo updates

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds

// ==================================================
//   Polling interval counters
// ==================================================

uint32_t lastButtonUpdate = 0;
uint32_t lastEncoderUpdate = 0;

#if MF_ANALOG_SUPPORT == 1
uint32_t lastAnalogAverage = 0;
uint32_t lastAnalogRead = 0;
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
uint32_t lastInputShifterUpdate = 0;
#endif

#if MF_DIGIN_MUX_SUPPORT == 1
uint32_t lastDigInMuxUpdate = 0;
#endif

#if MF_SERVO_SUPPORT == 1
uint32_t lastServoUpdate = 0;
#endif

void initPollIntervals(uint32_t time)
{
    // Init Time Gap between Inputs, do not read at the same loop
#if MF_DIGIN_MUX_SUPPORT == 1
    lastDigInMuxUpdate = time + 8;
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
    lastInputShifterUpdate = time + 6;
#endif
#if MF_ANALOG_SUPPORT == 1
    lastAnalogAverage = time + 4;
    lastAnalogRead = time + 4;
#endif
    lastButtonUpdate = time;
    lastEncoderUpdate = time + 2;

#if MF_SERVO_SUPPORT == 1
    lastServoUpdate = time;
#endif
}

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
    // Set time Gap between polls, do not read at the same loop
    initPollIntervals(millis());
}

void updatePowerSaving()
{
    if (!powerSavingMode && ((millis() - lastCommand) > (POWER_SAVING_TIME * 1000))) {
        // enable power saving
        powerSavingMode = true;
        SetPowerSavingMode(true);
    } else if (powerSavingMode && ((millis() - lastCommand) < (POWER_SAVING_TIME * 1000))) {
        // disable power saving
        powerSavingMode = false;
        SetPowerSavingMode(false);
    }
}

void timedUpdate(void (*fun)(), uint32_t *last, uint8_t intv)
{
    if (millis() - *last >= intv) {
        fun();
        *last = millis();
    }
}

// Loop function
void loop()
{
    // Process incoming serial data, and perform callbacks
    cmdMessenger.feedinSerialData();
    updatePowerSaving();

    // if config has been reset and still is not activated
    // do not perform updates to prevent mangling input
    // for config (shared buffers)
    if (!getStatusConfig())
        return;

    MUX.nextChannel();

    timedUpdate(readButtons, &lastButtonUpdate, MF_BUTTON_DEBOUNCE_MS);

    timedUpdate(readEncoder, &lastEncoderUpdate, MF_ENCODER_DEBOUNCE_MS);

#if MF_INPUT_SHIFTER_SUPPORT == 1
    timedUpdate(readInputShifters, &lastInputShifterUpdate, MF_INSHIFTER_POLL_MS);
#endif

#if MF_DIGIN_MUX_SUPPORT == 1
    timedUpdate(readDigInMux, &lastDigInMuxUpdate, MF_INMUX_POLL_MS);
#endif

#if MF_ANALOG_SUPPORT == 1
    timedUpdate(readAnalog, &lastAnalogRead, MF_ANALOGREAD_DELAY_MS);
    timedUpdate(readAnalogAvg, &lastAnalogAverage, MF_ANALOGAVERAGE_DELAY_MS);
#endif

    // segments do not need update

#if MF_STEPPER_SUPPORT == 1
    updateSteppers();
#endif

#if MF_SERVO_SUPPORT == 1
    timedUpdate(updateServos, &lastServoUpdate, MF_SERVO_DELAY_MS);
#endif
}

// Mobiflight.cpp
