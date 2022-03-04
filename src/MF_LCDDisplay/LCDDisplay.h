//
// LCDDisplay.h
//

#pragma once

#include <stdint.h>
#include "MFLCDDisplay.h"

namespace LCDDisplay
{
    
    void Add(uint8_t I2Caddress, uint8_t cols, uint8_t lines);
    void OnSet(void);

}

// LCDDisplay.h
