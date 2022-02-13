//
// InputHub.cpp
//

#include <Arduino.h>
#include "commandmessenger.h"
#include "config.h"
#include "stowManager.h"

#include "inputHub.h"
// Included by inputHub.h:
// #include "MFIOdevice.h"
// #include "MFButton.h"
// #include "MFAnalog.h"
// #include "MFEncoder.h"
// #include "MFInputShifter.h"
// #include "MFDigInMux.h"


// =============================================
//  General functions
// =============================================

void SetInputHandlers(void)
{
    MFButton::attachHandler(OnButtonChange);
    MFEncoder::attachHandler(OnEncoderChange);
    #ifdef MF_ANALOG_SUPPORT
    MFAnalog::attachHandler(OnAnalogChange);
    #endif
    #ifdef MF_INPUT_SHIFTER_SUPPORT
    MFInputShifter::attachHandler(OnInputShiftRegChange);
    #endif
    //TODO maybe move to some semantically more appropriate function
#if MF_DIGIN_MUX_SUPPORT == 1
    MFDigInMux::setMux(&MUX);
#endif

}

// ---------------------------------------------------
//  Button
// ---------------------------------------------------

void OnButtonChange(uint8_t eventId, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kButtonChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};

void AddButton(uint8_t pin, char const *name)
{
    MFButton *MFB;
    Stowage.AddItem(&MFB);
    // Non-templated alternative would be:
    // MFB = (MFButton *)Stowage.add(sizeof(MFButton));
    // if(MFB) new ((void *)MFB) MFButton;

    if(MFB) {
        MFB->attach(pin, name);
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added Button"));
    } else {
        cmdMessenger.sendCmd(kStatus, F("Button: Memory full"));
        #endif
    }
}

// ---------------------------------------------------
//  Encoder
// ---------------------------------------------------

void OnEncoderChange(uint8_t eventId, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kEncoderChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};

void AddEncoder(uint8_t pin1, uint8_t pin2, uint8_t encoder_type, char const *name)
{
    MFEncoder *MFE;

    Stowage.AddItem(&MFE);

    if(MFE) {
        MFE->attach(pin1, pin2, encoder_type, name);
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added Encoder"));
    } else {
        cmdMessenger.sendCmd(kStatus, F("Encoder: Memory full"));
        #endif
    }
}

// ---------------------------------------------------
//  Analog
// ---------------------------------------------------

#if MF_ANALOG_SUPPORT == 1

void OnAnalogChange(int value, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kAnalogChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(value);
    cmdMessenger.sendCmdEnd();
};

void AddAnalog(uint8_t pin, uint8_t sensitivity, char const *name)
{
    MFAnalog *MFA;

    Stowage.AddItem(&MFA);
    
    if(MFA) {
        MFA->attach(pin, sensitivity, name);
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added Analog"));
    } else {
        cmdMessenger.sendCmd(kStatus, F("Analog: Memory full"));
        #endif
    }
}

void UpdateAnalogAvg(void)
{
    MFAnalog *ain;

    Stowage.reset();
    while((ain = (MFAnalog *)Stowage.getNext(kTypeAnalogInput)) != NULL) {
        ain->updateAverage();
    }
}

#endif

// ---------------------------------------------------
//  ShiftReg
// ---------------------------------------------------

#if MF_INPUT_SHIFTER_SUPPORT == 1

void OnInputShiftRegChange(uint8_t eventId, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kInputShifterChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(pin);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};

void AddInputShiftReg(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t nModules, char const *name)
{
    MFInputShifter *MFI;

    Stowage.AddItem(&MFI);

    if(MFI) {
        MFI->attach(latchPin, clockPin, dataPin, nModules, name);
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added InShiftReg"));
    } else {
        cmdMessenger.sendCmd(kStatus, F("InShiftReg: Memory full"));
        #endif
    }
}

#endif

// inputHub.cpp
