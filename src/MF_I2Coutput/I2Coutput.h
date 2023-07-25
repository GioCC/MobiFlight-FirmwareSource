//
// I2Coutput.h
//
// (C) MobiFlight Project 2023
//

#pragma once
#include <stdint.h>

namespace I2Coutput
{
    void Add(uint8_t address, uint8_t cols, uint8_t lines);
    void Clear();
    void OnSet();

    bool isLCD(uint8_t addr);
}

// I2Coutput.h
