//
// DigInMux.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFDigInMux.h"
#include "MFMuxDriver.h"

extern MFMuxDriver MUX;

namespace DigInMux
{
    MFDigInMux *digInMux[MAX_DIGIN_MUX];
    uint8_t     digInMuxRegistered = 0;

    void handlerOnDigInMux(uint8_t eventId, uint8_t channel, const char *name)
    {
        cmdMessenger.sendCmdStart(kDigInMuxChange);
        cmdMessenger.sendCmdArg(name);
        cmdMessenger.sendCmdArg(channel);
        cmdMessenger.sendCmdArg(eventId);
        cmdMessenger.sendCmdEnd();
    };

    void Add(uint8_t dataPin, uint8_t nRegs, char const *name)
    {
        if (digInMuxRegistered == MAX_DIGIN_MUX)
            return;
        MFDigInMux *dip;
        if (!FitInMemory(sizeof(MFDigInMux))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("DigInMux does not fit in Memory"));
            return;
        }
        dip                          = new (allocateMemory(sizeof(MFDigInMux))) MFDigInMux(&MUX, name);
        digInMux[digInMuxRegistered] = dip;
        dip->attach(dataPin, (nRegs == 1), name);
        MFDigInMux::attachHandler(handlerOnDigInMux);
        digInMuxRegistered++;

#ifdef DEBUG2MSG
        cmdMessenger.sendCmd(kDebug, F("Added digital input MUX"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[digInMuxRegistered]->detach();
        }
        digInMuxRegistered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared dig. input Muxes"));
#endif
    }

    void read()
    {
        // By-device scan:
        // version where devices are scanned individually one after another

#if 0
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[i]->update();
        }
#endif

        // By-address scan:
        // Version where selector values are scanned sequentially,
        // and for every selector value all devices are scanned for 1 single input
        //
        // This version is probably slightly faster (each 4-bit muxDriver address
        // is set 1 time rather that 4); however it's likely worse with respect to
        // encoder reading.
        // Let's assume that an encoder is connected to 2 close Mux pins (most likely
        // occurrence, could be made a requirement); let's call "N" the number of
        // configured multiplexers (assume 16 bit), "Ta" the time for setting a selector
        // address, "Tr" the time to read an input (all including the overhead for
        // function calls).
        // With a by-device scan, its inputs are read at (slightly) longer intervals,
        // but sampled almost at the same instant; the time between their samples is about
        // (Ta + Tr).
        // With a by-address scan, the second input is read after a time of about
        // N * (16 * Tr + Ta), which is likely to be considerably slower (and also
        // depending on N).

        for (uint8_t sel = 0; sel < 16; sel++) {
            MUX.setChannel(sel);
            for (uint8_t i = 0; i < digInMuxRegistered; i++) {
                digInMux[i]->updateNext(sel);
            }
        }
    }

    void OnTrigger()
    {
        for (uint8_t i = 0; i < digInMuxRegistered; i++) {
            digInMux[i]->retrigger();
        }
    }
} // namespace

// DigInMux.cpp