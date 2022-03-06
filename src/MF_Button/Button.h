//
// Button.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFButton.h"
namespace Button
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t pin, char const *name = "Button");
}

// Button.h
