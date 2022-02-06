//
// InputHub.cpp
//

#include <new>

#include <Arduino.h>
#include "commandmessenger.h"
#include "config.h"
#include "stowManager.h"
//#include "registerpin.h"
#include "MFIOdevice.h"
#include "MFButton.h"
#include "MFAnalog.h"
#include "MFEncoder.h"
#include "MFInputShifter.h"
#include "inputHub.h"

// =============================================
//  General functions
// =============================================

void SetInputHandlers(void)
{
    MFButton::attachHandler(OnButtonChange);
    MFEncoder::attachHandler(OnEncoderChange);
    MFAnalog::attachHandler(OnAnalogChange);
    MFInputShifter::attachHandler(OnInputShiftRegChange);
}

void UpdateAllInputs(void)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
        in->update();
    }
}

void UpdateAllInputs(uint8_t type)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext(type))) != NULL) {
        in->update();
    }
}

void UpdateAnalogAvg(void)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext(kTypeAnalogInput))) != NULL) {
        ((MFAnalog *)in)->updateAverage();
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

// Generic form (the param init will use the returned pointer)
// template <typename T> T* AddItem(T** itemPtr, StowManager *SM)
// {
//     // Since itemPtr argument is required to set signature anyway, 
//     // we take advantage of it to carry the return value 
//     uint8_t *in;
//     in = SM->add(T::getSize(), T::getType()); 
//     if(in != NULL) {
//         new ((void *)in) T;
//         // param init done outside by specialized functions
//     }
//     if(*itemPtr != NULL) *itemPtr = <static_cast>(T*)in;
//     return <static_cast>(T*) in;
// }


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
    // Non-templated alternative would be more memory-consuming (both RAM and flash!):
    // MFB = (MFButton *)Stowage.add(MFButton::getSize(), MFButton::getType());
    // if(MFB) new ((void *)MFB) MFButton;

    MFB->setup(pin, name);

    #ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added encoder"));
    #endif
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

    MFE->setup(pin1, pin2, encoder_type, name);
    #ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added encoder"));
    #endif
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
    
    MFA->setup(pin, sensitivity, name);
    #ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added analog device "));
    #endif
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

    MFI->setup(latchPin, clockPin, dataPin, nModules, name);
    #ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added input shifter"));
    #endif
}

#endif

// inputHub.cpp
