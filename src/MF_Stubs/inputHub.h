//
// InputHub.h
//

#ifndef _INPUTHUB_H
#define _INPUTHUB_H

#include <stdint.h>

#include "MFBoards.h"
#include "StowManager.h"
#include "commandmessenger.h"
#include "config.h"

extern StowManager  Stowage;
extern CmdMessenger cmdMessenger;

// General Management functions

// Inputs + outputs:    
//TODO: MOVE THESE FUNCTIONS TO OTHER FILE
void UpdateAll(uint8_t type);
void UpdateAll(void);
void RetriggerAll(void);
void SetPowerSave(uint8_t mode);


void SetInputHandlers(void);
#if MF_ANALOG_SUPPORT == 1
void UpdateAnalogAvg(void);
#endif

// Handlers to be assigned to input device classes

void OnButtonChange(uint8_t eventId, uint8_t pin, const char *name);
void OnEncoderChange(uint8_t eventId, uint8_t pin, const char *name);
#if MF_ANALOG_SUPPORT == 1
void OnAnalogChange(int value, uint8_t pin, const char *name);
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
void OnInputShiftRegChange(uint8_t eventId, uint8_t pin, const char *name);
#endif

// Device add functions

void AddButton(uint8_t pin, char const *name = "Button");
void AddEncoder(uint8_t pin1, uint8_t pin2, uint8_t encoder_type, char const *name = "Encoder");
#if MF_ANALOG_SUPPORT == 1
void AddAnalog(uint8_t pin, uint8_t sensitivity = 3, char const *name = "AnalogInput");
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
void AddInputShiftReg(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t nModules, char const *name = "InShiftReg");
#endif

#endif // _INPUTHUB_H

