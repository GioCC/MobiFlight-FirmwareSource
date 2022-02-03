#include <Arduino.h>
#include "commandmessenger.h"
#include "config.h"
#include "stowManager.h"
#include "registerpin.h"
#include "MFButton.h"
#include "MFAnalog.h"
#include "MFEncoder.h"
#include "MFInputShifter.h"
#include "inputMgr.h"

extern StowManager  InStow;

// ---------------------------------------------------
//  Button
// ---------------------------------------------------

namespace Button
{
    // MFButton buttons[MAX_BUTTONS];
    // uint8_t buttonsRegistered = 0;

    // (original) handler.
    // Options for the handler:
    // supply to the class...
    // ...a function that can accepts all its varied data (and will compose the message), e.g.
    //     >>> button_callback(uint8_t data1, uint16_t data2, uint8_t data3, char *name)
    // ...a generic 'send' function (or fns) that can accepts different-typed data
    //    (and will compose the message), e.g. 
    //     >>> callback_StartStop(STARTMSG);    // callback_StartStop(uint8_t code)
    //     >>> callback_Attach(&data1, NULL);   // callback_Attach(uint16_t* arg1, char* arg2) 
    //     >>> callback_Attach(&data2, NULL);
    //     >>> callback_Attach(&data3, NULL);
    //     >>> callback_Attach(NULL,   name);
    //     >>> callback_StartStop(ENDMSG);

    void handlerOnButton(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kButtonChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    // Specialized form (need to supply params)
    void Add(uint8_t pin = 1, char const *name = "Button")
    {
        uint8_t *inBut;
        //if (buttonsRegistered == MAX_BUTTONS) return;
        //if (isPinRegistered(pin)) return;
        
        inBut = InStow.add(sizeof(In_Button), In_Base::BUTTON); 
        if(inBut != NULL) {
            new ((void *)inBut) In_Button();  // or In_Button(pin, name);
            // TODO: ((In_Button *)inBut)->init(pin, name); 

            //buttons[buttonsRegistered] = MFButton(pin, name);
            //buttons[buttonsRegistered].attachHandler(handlerOnButton);
            //buttonsRegistered++;
            
            //registerPin(pin, kTypeButton);

            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added button ") /* + name */);
            #endif
        }
    }

    // Generic form (the param init will use the returned pointer)
    template <typename T> T* Add(T* dummy, StowManager *SM)
    {
        // dummy argument only used to set signature
        // Or make it 'Add(T** res, ...)' and take advantage of it to 
        // carry the return value 
        uint8_t *in;
        in = SM->add(T::getSize(), T::getType()); 
        if(in != NULL) {
            new ((void *)in) T;
            // TODO: param init, e.g. ((In_Button *)inBut)->init(pin, name); 
        }
        return (T*) in;
    }


    void UpdateAll()
    {
        In_Button *inBut;
        InStow.reset();
        while((inBut = (In_Button *)(InStow.getNext(In_Base::BUTTON))) != NULL) {
            inBut->Update();
        }
    }

    void UpdateAll(uint8_t type)
    {
        In_Base *in;
        InStow.reset();
        while((in = (In_Base *)(InStow.getNext(type))) != NULL) {
            in->Update();
        }
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
        if (encodersRegistered == MAX_ENCODERS) return;
        if (isPinRegistered(pin1) || isPinRegistered(pin2)) return;

        encoders[encodersRegistered] = MFEncoder();
        encoders[encodersRegistered].attach(pin1, pin2, encoder_type, name);
        encoders[encodersRegistered].attachHandler(handlerOnEncoder);
        encodersRegistered++;
        
        registerPin(pin1, kTypeEncoder);
        registerPin(pin2, kTypeEncoder);
        
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
        if (analogRegistered == MAX_ANALOG_INPUTS) return;
        if (isPinRegistered(pin)) return;

        analog[analogRegistered] = MFAnalog(pin, name, sensitivity);
        analog[analogRegistered].attachHandler(handlerOnAnalogChange);
        analogRegistered++;
        
        registerPin(pin, kTypeAnalogInput);
        
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
        if (inputShiftersRegistered == MAX_INPUT_SHIFTERS) return;

        inputShifters[inputShiftersRegistered].attach(latchPin, clockPin, dataPin, modules, name);
        inputShifters[inputShiftersRegistered].clear();
        inputShifters[inputShiftersRegistered].attachHandler(handlerInputShifterOnChange);
        inputShiftersRegistered++;

        registerPin(latchPin, kTypeInputShifter);
        registerPin(clockPin, kTypeInputShifter);
        registerPin(dataPin, kTypeInputShifter);

        #ifdef DEBUG
        cmdMessenger.sendCmd(kStatus, F("Added input shifter"));
        #endif
    }

    #endif
}       // end of namespace InputShifter
