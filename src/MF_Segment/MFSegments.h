// MFSegments.h
//
/// \mainpage MF Button module for MobiFlight Framework
/// \par Revision History
/// \version 2.0 

#ifndef MFSegments_h
#define MFSegments_h

#include <Arduino.h>
#include <LedControl.h>
#include "MFIOdevice.h"   // For constants and documentation only!


class MFSegments  //: public MFIOdevice
{
public:
    static uint8_t getType(void) { return kTypeLedSegment; }

    MFSegments(void);

    void    attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness);
    void    detach(void);
    void    reset(uint8_t action);
    void    setval(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    void    update(void) {};  // Stub required for emulated polymorphism
    void    powerSave(bool state);
    
    void    setBrightness(byte module, byte value);
    void    test(void);

private:
    LedControl  _ledControl;
    byte        _moduleCount;
};
#endif