//
// MFMuxDriver.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFMuxDriver.h"

static void directPinMode(uint8_t port, uint8_t pinMsk, uint8_t mode)
{
    volatile uint8_t *reg, *out;
    reg = portModeRegister(port);
    out = portOutputRegister(port);

    uint8_t oldSREG = SREG;
    cli();
    if (mode == INPUT_PULLUP) {
        *reg &= ~pinMsk;
        *out |= pinMsk;
    } else if (mode == OUTPUT) {
        *reg |= pinMsk;
    }
    SREG = oldSREG;
}

static void directPinOut(uint8_t port, uint8_t pinMsk, uint8_t val)
{
    volatile uint8_t *out;
    out = portOutputRegister(port);
    uint8_t oldSREG = SREG;
    cli();
    (val == LOW ? (*out &= ~pinMsk) : (*out |= pinMsk));
    SREG = oldSREG;
}

MFMuxDriver::MFMuxDriver(void)
{
    _inited = 0;
    for (uint8_t i = 0; i < 4; i++) {
        _selPort[i]    = 0xFF;
        _selPinMask[i] = 0xFF;
    }
}

// Registers a new MUX input block and configures the driver pins
void MFMuxDriver::
    attach(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin)
{
    _selPort[0]    = digitalPinToPort(Sel0Pin);
    _selPinMask[0] = digitalPinToBitMask(Sel0Pin);
    _selPort[1]    = digitalPinToPort(Sel1Pin);
    _selPinMask[1] = digitalPinToBitMask(Sel1Pin);
    _selPort[2]    = digitalPinToPort(Sel2Pin);
    _selPinMask[2] = digitalPinToBitMask(Sel2Pin);
    _selPort[3]    = digitalPinToPort(Sel3Pin);
    _selPinMask[3] = digitalPinToBitMask(Sel3Pin);

    for (uint8_t i = 0; i < 4; i++) {
        directPinMode(_selPort[i], _selPinMask[i], OUTPUT);
    }
        
    _inited = 1;
    setChannel(0);
}

void MFMuxDriver::detach()
{
    for (uint8_t i = 0; i < 4; i++) {
        if (_selPort[i] == 0xFF) continue;
        directPinMode(_selPort[i], _selPinMask[i], INPUT_PULLUP);
        _selPort[i]    = 0xFF;
        _selPinMask[i] = 0xFF;
    }
    _inited = 0;
}

// Sets the driver lines to select the specified channel
void MFMuxDriver::setChannel(uint8_t value)
{
    if (!_inited) return;

    // Ideally, setChannel() should change all pins atomically (at the same time):
    // since it doesn't, be advised that there will be signal glitches because
    // the actual code - which is not latched - spans several values as the single bits are changed.
    // This should not be an issue, because e.g. in an input mux the output is only read at the end,
    // once the code is stable.
    // (Please note that output value settling is a completely different effect.)
    // However, this effect might have to be taken into account.

    _channel = value;
    directPinOut(_selPort[0], _selPinMask[0], (value & 0x01)!=0);
    directPinOut(_selPort[1], _selPinMask[1], (value & 0x02)!=0);
    directPinOut(_selPort[2], _selPinMask[2], (value & 0x04)!=0);
    directPinOut(_selPort[3], _selPinMask[3], (value & 0x08)!=0);
}


// Returns currently selected channel
uint8_t MFMuxDriver::getChannel(void)
{
    return _channel;
}

// Increments current channel, wraps around to 0
uint8_t MFMuxDriver::nextChannel(void)
{
    setChannel((++_channel) % 16);
    return _channel;
}

// Temporarily stores current channel for later retrieval
void MFMuxDriver::saveChannel(void)
{
    _savedChannel = _channel;
}

// Restored previously stored channel
void MFMuxDriver::restoreChannel(void)
{
    setChannel(_savedChannel);
}

// MFMuxDriver.cpp
