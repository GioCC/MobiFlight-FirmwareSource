// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION STR_VALUE(BUILD_VERSION)

//#define DEBUG 1

#define TESTING

#include <Arduino.h>
//#include "allocateMem.h"
#include "MFEEPROM.h"
#include "commandmessenger.h"
#include "MFBoards.h"
#ifdef MF_MUX_SUPPORT
#include "MFMuxDriver.h"
#endif
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
#define MF_INSHIFT_DELAY_MS           10    // time between updating input shift registers

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

void ResetDevices(void)
{
    MFIOdevice *in;
    // Trigger all release events first for inputs, does nothing for outputs
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
        in->onReset(MFIOdevice::ONRESET_RELEASE);
    }
    // ...then trigger all the press events for inputs, and clear outputs
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
        in->onReset(MFIOdevice::ONRESET_PRESS);
    }
    Stowage.reset();
}

void UpdateDevices(void)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
        in->update();
    }
}

void UpdateDevices(uint8_t type)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext(type))) != NULL) {
        in->update();
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
// TEST FUNCTIONS
// ************************************************************

void printItemSize(void)
{
#ifdef TESTING
    
#define TST_WRITE(s, d) \
    { sprintf(buf, fmt, s, d); \
    cmdMessenger.sendCmd(kStatus, buf); \
    delay(100); }
        
    char buf[40];
    const char *fmt = "Size of each <%s>: %d bytes";
    TST_WRITE("Output", MFOutput::getSize());
    TST_WRITE("Button", MFButton::getSize());
    TST_WRITE("Encoder", MFEncoder::getSize());
    TST_WRITE("AnalogIn", MFAnalog::getSize());
    TST_WRITE("InShiftReg", MFInputShifter::getSize());
    TST_WRITE("LEDsegment", MFSegments::getSize());
    TST_WRITE("Stepper", MFStepper::getSize());
    TST_WRITE("Servo", MFServo::getSize());
    TST_WRITE("OutShiftReg", MFOutputShifter::getSize());
    TST_WRITE("LCDdisplay", MFLCDDisplay::getSize());
#endif
}

void printReport(uint8_t nItems, const char *itemName)
{
#ifdef TESTING
    char buf[40];
    sprintf(buf, "Added %d %s", nItems, itemName);
    cmdMessenger.sendCmd(kStatus, buf);
    sprintf(buf, " used %d bytes, remaining %d", 
            Stowage.getUsedSize(), 
            Stowage.getFreeSize()
            );
    cmdMessenger.sendCmd(kStatus, buf);
    delay(1000);
#endif
}

void setupData(void)
{
uint8_t i;
#ifdef TESTING
delay(1000);

printItemSize();

Stowage.wipe();
printReport(0, "-none-");

// #define MAX_OUTPUTS 40
for(i=0; i<MAX_OUTPUTS; i++) AddOutput(1);
printReport(MAX_OUTPUTS, "Outputs");

// #define MAX_BUTTONS 68
for(i=0; i<MAX_BUTTONS; i++) AddButton(1);
printReport(MAX_BUTTONS, "Buttons");

// #define MAX_ENCODERS 20
for(i=0; i<MAX_ENCODERS; i++) AddEncoder(1, 2, 1);
printReport(MAX_ENCODERS, "Encoders");

// #define MAX_ANALOG_INPUTS 16
for(i=0; i<MAX_ANALOG_INPUTS; i++) AddAnalog(1);
printReport(MAX_ANALOG_INPUTS, "AnalogIns");

// #define MAX_INPUT_SHIFTERS 4
for(i=0; i<MAX_INPUT_SHIFTERS; i++) AddInputShiftReg(1,2,3,2);
printReport(MAX_INPUT_SHIFTERS, "InputShiftRegs");

// #define MAX_LEDSEGMENTS 4
for(i=0; i<MAX_LEDSEGMENTS; i++) AddLedSegment(1,2,3,2,15);
printReport(MAX_LEDSEGMENTS, "LEDSegments");

// #define MAX_STEPPERS 10
for(i=0; i<MAX_STEPPERS; i++) AddStepper(1,2,3,4,5);
printReport(MAX_STEPPERS, "Steppers");

// #define MAX_MFSERVOS 10
for(i=0; i<MAX_MFSERVOS; i++) AddServo(1);
printReport(MAX_MFSERVOS, "Servos");

// #define MAX_OUTPUT_SHIFTERS 4
for(i=0; i<MAX_OUTPUT_SHIFTERS; i++) AddOutShiftReg(1,2,3,2);
printReport(MAX_OUTPUT_SHIFTERS, "OutShiftRegs");

// #define MAX_MFLCD_I2C 2
for(i=0; i<MAX_MFLCD_I2C; i++) AddLcdDisplay(1, 40, 2);
printReport(MAX_MFLCD_I2C, "LCD displays");

#endif
}


// ************************************************************
// Setup
// ************************************************************
void setup()
{
  Serial.begin(115200);
  attachCommandCallbacks();
  SetInputHandlers();

  cmdMessenger.printLfCr();
  resetBoard();

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

ResetDevices();

#ifdef TESTING
    while(1) {
        setupData();
        delay(60000);
    }
#endif
}

//TODO compare mem size of macro & function version:
#define checkUpdate(typ, tim, int) \
    do{\
        if (millis() - (*tim) >= (int)) { \
            *tim = millis(); \
            UpdateDevices(typ);\
        }   \
    } while(0);

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
    if (getStatusConfig()) {

        checkUpdate(kTypeButton, &lastButtonUpdate, MF_BUTTON_DEBOUNCE_MS);
        checkUpdate(kTypeEncoder, &lastEncoderUpdate, MF_ENCODER_DEBOUNCE_MS);
        #if MF_SERVO_SUPPORT == 1
        checkUpdate(kTypeServo, &lastServoUpdate, MF_SERVO_DELAY_MS);
        #endif
        #if MF_INPUT_SHIFTER_SUPPORT == 1
        checkUpdate(kTypeInShiftReg, &lastInputShifterUpdate, MF_INSHIFT_DELAY_MS);
        #endif
        #if MF_ANALOG_SUPPORT == 1
        checkUpdate(kTypeAnalogInput, &lastAnalogRead, MF_ANALOGREAD_DELAY_MS);
        if (millis() - lastAnalogAverage >= MF_ANALOGAVERAGE_DELAY_MS) {
            lastAnalogAverage = millis();
            // Analog::readAverage();
            UpdateAnalogAvg();
        }
        #endif
        #if MF_STEPPER_SUPPORT == 1
        UpdateDevices(kTypeStepper);
        #endif
        // lcds, outputs, segments do not need update
    }
}


