#pragma once
#include <stdint.h>
#include "MFLCDDisplay.h"

namespace LCDDisplay
{
void Add(uint8_t address = 0x24, uint8_t cols = 16, uint8_t lines = 2);
void Clear();
void OnSet();
}
