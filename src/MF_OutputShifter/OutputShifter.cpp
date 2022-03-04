//
// OutputShifter.cpp
//

#include <Arduino.h>
#include "MFBoards.h"
#include "MFOutputShifter.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace OutputShifter
{
    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules)
    {
        MFOutputShifter *MFS;

        Stowage.AddItem(&MFS);

        if(MFS) {
            MFS->attach(latchPin, clockPin, dataPin, modules);
            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added OutputShifter"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("OutputShifter: Memory full"));
            #endif
        }
    }

    void OnInit(void)
    {
        MFOutputShifter *MFS;
        uint8_t module = cmdMessenger.readInt16Arg();
        MFS = (MFOutputShifter *)(Stowage.getNth((uint8_t)module, kTypeOutShiftReg));
        if(MFS) {
            MFS->clear();
            setLastCommandMillis();
        }
    }

    void OnSet(void)
    {
        MFOutputShifter *MFS;
        int module  = cmdMessenger.readInt16Arg();
        char *pins  = cmdMessenger.readStringArg();
        int  value  = cmdMessenger.readInt16Arg();
        MFS = (MFOutputShifter *)(Stowage.getNth((uint8_t)module, kTypeOutShiftReg));
        if(MFS) {
            MFS->setPins(pins, value);
            setLastCommandMillis();
        }
    }
}   // namespace

// OutputShifter.cpp
