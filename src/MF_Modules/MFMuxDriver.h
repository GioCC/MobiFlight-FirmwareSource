//
// MFMuxDriver.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <Arduino.h>

extern "C" {
typedef void (*MuxDigInEvent)(byte, uint8_t, const char *);
};

class MFMuxDriver
{
public:
    MFMuxDriver(void);
    void attach(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin);
    void detach();

    uint8_t setChannel(uint8_t value);

    // Get stored channel number (see comments to RestoreChannel)
    uint8_t getChannel(void) { return _channel; };

    // Increments current channel, wraps around to 0
    uint8_t nextChannel(void) { return setChannel((++_channel) % 16); }

    // Temporarily stores current channel for later retrieval
    void saveChannel(void) { _savedChannel = _channel; } // Not reentrant - one level only

    // Restores previously stored channel
    // >>> This intentionally only restores the channel number, NOT the selector outputs,
    // in order not to waste time.
    // A caller will most likely set a new channel right away, and use the stored value
    // just to determine which channel should be next.
    void restoreChannel(void) { _channel = _savedChannel; }

private:
    enum { MUX_INITED = 0,
    };

    uint8_t _selPin[4]; // Selector pins; 0 is LSb
    uint8_t _flags;
    uint8_t _channel;
    uint8_t _savedChannel;
};

// MFMuxDriver.h
