#ifndef _output_h
#define _output_h
#include "MFOutput.h"

// ==== Inherited ====  (-> MF_device.cpp)
// -
void MFOutput::onReset() {}     // Set value = 0?
// PowerSaveOutputs -> powerSavingMode
void MFOutput::powerSave(bool state);
// ClearOutputs
void MFOutput::detach();

// ==== Specialized ====
// setup
void AddOutput(uint8_t pin);
// setval
void OnSetPin(uint8_t pin, uint8_t state);

// ==== Additional ====
void PowerSaveOutputs(bool state);

#endif