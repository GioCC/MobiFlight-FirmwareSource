//
// I2Coutput.cpp
//
// (C) MobiFlight Project 2023
//

#include "mobiflight.h"
#include "MFI2Coutput.h"
#include "I2Coutput.h"

namespace I2Coutput
{
    MFI2Coutput *I2Cout[MAX_MFI2COUT];
    uint8_t      I2Cout_Registered = 0;

    void Add(uint8_t address, uint8_t cols, uint8_t lines)
    {
        if (I2Cout_Registered == MAX_MFI2COUT)
            return;

        if (!FitInMemory(sizeof(MFI2Coutput))) {
            // Error Message to Connector
            cmdMessenger.sendCmd(kStatus, F("I2Coutput does not fit in Memory!"));
            return;
        }
        I2Cout[I2Cout_Registered] = new (allocateMemory(sizeof(MFI2Coutput))) MFI2Coutput;
        I2Cout[I2Cout_Registered]->attach(address, cols, lines);
        I2Cout_Registered++;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Added I2Coutput"));
#endif
    }

    void Clear()
    {
        for (uint8_t i = 0; i < I2Cout_Registered; i++) {
            I2Cout[i]->detach();
        }
        I2Cout_Registered = 0;
#ifdef DEBUG2CMDMESSENGER
        cmdMessenger.sendCmd(kDebug, F("Cleared I2Coutput"));
#endif
    }

    void OnSet()
    {
        int   address = cmdMessenger.readInt16Arg();
        char *output  = cmdMessenger.readStringArg();
        cmdMessenger.unescape(output); // TODO Required?
        I2Cout[address]->display(output);
        setLastCommandMillis();
    }

    bool isLCD(uint8_t addr)
    {
        uint8_t a = addr & 0x07;
        return (a == 0x20 || a == 0x38);
    }

} // namespace

// I2Coutput.cpp
