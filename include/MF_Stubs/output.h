#ifndef _output_h
#define _output_h
#include "MFOutput.h"

// ==== Inherited ====  (-> MF_device.cpp)
// -
void MFSegments::clear() {}     // Set value = 0?
// PowerSaveOutputs -> powerSavingMode
void MFSegments::powerSave(bool state);
// ClearOutputs
void MFSegments::detach();

// ==== Specialized ====
// setup
void AddOutput(uint8_t pin, char const *name = "Output");
// setval
void OnSetPin(uint8_t pin, uint8_t state);

// ==== Additional ====
void PowerSaveOutputs(bool state);

#endif