//
// MFIOdevice.h
//
// (virtual) Base class for MF_xxx input/output devices 
//

#ifndef _MFIODEVICE_H
#define _MFIODEVICE_H

#include <stdint.h>
#include "config.h"

class MFIOdevice
{
    public:

        MFIOdevice() {}

        virtual void clear(void)           = 0;     //     [OUT] Clear/initialize the module state
        virtual void powerSave(uint8_t mode){};     // [IN][OUT] Set power saving state
        virtual void detach(void)           {};     // [IN][OUT] Called "Clear" in old FW

        virtual void update(void)       = 0;        // [IN][OUT] Fetch values (I)/ Update internal state (O)
        virtual void onReset(void)      {};
        virtual void detach(void)       {};         // [IN][OUT] Cleanup resources when stood down

        // Common methods like following should exist, however since it's 
        // very difficult to define a common signature for the interface
        // (or because they could not be made accessible when required, 
        // like getSize()/getType/()), they're only listed here as a reminder.
        // Each class will define the corresponding methods with their own signature.

        // virtual uint8_t getSize(void)   = 0;
        // virtual uint8_t getType(void)   = 0;

        // virtual void setup(...)         = 0;     // [IN][OUT] Assign connections upon creation
        // virtual void attachHandler(...) = 0;     // [IN]      When an input event occurs, manage it
        // virtual void setval(...)        = 0;     //     [OUT] Send new values to the device

};

#endif // _MFIODEVICE_H
