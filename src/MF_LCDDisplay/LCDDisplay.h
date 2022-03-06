//
// LCDDisplay.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFLCDDisplay.h"

namespace LCDDisplay
{
    DECLARE_VT_STUBS;   // see IODevice.h
   
    void Add(uint8_t I2Caddress, uint8_t cols, uint8_t lines);
    void OnSet(void);

}

// LCDDisplay.h
