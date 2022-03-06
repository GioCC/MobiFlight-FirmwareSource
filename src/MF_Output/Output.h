//
// Output.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFOutput.h"

namespace Output
{
    DECLARE_VT_STUBS;   // see IODevice.h
    
    void Add(uint8_t pin = 1);
    void OnSet(void);
}

// Output.h

