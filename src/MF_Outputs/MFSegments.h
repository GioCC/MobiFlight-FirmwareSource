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

    MFSegments(void);

    static uint8_t getType(void) { return kTypeLedSegment; }
    //static uint8_t getSize(void) { return sizeof(MFSegments); }

    void    attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness);
    
    void    reset(uint8_t action);
    void    powerSave(bool state);
    void    detach(void);
    
    void    setval(byte module, char *string, byte points, byte mask, bool convertPoints = false);
    
    void    setBrightness(byte module, byte value);
    void    test(void);

    void    update(void) {};  // Stub required for emulated polymorphism

private:
    LedControl  _ledControl;
    byte        _moduleCount;
};
#endif