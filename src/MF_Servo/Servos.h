//
// Servos.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFServo.h"

namespace Servos
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t pin);
    void OnSet(void);
}

// Servos.h
