#pragma once
#include <stdint.h>
#include "MFServo.h"

namespace Servos
{
void Add(uint8_t pin);
void Clear();
void OnSet();
void update();
}
