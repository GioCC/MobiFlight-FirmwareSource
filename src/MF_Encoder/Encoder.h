//
// Encoder.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFEncoder.h"

namespace Encoder
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t pin1, uint8_t pin2, uint8_t encoder_type = 0, char const *name = "Encoder");
}

// Encoder.h
