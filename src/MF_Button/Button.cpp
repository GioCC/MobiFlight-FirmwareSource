//
// Button.cpp
//

#include <Arduino.h>
#include "MFBoards.h"
#include "MFButton.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace Button
{

    void OnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kButtonChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin, char const *name)
    {
        MFButton *MFB;
        Stowage.AddItem(&MFB);
        // Non-templated alternative would be:
        // MFB = (MFButton *)Stowage.add(sizeof(MFButton));
        // if(MFB) new ((void *)MFB) MFButton;

        if(MFB) {
            MFB->attach(pin, name);
            MFButton::attachHandler(OnChange);
            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added Button"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("Button: Memory full"));
            #endif
        }
    }
}  // namespace

// Button.cpp