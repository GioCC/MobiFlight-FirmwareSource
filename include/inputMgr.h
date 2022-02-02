//
// Everything moved to MFAnalog.h
//

#ifndef _inputMgr
#define _inputMgr

#include "MFBoards.h"

namespace Button
{
void Add(uint8_t pin, char const *name);
void Clear();
void read();
void OnTrigger();
}

namespace Encoder
{
void Add(uint8_t pin1, uint8_t pin2, uint8_t encoder_type, char const *name);
void Clear();
void read();
}

#if MF_ANALOG_SUPPORT == 1
namespace Analog
{
void Add(uint8_t pin, char const *name, uint8_t sensitivity);
void Clear();
void read();
void readAverage();
}
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
namespace InputShifter
{
void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name);
void Clear();
void read();
void OnTrigger();
}
#endif


#endif