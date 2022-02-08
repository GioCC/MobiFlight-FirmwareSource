//
// MFOutBase.h
//
// (virtual) Base class for MF_xxx input devices 
//

#ifndef _MFOUTBASE_H
#define _MFOUTBASE_H

#include <stdint.h>

#include "config.h"

class MFOutBase
{
    public:

        MFOutBase() {}

        virtual void onReset(void)          {};     // Clear/initialize the module state
        virtual void powerSave(uint8_t mode){};
        virtual void detach(void)           {};     // NOW CALLED "CLEAR"!!!

        // Common methods like following should exist, however since it's 
        // very difficult to define a common signature for the interface
        // (or because they could not be made accessible when required, 
        // like getSize()/getType/()), they're only listed here as a reminder.
        // Each class will define the corresponding methods with their own signature.

        // virtual uint8_t getSize(void)   = 0;
        // virtual uint8_t getType(void)   = 0;

        // virtual void setup(...)         = 0;     // Assign connections upon creation
        // virtual void update(...)        = 0;     // Send new values to the device
        
};

#endif // _MFOUTBASE_H
