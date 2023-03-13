//
// MFMuxDriver.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFMuxDriver.h"

MFMuxDriver::MFMuxDriver(void)
{
    _inited = 0;
    for (uint8_t i = 0; i < 4; i++) {
#ifdef ARDUINO_ARCH_AVR
        _selPort[i]    = 0xFF;
        _selPinMask[i] = 0xFF;
#else
        _selPin[i] = 0xFF;
#endif
    }
}

// Registers a new MUX input block and configures the driver pins
void MFMuxDriver::
    attach(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin)
{
#ifdef ARDUINO_ARCH_AVR
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
#else
    _selPin[0] = Sel0Pin;
    _selPin[1] = Sel1Pin;
    _selPin[2] = Sel2Pin;
    _selPin[3] = Sel3Pin;

    for (uint8_t i = 0; i < 4; i++) {
        PinMode(_selPin[i], OUTPUT);
    }
#endif

    _inited = 1;
    setChannel(0);
}

void MFMuxDriver::detach()
{
    for (uint8_t i = 0; i < 4; i++) {
#ifdef ARDUINO_ARCH_AVR
        if (_selPort[i] == 0xFF) continue;
        directPinMode(_selPort[i], _selPinMask[i], INPUT_PULLUP);
        _selPort[i]    = 0xFF;
        _selPinMask[i] = 0xFF;
#else
        if (_selPin[i] == 0xFF) continue;
        pinMode(_selPin[i], INPUT_PULLUP);
        _selPin[i] = 0xFF;
#endif
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
#ifdef ARDUINO_ARCH_AVR
    directPinOut(_selPort[0], _selPinMask[0], (value & 0x01) != 0);
    directPinOut(_selPort[1], _selPinMask[1], (value & 0x02) != 0);
    directPinOut(_selPort[2], _selPinMask[2], (value & 0x04) != 0);
    directPinOut(_selPort[3], _selPinMask[3], (value & 0x08) != 0);
#else
    digitalWrite(_selPin[0], (value & 0x01) != 0);
    digitalWrite(_selPin[1], (value & 0x02) != 0);
    digitalWrite(_selPin[2], (value & 0x04) != 0);
    digitalWrite(_selPin[3], (value & 0x08) != 0);
#endif
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
