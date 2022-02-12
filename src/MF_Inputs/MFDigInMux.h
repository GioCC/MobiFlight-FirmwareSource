// MFDigInMux.h

#ifndef MFMUXDigitalIn_h
#define MFMUXDigitalIn_h

#include "MFIOdevice.h"
#include "MFMuxDriver.h"
#include <Arduino.h>

extern "C" {
typedef void (*MuxDigInEvent)(byte, uint8_t, const char *);
};

enum {
    MuxDigInOnPress,
    MuxDigInOnRelease,
};

class MFDigInMux : MFIOdevice
{
private:
    enum { 
        MUX_INITED   = 0,
        MUX_HALFSIZE = 1,
        MUX_LAZY     = 2,
    };

    static MFMuxDriver  *_MUX;
    static MuxDigInEvent _handler;

    const char     *_name;
    uint8_t         _dataPin; // Data pin - MUX common, input to AVR
    uint8_t         _flags;
    uint16_t        _lastState;

    void    poll(bool detect, bool lazy);
    void    detectChanges(uint16_t lastState, uint16_t currentState);
    void    trigger(uint8_t channel, bool state);

public:
    enum { 
        MUX_MODE_FAST = 0,
        MUX_MODE_LAZY = 1,
    };

    static uint8_t getType(void) { return kTypeDigInMux; }
    static uint8_t getSize(void) { return sizeof(MFDigInMux); }
    static void    attachHandler(MuxDigInEvent newHandler) { _handler = newHandler; }
    
    static void    setMux(MFMuxDriver *MUX);

    MFDigInMux(void);
    //MFDigInMux(MFMuxDriver *MUX, const char *name);
    void       attach(uint8_t dataPin, bool halfSize, char const *name);
 
    void       onReset(uint8_t action);
    void       update(void);
    void       detach(void);
    
    void       setLazyMode(bool mode);
    uint16_t   getValues(void) { return _lastState; }
};
#endif
