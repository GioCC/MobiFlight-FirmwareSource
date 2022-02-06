#ifndef _segment_h
#define _segment_h
#include "MFSegments.h"

// ==== Inherited ====  (-> MF_device.cpp)
// OnInitModule  (== OnSetModuleBrightness!!!)
void MFSegments::clear();
// PowerSaveLedSegment -> powerSavingMode
void MFSegments::powerSave(bool state);
// ClearLedSegments
void MFSegments::detach();

// ==== Specialized ====
// setup
void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness);
// setval
void OnSetModule(byte module, char *string, byte points, byte mask, bool convertPoints);

// ==== Additional ====
void OnSetModuleBrightness();
void PowerSaveLedSegment(bool state);


#endif