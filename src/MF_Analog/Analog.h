//
// Analog.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFAnalog.h"

namespace Analog
{
    DECLARE_VT_STUBS;   // see IODevice.h
 
    void Add(uint8_t pin, uint8_t sensitivity = 3, char const *name = "AnalogInput");
    void UpdateAverage(void);
}

// Analog.h
