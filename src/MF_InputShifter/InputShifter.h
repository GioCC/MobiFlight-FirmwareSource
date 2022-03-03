#pragma once
#include <stdint.h>

namespace InputShifter
{
void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name = "InputShifter");
void Clear();
//void OnInit();        // this is defined but not used!?
void read();
void OnTrigger();
}
