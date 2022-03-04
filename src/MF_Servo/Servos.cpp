//
// Servos.cpp
//

#include <Arduino.h>
#include "MFBoards.h"
#include "MFServo.h"
#include "commandmessenger.h"
#include "stowManager.h"
#include "mobiflight.h"

extern CmdMessenger cmdMessenger;
extern StowManager  Stowage;

namespace Servos
{
    void Add(uint8_t pin)
    {
        MFServo *MFS;

        Stowage.AddItem(&MFS);

        if(MFS) {
            MFS->attach(pin, true);
            #ifdef DEBUG
            cmdMessenger.sendCmd(kStatus, F("Added Servo"));
        } else {
            cmdMessenger.sendCmd(kStatus, F("Servo: Memory full"));
            #endif
        }
    }

    void OnSet(void)
    {
        MFServo *MFS;
        int nServo = cmdMessenger.readInt16Arg();
        int value  = cmdMessenger.readInt16Arg();
        MFS = (MFServo *)(Stowage.getNth((uint8_t)nServo, kTypeServo));
        if(MFS) {
            MFS->setval(value);
            setLastCommandMillis();
        }
    }
}   // namespace

// Servos.cpp
