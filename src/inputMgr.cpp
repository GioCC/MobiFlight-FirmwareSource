#include <Arduino.h>
#include "commandmessenger.h"
#include "config.h"
#include "registerpin.h"
#include "MFButton.h"
#include "MFAnalog.h"
#include "MFEncoder.h"
#include "MFInputShifter.h"
#include "inputMgr.h"

#include "commandmessenger.h"

// ---------------------------------------------------
//  Button
// ---------------------------------------------------

namespace Button
{
    // MFButton buttons[MAX_BUTTONS];
    // uint8_t buttonsRegistered = 0;


    void handlerOnButton(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kButtonChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin = 1, char const *name = "Button")
    {
        if (buttonsRegistered == MAX_BUTTONS)
            return;

        if (isPinRegistered(pin))
            return;

        buttons[buttonsRegistered] = MFButton(pin, name);
        buttons[buttonsRegistered].attachHandler(handlerOnButton);
        registerPin(pin, kTypeButton);
        buttonsRegistered++;
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added button ") /* + name */);
        #endif
    }
}

// ---------------------------------------------------
//  Encoder
// ---------------------------------------------------

namespace Encoder
{
    // MFEncoder encoders[MAX_ENCODERS];
    // uint8_t encodersRegistered = 0;

    void handlerOnEncoder(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kEncoderChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t encoder_type = 0, char const *name = "Encoder")
    {
        if (encodersRegistered == MAX_ENCODERS)
            return;
        if (isPinRegistered(pin1) || isPinRegistered(pin2))
            return;

        encoders[encodersRegistered] = MFEncoder();
        encoders[encodersRegistered].attach(pin1, pin2, encoder_type, name);
        encoders[encodersRegistered].attachHandler(handlerOnEncoder);
        
        registerPin(pin1, kTypeEncoder);
        registerPin(pin2, kTypeEncoder);
        encodersRegistered++;
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added encoder"));
        #endif
    }

}       // end of namespace encoder

// ---------------------------------------------------
//  Analog
// ---------------------------------------------------

namespace Analog
{
    #if MF_ANALOG_SUPPORT == 1
    // MFAnalog analog[MAX_ANALOG_INPUTS];
    // uint8_t analogRegistered = 0;

    void handlerOnAnalogChange(int value, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kAnalogChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(value);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin = 1, char const *name = "AnalogInput", uint8_t sensitivity = 3)
    {
        if (analogRegistered == MAX_ANALOG_INPUTS)
            return;

        if (isPinRegistered(pin))
            return;

        analog[analogRegistered] = MFAnalog(pin, name, sensitivity);
        analog[analogRegistered].attachHandler(handlerOnAnalogChange);
        registerPin(pin, kTypeAnalogInput);
        analogRegistered++;
        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added analog device "));
        #endif
    }

    #endif
}       // end of namespace Analog

// ---------------------------------------------------
//  ShiftReg
// ---------------------------------------------------

namespace InputShifter
{
    #if MF_INPUT_SHIFTER_SUPPORT == 1
    // MFInputShifter inputShifters[MAX_INPUT_SHIFTERS];
    // uint8_t inputShiftersRegistered = 0;

    void handlerInputShifterOnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kInputShifterChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(pin);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name = "Shifter")
    {
        if (inputShiftersRegistered == MAX_INPUT_SHIFTERS)
            return;
        inputShifters[inputShiftersRegistered].attach(latchPin, clockPin, dataPin, modules, name);
        inputShifters[inputShiftersRegistered].clear();
        registerPin(latchPin, kTypeInputShifter);
        registerPin(clockPin, kTypeInputShifter);
        registerPin(dataPin, kTypeInputShifter);

        inputShifters[inputShiftersRegistered].attachHandler(handlerInputShifterOnChange);

        inputShiftersRegistered++;

        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added input shifter"));
        #endif
    }

    #endif
}       // end of namespace InputShifter
