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
    uint8_t getChannel(void) { return _channel; };

private:
    enum { MUX_INITED = 0,
    };

    uint8_t _selPin[4]; // Selector pins; 0 is LSb
    uint8_t _flags;
    uint8_t _channel;
};

// MFMuxDriver.h
