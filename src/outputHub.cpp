//
// outputHub.cpp
//

#include <new>

#include <Arduino.h>
#include "commandmessenger.h"
#include "config.h"
#include "stowManager.h"

#include "MFOutBase.h"
#include "MFButton.h"
#include "MFAnalog.h"
#include "MFEncoder.h"
#include "MFInputShifter.h"
#include "outputHub.h"

// =============================================
//  General functions
// =============================================

void UpdateServos(void)
{
    //TODO
}

void UpdateSteppers(void)
{
    //TODO
}


//TODO This will be used also for inputs!!! 
void ClearDeviceConfig(void)
{
    MFIOdevice *in;
    Stowage.reset();
    while((in = (MFIOdevice *)(Stowage.getNext())) != NULL) {
        in->detach();
    }
    Stowage.wipe();
}

//TODO This will be used also for inputs!!! 
// Generic form (the param init will use the returned pointer)
// template <typename T> T* AddItem(T** itemPtr, StowManager *SM)
// {
//     // Since itemPtr argument is required to set signature anyway, 
//     // we take advantage of it to carry the return value 
//     uint8_t *in;
//     in = SM->add(T::getSize(), T::getType()); 
//     if(in != NULL) {
//         new ((void *)in) T;
//         // param init done outside by specialized functions
//     }
//     if(*itemPtr != NULL) *itemPtr = <static_cast>(T*)in;
//     return <static_cast>(T*) in;
// }


// ---------------------------------------------------
//  Pin Output
// ---------------------------------------------------


// ---------------------------------------------------
//  LEDSegment
// ---------------------------------------------------

#if MF_SEGMENT_SUPPORT == 1
#endif

// ---------------------------------------------------
//  Output ShiftReg
// ---------------------------------------------------

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
#endif

// ---------------------------------------------------
//  Servo
// ---------------------------------------------------

#if MF_SERVO_SUPPORT == 1
#endif

// ---------------------------------------------------
//  Stepper
// ---------------------------------------------------

#if MF_STEPPER_SUPPORT == 1
#endif

// ---------------------------------------------------
//  LCD
// ---------------------------------------------------

#if MF_LCD_SUPPORT == 1
#endif

// outputHub.cpp
