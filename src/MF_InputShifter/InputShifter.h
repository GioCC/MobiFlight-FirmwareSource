//
// InputShifter.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFInputShifter.h"
namespace InputShifter
{
    DECLARE_VT_STUBS;   // see IODevice.h
 
    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules = 1, char const *name = "InputShifter");
}

// InputShifter.h
