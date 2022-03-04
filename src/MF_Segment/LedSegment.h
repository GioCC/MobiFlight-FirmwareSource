//
// LedSegment.h
//

#pragma once

#include <stdint.h>
#include "MFSegments.h"

namespace LedSegment
{
    void Add(uint8_t dataPin, uint8_t csPin, uint8_t clkPin, uint8_t numDevices, uint8_t brightness);
    void OnInit(void);
    void OnSet(void);
    void OnSetBrightness(void);
}

// LedSegment.h
