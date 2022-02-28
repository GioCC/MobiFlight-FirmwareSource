// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION STR_VALUE(BUILD_VERSION)

//#define DEBUG 1

//#define TESTING

#include <Arduino.h>
//#include "allocateMem.h"
#include "MFBoards.h"
#include "MFEEPROM.h"
#include "commandmessenger.h"
#ifdef MF_MUX_SUPPORT
#include "MFMuxDriver.h"
#endif
#include "config.h"
#include "inputHub.h"
#include "outputHub.h"

#define MF_BUTTON_DEBOUNCE_MS 10     // time between updating the buttons
#define MF_SERVO_DELAY_MS 5          // time between servo updates
#define MF_ANALOGAVERAGE_DELAY_MS 10 // time between updating the analog average calculation
#define MF_ANALOGREAD_DELAY_MS 50    // time between sending analog values
#define MF_ENCODER_DEBOUNCE_MS 1     // time between updating encoders
#define MF_INSHIFT_DELAY_MS 10       // time between updating input shift registers

bool powerSavingMode = false;
const unsigned long POWER_SAVING_TIME = 60 * 15; // in seconds

uint32_t lastButtonUpdate = 0;
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

// INVOKE(detach, p, kTypeButton);
#define INVOKE(function, devtype)                                  \
    {                                                              \
        void *dev;                                                 \
        uint8_t type;                                              \
        Stowage.reset();                                           \
        do {                                                       \
            if (devtype == StowManager::TypeALL)                   \
                type = Stowage.getTypeOfNext();                    \
            dev = Stowage.getNext(devtype);                        \
            if (dev) {                                             \
                switch (type) {                                    \
                case kTypeButton:                                  \
                    static_cast<MFButton *>(dev)->function;        \
                    break;                                         \
                case kTypeAnalogInput:                             \
                    static_cast<MFAnalog *>(dev)->function;        \
                    break;                                         \
                case kTypeEncoder:                                 \
                case kTypeEncoderSingleDetent:                     \
                    static_cast<MFEncoder *>(dev)->function;       \
                    break;                                         \
                case kTypeInShiftReg:                              \
                    static_cast<MFInputShifter *>(dev)->function;  \
                    break;                                         \
                case kTypeDigInMux:                                \
                    static_cast<MFDigInMux *>(dev)->function;      \
                    break;                                         \
                case kTypeOutput:                                  \
                    static_cast<MFOutput *>(dev)->function;        \
                    break;                                         \
                case kTypeOutShiftReg:                             \
                    static_cast<MFOutputShifter *>(dev)->function; \
                    break;                                         \
                case kTypeLedSegment:                              \
                    static_cast<MFSegments *>(dev)->function;      \
                    break;                                         \
                case kTypeServo:                                   \
                    static_cast<MFServo *>(dev)->function;         \
                    break;                                         \
                case kTypeStepper:                                 \
                case kTypeStepperDeprecated:                       \
                    static_cast<MFStepper *>(dev)->function;       \
                    break;                                         \
                case kTypeLcdDisplayI2C:                           \
                    static_cast<MFLCDDisplay *>(dev)->function;    \
                    break;                                         \
                default:                                           \
                    break;                                         \
                }                                                  \
            }                                                      \
        } while (dev);                                             \
    }

void WipeDevices(void)
{
    // Reset device storage (this will do all devices)
    INVOKE(detach(), StowManager::TypeALL);
    Stowage.wipe();
}

void ResetDevices(void)
{
    // Trigger all release events first for inputs, does nothing for outputs
    INVOKE(reset(ONRESET_RELEASE), StowManager::TypeALL);
    // ...then trigger all the press events for inputs, and clear outputs
    INVOKE(reset(ONRESET_PRESS), StowManager::TypeALL);
}

void UpdateDevices(void)
{
    INVOKE(update(), StowManager::TypeALL);
}

void UpdateDevices(uint8_t type)
{
    INVOKE(update(), type);
}

