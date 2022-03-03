#pragma once
#include <stdint.h>
#include "MFOutput.h"
namespace Output
{
void Add(uint8_t pin = 1);
void Clear(void);
void OnSet(void);
void PowerSave(bool state);
}
