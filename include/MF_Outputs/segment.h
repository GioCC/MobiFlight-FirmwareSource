#ifndef _segment_h
#define _segment_h


void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness);
void ClearLedSegments();
void PowerSaveLedSegment(bool state);
void OnInitModule();
void OnSetModule();
void OnSetModuleBrightness();


#endif