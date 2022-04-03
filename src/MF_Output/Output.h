//
// Output.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

namespace Output
{
    DECLARE_VT_STUBS;   // see IODevice.h
    
    void Add(uint8_t pin = 1);
    void OnSet(void);
}

// Output.h

