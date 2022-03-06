//
// OutputShifter.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFOutputShifter.h"

namespace OutputShifter
{
    DECLARE_VT_STUBS;   // see IODevice.h
   
    void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules);
    void OnInit(void);
    void OnSet(void);

}

// OutputShifter.h
