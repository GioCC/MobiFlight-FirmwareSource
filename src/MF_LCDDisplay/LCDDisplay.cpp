//
// LCDDisplay.cpp
//

#include <Arduino.h>
#include "MFBoards.h"
#include "MFLCDDisplay.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace LCDDisplay
{
    DEFINE_VT_STUBS(MFLCDDisplay);   // see IODevice.h

    void Add(uint8_t I2Caddress, uint8_t cols, uint8_t lines)
    {
        MFLCDDisplay *MFL;
        Stowage.AddItem(&MFL);

        if(MFL) {
            MFL->attach(I2Caddress, cols, lines);
            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added LCD display"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("LCD display: Memory full"));
            #endif
        }
    }

    void OnSet(void)
    {
        MFLCDDisplay *MFL;
        int     nLCD = cmdMessenger.readInt16Arg();
        char *output = cmdMessenger.readStringArg();
        //MFS = static_cast<MFSegments *>(Stowage.getNth(module, kTypeLedSegment));
        MFL = (MFLCDDisplay *)(Stowage.getNth((uint8_t)nLCD, kTypeLcdDisplayI2C));
        if(MFL) {
            MFL->setval(output);
            setLastCommandMillis();
        }
    }
}   // namespace

// LCDDisplay.cpp
