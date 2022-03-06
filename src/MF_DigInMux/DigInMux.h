//
// DigInMux.h
//

#pragma once

#include <stdint.h>
#include "IOdevice.h"
#include "MFDigInMux.h"
#include "MFMuxDriver.h"
namespace DigInMux
{
    DECLARE_VT_STUBS;   // see IODevice.h

    void Add(uint8_t dataPin, uint8_t nRegs, char const *name = "MUXDigIn", bool mode = MFDigInMux::MuxModeFast);
}

// DigInMux.h
