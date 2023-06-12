//
// Stepper.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "mobiflight.h"
#include "MFStepper.h"

namespace Stepper
{
    DEFINE_VT_STUBS(MFStepper); // see IODevice.h

    void Add(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin1, uint8_t mode, int8_t backlash, bool deactivateOutput)
    {
        // if (steppersRegistered == MAX_STEPPERS)
        //     return;

        // if (!FitInMemory(sizeof(MFStepper))) {
        //     // Error Message to Connector
        //     cmdMessenger.sendCmd(kStatus, F("Stepper does not fit in Memory!"));
        //     return;
        // }

        // steppers[steppersRegistered] = new (allocateMemory(sizeof(MFStepper))) MFStepper;
        // steppers[steppersRegistered]->attach(pin1, pin2, pin3, pin4, btnPin1, mode, backlash, deactivateOutput);

        // if (btnPin1 > 0) {
        //     // this triggers the auto reset if we need to reset
        //     steppers[steppersRegistered]->reset();
        // }
        // // all set
        // steppersRegistered++;

        // #ifdef DEBUG2CMDMESSENGER
        //     cmdMessenger.sendCmd(kDebug, F("Added stepper"));
        // #endif

        MFStepper *MFS;

        Stowage.AddItem(&MFS);

        if (MFS) {
            MFS->attach(pin1, pin2, pin3, pin4, btnPin1, mode, backlash, deactivateOutput);

            if (btnPin1 > 0) {
                // this triggers the auto reset if we need to reset
                MFS->reset(ONRESET_DEFAULT);
            }

#ifdef DEBUG2CMDMESSENGER
            cmdMessenger.sendCmd(kDebug, F("Added stepper"));
        } else {
            cmdMessenger.sendCmd(kDebug, F("Stepper: Memory full"));
#endif
        }
    }

    void Clear()
    {
        MFStepper *MFS;
        Stowage.reset();
        while (nullptr != (MFS = reinterpret_cast<MFStepper *>(Stowage.getNext(kTypeStepper)))) {
            MFS->detach();
        }
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared steppers"));
#endif
    }

    void OnSet()
    {
        MFStepper *MFS;
        uint8_t    index  = (uint8_t)cmdMessenger.readInt16Arg();
        long       newPos = cmdMessenger.readInt32Arg();

        MFS = reinterpret_cast<MFStepper *>(Stowage.getNth(index, kTypeStepper));
        if (MFS) {
            MFS->moveTo(newPos);
            setLastCommandMillis();
        }
    }

    void OnReset()
    {
        MFStepper *MFS;
        uint8_t    index = (uint8_t)cmdMessenger.readInt16Arg();

        MFS = reinterpret_cast<MFStepper *>(Stowage.getNth(index, kTypeStepper));
        if (MFS) {
            MFS->reset();
            setLastCommandMillis();
        }
    }

    void OnSetZero()
    {
        MFStepper *MFS;
        uint8_t    index = (uint8_t)cmdMessenger.readInt16Arg();

        MFS = reinterpret_cast<MFStepper *>(Stowage.getNth(index, kTypeStepper));
        if (MFS) {
            MFS->setZero();
            setLastCommandMillis();
        }
    }

    void OnSetSpeedAccel()
    {
        MFStepper *MFS;
        uint8_t    index    = (uint8_t)cmdMessenger.readInt16Arg();
        uint16_t   maxSpeed = cmdMessenger.readInt16Arg();
        uint16_t   maxAccel = cmdMessenger.readInt16Arg();

        MFS = reinterpret_cast<MFStepper *>(Stowage.getNth(index, kTypeStepper));
        if (MFS) {
            MFS->setMaxSpeed(maxSpeed);
            MFS->setAcceleration(maxAccel);
            // setLastCommandMillis();
        }
    }

    void Update()
    {
        MFStepper *MFS;
        Stowage.reset();
        while (nullptr != (MFS = reinterpret_cast<MFStepper *>(Stowage.getNext(kTypeStepper)))) {
            MFS->update();
        }
    }

    void PowerSave(bool state)
    {
        MFStepper *MFS;
        Stowage.reset();
        while (nullptr != (MFS = reinterpret_cast<MFStepper *>(Stowage.getNext(kTypeStepper)))) {
            MFS->powerSave(state);
        }
    }

} // namespace

// Stepper.cpp
