// MFOutputShifter.cpp
//
// Copyright (C) 2021

#include "MFOutputShifter.h"

MFOutputShifter::MFOutputShifter()
{
    _moduleCount = 0;
}

void MFOutputShifter::attach(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t moduleCount)
{
    _latchPin = latchPin;
    _clockPin = clockPin;
    _dataPin = dataPin;
    _moduleCount = moduleCount;

    pinMode(_latchPin, OUTPUT);
    pinMode(_clockPin, OUTPUT);
    pinMode(_dataPin, OUTPUT);

    //onReset();
}

void MFOutputShifter::onReset(uint8_t action)
{
    if (action != ONRESET_DEFAULT) return;
    if (0 == _moduleCount) return;
    for (uint8_t i = _moduleCount; i > 0; i--) {
        _outputBuffer[i] = 0;
    }
    update();
}

void MFOutputShifter::update()
{
    digitalWrite(_latchPin, LOW);
    for (uint8_t i = _moduleCount; i > 0; i--) {
        shiftOut(_dataPin, _clockPin, MSBFIRST, _outputBuffer[i]); // LSBFIRST, MSBFIRST,
    }
    digitalWrite(_latchPin, HIGH);
}

void MFOutputShifter::powerSave(uint8_t state)
{
    //onReset();
}

void MFOutputShifter::detach()
{
    //onReset();
    _moduleCount = 0;
}

void MFOutputShifter::setPin(uint8_t pin, uint8_t value, uint8_t refresh)
{
    if (0 == _moduleCount) return;

    uint8_t idx = (pin & 0xF8) >> 3;
    uint8_t msk = (0x01 << (pin & 0x07));
    // if(idx < _moduleCount) return;
    if (value > 0) {
        _outputBuffer[idx] |= msk;
    } else {
        _outputBuffer[idx] &= ~msk;
    }
    if (refresh)
        update();
}

void MFOutputShifter::setPins(char *pins, uint8_t value)
{
    if (0 == _moduleCount) return;

    char *pinTokens = strtok(pins, "|");
    while (pinTokens != 0) {
        uint8_t num = (uint8_t)atoi(pinTokens);
        setPin(num, value, 0);
        pinTokens = strtok(0, "|");
    }
    update();
}

void MFOutputShifter::test()
{
    for (uint8_t b = 0; b < _moduleCount * 8; b++) {
        uint8_t idx = (b & 0xF8) >> 3;
        uint8_t msk = (0x01 << (b & 0x07));
        _outputBuffer[idx] = msk;
        if (msk == 0x01 && idx != 0)
            _outputBuffer[idx - 1] = 0x00;
        update();
        delay(50);

        _outputBuffer[_moduleCount - 1] = 0;
        update();
    }
}
