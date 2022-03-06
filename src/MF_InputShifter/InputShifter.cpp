//
// InputShifter.cpp
//
#include <Arduino.h>
#include "MFBoards.h"
#include "MFInputShifter.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"

// #if MF_INPUT_SHIFTER_SUPPORT == 1

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace InputShifter
{

    DEFINE_VT_STUBS(MFInputShifter);   // see IODevice.h

    void OnChange(uint8_t eventId, uint8_t pin, const char *name)
    {
        cmdMessenger.sendCmdStart(kInputShifterChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(pin);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t nModules, char const *name)
    {
        MFInputShifter *MFI;

        Stowage.AddItem(&MFI);

        if(MFI) {
            MFI->attach(latchPin, clockPin, dataPin, nModules, name);
            MFInputShifter::attachHandler(OnChange);
            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added InShiftReg"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("InShiftReg: Memory full"));
            #endif
        }
    }
}  // namespace

// #endif  // MF_INPUT_SHIFTER_SUPPORT

// InputShifter.cpp
