//
// DigInMux.cpp
//
#include <Arduino.h>
#include "MFBoards.h"
#include "MFDigInMux.h"
#include "MFMuxDriver.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"

// #if MF_DIGIN_MUX_SUPPORT == 1


extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace DigInMux
{
    DEFINE_VT_STUBS(MFDigInMux);   // see IODevice.h

    void OnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kDigInMuxChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(pin);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t dataPin, uint8_t nRegs, char const *name, bool mode)
    {
        MFDigInMux *MFI;

        Stowage.AddItem(&MFI);

        if(MFI) {
            MFI->attach(dataPin, (nRegs==1), name);
            MFI->setLazyMode(mode==MFDigInMux::MuxModeLazy);
            MFDigInMux::setMux(&MUX);
            MFDigInMux::attachHandler(OnChange);
            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added DigInMux"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("DigInMux: Memory full"));
            #endif
        }
    }

}

// #endif  // MF_DIGIN_MUX_SUPPORT

// DigInMux.cpp
