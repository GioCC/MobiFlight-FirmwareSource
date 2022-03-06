//
// Encoder.cpp
//

#include <Arduino.h>
#include "MFBoards.h"
#include "MFEncoder.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace Encoder
{
    DEFINE_VT_STUBS(MFEncoder);   // see IODevice.h

    void OnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kEncoderChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin1, uint8_t pin2, uint8_t encoder_type, char const *name)
    {
        MFEncoder *MFE;

        Stowage.AddItem(&MFE);

        if(MFE) {
            MFE->attach(pin1, pin2, encoder_type, name);
            MFEncoder::attachHandler(OnChange);
            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added Encoder"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("Encoder: Memory full"));
            #endif
        }
    }
}   // namespace

// Encoder.cpp
