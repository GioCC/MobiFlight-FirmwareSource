// MFSegments.cpp
//
// Copyright (C) 2013-2021

#include "MFSegments.h"

MFSegments::MFSegments()
{
    _moduleCount = 0;
}

void MFSegments::attach(int dataPin, int csPin, int clkPin, byte moduleCount, byte brightness)
{
    _ledControl.begin(dataPin, clkPin, csPin, moduleCount);
    _moduleCount = moduleCount;
    for (int i = 0; i != _moduleCount; ++i) {
        setBrightness(i, brightness);
        _ledControl.shutdown(i, false);
        _ledControl.clearDisplay(i);
    }
}

void MFSegments::detach()
{
    if (_moduleCount) {
        for (int i = 0; i != _moduleCount; ++i) {
            _ledControl.shutdown(i, true);
        }
        _moduleCount = 0;
    }
}

void MFSegments::setval(byte module, char *string, byte points, byte mask, bool convertPoints)
{
    if (_moduleCount) {
        byte digit = 8;
        byte pos = 0;
        for (int i = 0; i != 8; i++) {
            digit--;
            if (((1 << digit) & mask) == 0)
                continue;
            _ledControl.setChar(module, digit, string[pos], ((1 << digit) & points));
            pos++;
        }
    }
}

void MFSegments::setBrightness(byte module, byte value)
{
    if (module < _moduleCount) {
        if (value) {
            _ledControl.setIntensity(module, value - 1);
            _ledControl.shutdown(module, false);
        } else {
            _ledControl.shutdown(module, true);
        }
    }
}

void MFSegments::powerSave(bool state)
{
    for (byte i = 0; i < _moduleCount; ++i) {
        _ledControl.shutdown(i, state);
    }
}

void MFSegments::reset(uint8_t action)
{
    if (action != ONRESET_DEFAULT)
        return;
    if (_moduleCount) {
        for (int i = 0; i != _moduleCount; ++i) {
            setBrightness(i, 15);
            _ledControl.clearDisplay(i);
        }
    }
}

void MFSegments::test()
{
    if (_moduleCount) {
        byte _delay = 10;
        byte module = 0;
        byte digit = 0;

        for (digit = 0; digit < 8; digit++) {
            for (module = 0; module != _moduleCount; ++module) {
                _ledControl.setDigit(module, digit, 8, 1);
            }
            delay(_delay);
        }

        for (digit = 0; digit < 8; digit++) {
            for (module = 0; module != _moduleCount; ++module) {
                _ledControl.setChar(module, 7 - digit, '-', false);
            }
            delay(_delay);
        }

        for (digit = 0; digit < 8; digit++) {
            for (module = 0; module != _moduleCount; ++module) {
                _ledControl.setChar(module, 7 - digit, ' ', false);
            }
            delay(_delay);
        }
    }
}

// MFSegments.cpp