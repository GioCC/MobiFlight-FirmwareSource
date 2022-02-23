//
// InputHub.h
//

#ifndef _INPUTHUB_H
#define _INPUTHUB_H

#include <stdint.h>

#include "MFBoards.h"
#include "mobiflight.h"

#include "MFButton.h"
#include "MFAnalog.h"
#include "MFEncoder.h"
#include "MFInputShifter.h"
#include "MFMuxDriver.h"
#include "MFDigInMux.h"

// Exported variables

extern uint32_t lastButtonUpdate;
extern uint32_t lastEncoderUpdate;

#if MF_ANALOG_SUPPORT == 1
extern uint32_t lastAnalogAverage;
extern uint32_t lastAnalogRead;
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
extern uint32_t lastInputShifterUpdate;
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
extern uint32_t lastDigInMuxUpdate;
#endif

extern CmdMessenger cmdMessenger;
#if MF_MUX_SUPPORT == 1
extern MFMuxDriver MUX;
#endif

// General Management functions

void attachEventCallbacks(void);
void OnTrigger(void);

#if MF_ANALOG_SUPPORT == 1
void UpdateAnalogAvg(void);
#endif

// Handlers to be assigned to input device classes

void handlerOnButton(uint8_t eventId, uint8_t pin, const char *name);
void handlerOnEncoder(uint8_t eventId, uint8_t pin, const char *name);
#if MF_ANALOG_SUPPORT == 1
void handlerOnAnalogChange(int value, uint8_t pin, const char *name);
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
void handlerInputShifterOnChange(uint8_t eventId, uint8_t pin, const char *name);
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
void handlerDigInMuxOnChange(uint8_t eventId, uint8_t channel, const char *name);
#endif

// Device management functions

void AddButton(uint8_t pin, char const *name = "Button");
void ClearButtons();
void readButtons();

void AddEncoder(uint8_t pin1, uint8_t pin2, uint8_t encoder_type, char const *name = "Encoder");
void ClearEncoders();
void readEncoder();

#if MF_ANALOG_SUPPORT == 1
void AddAnalog(uint8_t pin, char const *name = "AnalogInput", uint8_t sensitivity = 3);
void ClearAnalog();
void readAnalog();
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
void AddInputShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name = "Shifter");
void ClearInputShifters();
void readInputShifters();
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
void AddDigInMux(uint8_t dataPin, uint8_t nRegs, char const *name = "MUXDigIn", bool mode = MFDigInMux::MUX_MODE_FAST);
void ClearDigInMux();
void readDigInMux();
#endif
#if MF_MUX_SUPPORT == 1
void AddMultiplexer(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin);
void ClearMultiplexer();
#endif

#endif // _INPUTHUB_H

