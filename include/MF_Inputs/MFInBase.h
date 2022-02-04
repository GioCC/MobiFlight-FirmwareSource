//
// MFInBase.h
//
// (virtual) Base class for MF_xxx input devices 
//

#ifndef _MFINBASE_H
#define _MFINBASE_H

#include <stdint.h>

#include "config.h"

class MFInBase
{
    public:
        // typedef enum {
        //     NONE        = kTypeNotSet,
        //     BUTTON      = kTypeButton,
        //     ENCODER_SD  = kTypeEncoderSingleDetent, //deprecated
        //     ENCODER     = kTypeEncoder,
        //     ANALOG      = kTypeAnalogInput,
        //     SHIFTREG    = kTypeInputShifter,
        // } InType;

        MFInBase() {}

        // virtual uint8_t getSize(void)   = 0;
        // virtual uint8_t getType(void)   = 0;
        // virtual void OnChange(void) = 0;
        // virtual void Add(void) = 0;
        
        virtual void update(void)       = 0;
        virtual void onReset(void)      {};
        virtual void detach(void)       {};
};

#endif // _MFINBASE_H
