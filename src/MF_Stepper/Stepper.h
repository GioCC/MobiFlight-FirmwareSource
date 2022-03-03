#pragma once
#include <stdint.h>
#include "MFStepper.h"

namespace Stepper
{
void Add(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t btnPin1);
void Clear();
void OnSet();
void OnReset();
void OnSetZero();
void update();
}
