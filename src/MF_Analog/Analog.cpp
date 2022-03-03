//
// Analog.cpp
//
#include <Arduino.h>
#include "MFBoards.h"
#include "MFAnalog.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"
#include "config.h"

// #if MF_ANALOG_SUPPORT == 1

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace Analog
{

    void OnChange(int value, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kAnalogChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(value);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t pin, uint8_t sensitivity, char const *name)
    {
        MFAnalog *MFA;

        Stowage.AddItem(&MFA);
        
        if(MFA) {
            MFA->attach(pin, sensitivity, name);
            MFAnalog::attachHandler(OnChange);
            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added Analog"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("Analog: Memory full"));
            #endif
        }
    }

    void UpdateAverage(void)
    {
        MFAnalog *ain;

        Stowage.reset();
        while((ain = (MFAnalog *)Stowage.getNext(kTypeAnalogInput)) != NULL) {
            ain->updateAverage();
        }
    }
} // namespace

// #endif  //MF_ANALOG_SUPPORT

// Analog.cpp
