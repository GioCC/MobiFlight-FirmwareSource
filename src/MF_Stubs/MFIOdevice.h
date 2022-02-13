//
// MFIOdevice.h
//
// (virtual) Base class for MF_xxx input/output devices 
//

#ifndef _MFIODEVICE_H
#define _MFIODEVICE_H

#include <stdint.h>
#include "config.h"

// Arguments of onReset
// (These should belong to the class - see also comment below)
enum {
    ONRESET_PRESS, 
    ONRESET_RELEASE, 
    ONRESET_DEFAULT = ONRESET_PRESS, 
};

// This class is meant to be used as a base class for all MF devices, both input and output.
//
// Ordinarily, this would make for a sensible choice, allowing to exploit polymorphism mainnly in
// the "bulk" device processing functions.
// However, the overhead entailed in inheritance with virtual members (additional 2 bytes of RAM
// for the vtables for EACH object, plus the vtables themselves) has been deemed excessive
// for the limited available resources.
// A "custom" polymorphism has therefore been implemented by storing a 1-byte type code
// with each object and handling the method invocation of the proper class explicitly.
// 
// The following class has been left in place as an interface template and a remainder of 
// what methods each device class should supply, even though that is no longer enforced by
// inheritance.

class MFIOdevice
{
    public:

        MFIOdevice() {}

        virtual void update(void)       {};         // [IN][OUT] Fetch values (I)/ Update internal state (O)
        virtual void powerSave(uint8_t mode){};     // [IN][OUT] Set power saving state
        virtual void detach(void)       {};         // [IN][OUT] Cleanup resources when stood down
        virtual void onReset(uint8_t action = ONRESET_DEFAULT)
                                        {};         // [IN][OUT] Clear the module state. For inputs: does NOT read current actual state!

        // Note
        // The onReset function has slightly different meanings for input and output devices:
        // - for INPUTS, it has to first update the current state, and then generate a retrigger sequence
        //   according to the logic described in 
        //   https://github.com/MobiFlight/MobiFlight-Connector/issues/497 and 
        //   https://github.com/MobiFlight/MobiFlight-Connector/pull/502.
        //   This requires firing all Release events first, followed by all Press events.
        //   (IMPORTANT: these trigger events are generated on CURRENT STATUS, not ON CHANGE!)
        //   In order to accomplish this, the function has to be called twice:
        //   - the first time with parameter "RELEASE", whereby (for every device) only inputs in the Released state
        //     will trigger an event;
        //   - the first time with parameter "PRESS", whereby only inputs in the Released state
        //     will trigger an event.
        // - for OUTPUTS, it will clear the internal state of all outputs (whatever this means for any
        //   given device) and send/activate that status.
        // Since the function must be called twice (because of inputs), in order to avoid unnecessary 
        // double operations:   
        // - for INPUTS, only the call with argument "RELEASE", which is made first, performs an update
        //   (peripheral poll / data refresh);
        // - for OUTPUTS, only the call with argument "PRESS" (which is also defined as default)
        //   will be at all effective.

        // Common methods like following should exist, however since it's 
        // very difficult to define a common signature for the interface
        // (or because they could not be made accessible when required, 
        // like getSize()/getType()), they're only listed here as a reminder.
        // Each class will define the corresponding methods with their own signature.

        // static  uint8_t getType(void)   = 0;     // [IN][OUT] Return the class (type) code
        // static  uint8_t getSize(void)   = 0;     // [IN][OUT] (Optional, it really boils down to a straight sizeof())

        // virtual void attach(...)         = 0;    // [IN][OUT] Assign connections upon creation
        // virtual void attachHandler(...) = 0;     // [IN]      When an input event occurs, manage it
        // virtual void setval(...)        = 0;     //     [OUT] Send new values to the device

};

#endif // _MFIODEVICE_H

//!TEMPLATE:

    // static uint8_t getType(void) { return kType...; }
    // static uint8_t getSize(void) { return sizeof(MF...); }

    // MF...(void);
    // void attach(...);

    // void onReset(void);
    // void update(void);
    // void powerSave(uint8_t state);
    // void detach(void);
    
    // void setval(...);

//! TEMPLATE END