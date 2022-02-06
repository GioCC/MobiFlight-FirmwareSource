//
// outputHub.h
//

#ifndef _OUTPUTHUB_H
#define _OUTPUTHUB_H

#include <stdint.h>

#include <Arduino.h>
#include "MFBoards.h"
#include "StowManager.h"
#include "commandmessenger.h"
#include "config.h"

#include "MFOutput.h"
#include "MFSegments.h"
#include "MFOutputShifter.h"
#include "MFServo.h"
#include "MFStepper.h"
#include "MFLCDDisplay.h"

extern StowManager  Stowage;
extern CmdMessenger cmdMessenger;

// General Management functions

void UpdateServos();
void UpdateSteppers();
void ClearAll();
void PowerSave(uint8_t mode);

void ClearDeviceConfig(void);   // Inputs + outputs!

// Device add functions
// template <typename T> T* AddItem(T** dummy, StowManager *SM = &Stowage);


#endif // _OUTPUTHUB_H

