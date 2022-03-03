//
// Analog.h
//

#pragma once

#include <stdint.h>
#include "MFAnalog.h"
namespace Analog
{
    void Add(uint8_t pin, uint8_t sensitivity = 3, char const *name = "AnalogInput");
    void UpdateAverage(void);
}

// Analog.h
