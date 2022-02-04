#include <Arduino.h>
#include "MFBoards.h"
#include "commandmessenger.h"
#include "registerpin.h"
#include "config.h"
#include "MFSegments.h"


#if MF_SEGMENT_SUPPORT == 1

MFSegments ledSegments[MAX_LEDSEGMENTS];
uint8_t ledSegmentsRegistered = 0;


void AddLedSegment(int dataPin, int csPin, int clkPin, int numDevices, int brightness)
{
  if (ledSegmentsRegistered == MAX_LEDSEGMENTS)
    return;

  if (isPinRegistered(dataPin) || isPinRegistered(clkPin) || isPinRegistered(csPin))
    return;

  ledSegments[ledSegmentsRegistered].attach(dataPin, csPin, clkPin, numDevices, brightness); // lc is our object

  registerPin(dataPin, kTypeLedSegment);
  registerPin(csPin, kTypeLedSegment);
  registerPin(clkPin, kTypeLedSegment);
  ledSegmentsRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added Led Segment"));
#endif
}

void ClearLedSegments()
{
  clearRegisteredPins(kTypeLedSegment);
  for (int i = 0; i != ledSegmentsRegistered; i++)
  {
    ledSegments[ledSegmentsRegistered].detach();
  }
  ledSegmentsRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared segments"));
#endif
}

void PowerSaveLedSegment(bool state)
{
  for (int i = 0; i != ledSegmentsRegistered; ++i)
  {
    ledSegments[i].powerSavingMode(state);
  }
}

void OnInitModule()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg();
  ledSegments[module].setBrightness(subModule, brightness);
  setLastCommandMillis(millis());
}

void OnSetModule()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  char *value = cmdMessenger.readStringArg();
  uint8_t points = (uint8_t)cmdMessenger.readInt16Arg();
  uint8_t mask = (uint8_t)cmdMessenger.readInt16Arg();
  ledSegments[module].display(subModule, value, points, mask);
  setLastCommandMillis(millis());
}

void OnSetModuleBrightness()
{
  int module = cmdMessenger.readInt16Arg();
  int subModule = cmdMessenger.readInt16Arg();
  int brightness = cmdMessenger.readInt16Arg();
  ledSegments[module].setBrightness(subModule, brightness);
  setLastCommandMillis(millis());
}

#endif