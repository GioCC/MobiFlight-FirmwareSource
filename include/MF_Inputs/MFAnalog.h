// MFAnalog.h
//
/// \mainpage MF MFAnalog module for MobiFlight Framework
/// \par Revision History
/// \version 1.0 Initial release
/// \author  Manfred Berry (manfred@nystedberry.info) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2021 Manfred Berry

#ifndef MFAnalog_h
#define MFAnalog_h

#include "MFIOdevice.h"

#define ADC_MAX_AVERAGE                 8           // must be 2^n
#define ADC_MAX_AVERAGE_LOG2            3           // please calculate LOG2(ADC_MAX_AVERAGE)

extern "C"
{
  // callback functions
  typedef void (*analogEventHandler) (int, uint8_t, const char *);
};


/////////////////////////////////////////////////////////////////////
/// \class MFAnalog MFAnalog.h <MFAnalog.h>

class MFAnalog: public MFIOdevice
{
private:
    static analogEventHandler   _handler; 

    const char* _name;
    uint8_t     _pin;

    int         _lastValue;
    uint8_t     _sensitivity;

    uint16_t    ADC_Buffer[ADC_MAX_AVERAGE] = {0};     // Buffer for all values from each channel  
    uint16_t    ADC_Average_Total = 0;                 // sum of sampled values, must be divided by ADC_MAX_AVERAGE to get actual value
    volatile 
    uint8_t     ADC_Average_Pointer = 0;       // points to the actual position in ADC_BUFFER
    uint32_t    _lastReadBuffer;

public:
    static uint8_t getType(void) { return kTypeAnalogInput; }
    static uint8_t getSize(void) { return sizeof(MFAnalog); }
    static void attachHandler(analogEventHandler newHandler)  { _handler = newHandler; }   

    MFAnalog(void)  {};
    
    //MFAnalog(uint8_t pin = 1, const char * name = "Analog Input", uint8_t sensitivity = 2);
    void setup(uint8_t pin, uint8_t sensitivity = 2, const char *name = "Analog Input" );

    void update();
    void onReset(void)      {};
    void detach(void)       {};

    void updateAverage();   
    
};

#endif 