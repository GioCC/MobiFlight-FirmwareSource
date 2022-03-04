//
// Output.cpp
//

#include <Arduino.h>
#include "MFBoards.h"
#include "MFOutput.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace Output
{
    void Add(uint8_t pin)
    {
        MFOutput *MFO;

        Stowage.AddItem(&MFO);

        if(MFO) {
            MFO->attach(pin);

            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added Output"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("MFoutput: Memory full"));
            #endif
        }
    }

    void OnSet(void)
    {
        // MFOutput *MFO;
        uint8_t nOutput = cmdMessenger.readInt16Arg();
        uint8_t state   = cmdMessenger.readInt16Arg();      // interpret string as boolean

        // MFO = (MFOutput *)(Stowage.getNth((uint8_t)nOutput, kTypeOutput));
    
        // CURRENTLY INCORRECT:
        // Here, "Pin" is the actual output pin, NOT the object index!!!
        // Patch:
        // Build a dummy object just for the purpose of setting the value
        MFOutput MFOut;
        MFOut.attach(nOutput);

        // Once the situation is rectified, uncomment statements below:
        // if(MFO) {
            // MFO->setval(state);       //analogWrite(pin, state);
            MFOut.setval(state);
            setLastCommandMillis();
        // }
    }
}   // namespace

// Output.cpp
