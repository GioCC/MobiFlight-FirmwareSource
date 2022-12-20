//
// MFDigInMux.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>
#include "MFMuxDriver.h"

extern "C" {
typedef void (*MuxDigInEvent)(byte, uint8_t, const char *);
};

enum {
    MuxDigInOnPress,
    MuxDigInOnRelease,
};

class MFDigInMux
{
public:
    MFDigInMux(void);
    MFDigInMux(MFMuxDriver *MUX, const char *name);
    static void setMux(MFMuxDriver *MUX);
    static void attachHandler(MuxDigInEvent newHandler);

    void     attach(uint8_t dataPin, bool halfSize, char const *name);
    void     detach();
    void     clear();
    void     retrigger();
    void     update() { poll(DO_TRIGGER); };
    void     updateNext(uint8_t pass) { pollNext(DO_TRIGGER, pass); };
    uint16_t getValues(void) { return _lastState; }

private:
    enum { MUX_INITED   = 0,
           MUX_HALFSIZE = 1,
    };

    enum { DONT_TRIGGER = 0,
           DO_TRIGGER   = 1 };

    static MFMuxDriver  *_MUX;
    static MuxDigInEvent _inputHandler;

    const char *_name;
    uint8_t     _dataPin; // Data pin - MUX common, input to AVR
    uint8_t     _flags;
    uint16_t    _lastState;
    uint16_t    _tempState;

    void poll(bool doTrigger);
    void pollNext(bool doTrigger, uint8_t pass);
    bool detectChanges(uint16_t lastState, uint16_t currentState);
    void trigger(uint8_t channel, bool state);
};

// MFDigInMux.h