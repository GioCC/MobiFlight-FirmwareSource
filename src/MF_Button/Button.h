#pragma once

#include <stdint.h>

namespace Button
{
void Add(uint8_t pin, char const *name = "Button");
void Clear();
void read();
void OnTrigger();
}
