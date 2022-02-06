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

        MFInBase() {}

        // Common methods like following should exist, however since it's 
        // very difficult to define a common signature for the interface
        // (or because they could not be made accessible when required, 
        // like getSize()/getType/()), they're only listed here as a reminder.
        // Each class will define the corresponding methods with their own signature.
        
        // virtual uint8_t getSize(void)   = 0;
        // virtual uint8_t getType(void)   = 0;

        // virtual void setup(...)         = 0;     // Assign connections upon creation
        // virtual void attachHandler(...) = 0;     // When an input event occurs, manage it
        
        virtual void update(void)       = 0;
        virtual void onReset(void)      {};
        virtual void detach(void)       {};
};

#endif // _MFINBASE_H