void SetPowerSave(uint8_t mode)
{
    MFIOdevice *in;
    Stowage.reset();
    while ((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
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
    if (!powerSavingMode && ((millis() - getLastCommandMillis()) > (POWER_SAVING_TIME * 1000))) {
        SetPowerSavingMode(true); // enable power saving
    } else if (powerSavingMode && ((millis() - getLastCommandMillis()) < (POWER_SAVING_TIME * 1000))) {
        SetPowerSavingMode(false); // disable power saving
    }
}

// ************************************************************
// TEST FUNCTIONS
// ************************************************************

void printItemSize(void)
{
#ifdef TESTING

#define TST_WRITE(s, d)                     \
    {                                       \
        sprintf(buf, fmt, s, d);            \
        cmdMessenger.sendCmd(kStatus, buf); \
        delay(100);                         \
    }

    char buf[40];
    const char *fmt = "Actual size of each <%s>: %d bytes";
    // The actual size in the buffer of each device is as follows:
    // <n> bytes for the "bare" class attributes
    // + 1 bytes for buffer link pointer (increment to next element in buffer)
    // + 1 byte  for type code (allowing to cast the object to the correct type)
    // First component is included in the value returned by sizeof(),
    // other 2 component are added explicitly below
    TST_WRITE("Output", sizeof(MFOutput) + 2);
    TST_WRITE("Button", sizeof(MFButton) + 2);
    TST_WRITE("Encoder", sizeof(MFEncoder) + 2);
    TST_WRITE("AnalogIn", sizeof(MFAnalog) + 2);
    TST_WRITE("InShiftReg", sizeof(MFInputShifter) + 2);
    TST_WRITE("LEDsegment", sizeof(MFSegments) + 2);
    TST_WRITE("Stepper", sizeof(MFStepper) + 2);
    TST_WRITE("Servo", sizeof(MFServo) + 2);
    TST_WRITE("OutShiftReg", sizeof(MFOutputShifter) + 2);
    TST_WRITE("LCDdisplay", sizeof(MFLCDDisplay) + 2);
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
            Stowage.getFreeSize());
    cmdMessenger.sendCmd(kStatus, buf);
    delay(1000);
#endif
}

void setupData(void)
{
#ifdef TESTING
    uint8_t i;
    delay(1000);

    printItemSize();

    Stowage.wipe();
    printReport(0, "-none-");

    // #define MAX_OUTPUTS 40
    for (i = 0; i < MAX_OUTPUTS; i++)
        AddOutput(1);
    printReport(MAX_OUTPUTS, "Outputs");

    // #define MAX_BUTTONS 68
    for (i = 0; i < MAX_BUTTONS; i++)
        AddButton(1);
    printReport(MAX_BUTTONS, "Buttons");

    // #define MAX_ENCODERS 20
    for (i = 0; i < MAX_ENCODERS; i++)
        AddEncoder(1, 2, 1);
    printReport(MAX_ENCODERS, "Encoders");

    // #define MAX_ANALOG_INPUTS 16
    for (i = 0; i < MAX_ANALOG_INPUTS; i++)
        AddAnalog(1);
    printReport(MAX_ANALOG_INPUTS, "AnalogIns");

    // #define MAX_INPUT_SHIFTERS 4
    for (i = 0; i < MAX_INPUT_SHIFTERS; i++)
        AddInputShiftReg(1, 2, 3, 2);
    printReport(MAX_INPUT_SHIFTERS, "InputShiftRegs");

    // #define MAX_LEDSEGMENTS 4
    for (i = 0; i < MAX_LEDSEGMENTS; i++)
        AddLedSegment(1, 2, 3, 2, 15);
    printReport(MAX_LEDSEGMENTS, "LEDSegments");

    // #define MAX_STEPPERS 10
    for (i = 0; i < MAX_STEPPERS; i++)
        AddStepper(1, 2, 3, 4, 5);
    printReport(MAX_STEPPERS, "Steppers");

    // #define MAX_MFSERVOS 10
    for (i = 0; i < MAX_MFSERVOS; i++)
        AddServo(1);
    printReport(MAX_MFSERVOS, "Servos");

    // #define MAX_OUTPUT_SHIFTERS 4
    for (i = 0; i < MAX_OUTPUT_SHIFTERS; i++)
        AddOutShiftReg(1, 2, 3, 2);
    printReport(MAX_OUTPUT_SHIFTERS, "OutShiftRegs");

    // #define MAX_MFLCD_I2C 2
    for (i = 0; i < MAX_MFLCD_I2C; i++)
        AddLcdDisplay(1, 40, 2);
    printReport(MAX_MFLCD_I2C, "LCD displays");

#endif
}

// ************************************************************
// Setup
// ************************************************************
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
#if MF_SERVO_SUPPORT == 1
    lastServoUpdate = millis();
#endif

    lastButtonUpdate = millis();
    lastEncoderUpdate = millis(); // encoders will be updated every 1ms

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
    while (1) {
        setupData();
        delay(60000);
    }
#endif
}

void checkUpdate(uint8_t typ, uint32_t *tim, uint32_t intv)
{
    if (millis() - (*tim) >= (intv)) { 
        *tim = millis();              
        UpdateDevices(typ);           
    }                                 
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
