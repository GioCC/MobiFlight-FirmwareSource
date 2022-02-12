// MFSegments.cpp
//
// Copyright (C) 2013-2014

#include "MFLCDDisplay.h"

MFLCDDisplay::MFLCDDisplay()
    : _address(0xFF) {}

void MFLCDDisplay::attach(byte address, byte cols, byte lines)
{
    if (address == 0xFF) return;
    _address = address;
    _cols    = cols;
    _lines   = lines;
    _lcdDisplay.init((uint8_t)address, (uint8_t)cols, (uint8_t)lines);
    _lcdDisplay.backlight();
    Wire.setClock(400000);
    test();
}

void MFLCDDisplay::setval(const char *string)
{
    if (_address == 0xFF) return;
    for (uint8_t line = 0; line != _lines; line++) {
        _lcdDisplay.setCursor(0, line);
        _lcdDisplay.writeString(&string[line * _cols], _cols);
    }
}

void MFLCDDisplay::detach()
{
    _lcdDisplay.clear();
    _lcdDisplay.noBacklight();
    _address = 0xFF;
}

void MFLCDDisplay::onReset(uint8_t action)
{
    if(action == ONRESET_DEFAULT) {
        test();
    }
}

void MFLCDDisplay::powerSave(bool state)
{
    if (state)
        _lcdDisplay.noBacklight();
    else
        _lcdDisplay.backlight();
}

void MFLCDDisplay::test()
{
    if (_address == 0xFF) return;
    uint8_t preLines = 0;
    _lcdDisplay.clear();

    if (_lines > 2) {
        preLines = (_lines / 2) - 1; // floor needs much Flash and for integer it's the same
    }

    _printCentered("MF LCD Display", preLines++);
    if (_lines > 1) {
        _printCentered("Test OK", preLines++);
    }

    _lcdDisplay.setCursor(0, 0);
}

void MFLCDDisplay::_printCentered(const char *str, uint8_t line)
{
    _lcdDisplay.setCursor(0, line);
    for (byte c = 0; c != ((_cols - strlen(str)) / 2); c++) {
        _lcdDisplay.print(F(" "));
    }
    _lcdDisplay.print(str);
    for (byte c = 0; c != ((_cols - strlen(str)) / 2); c++) {
        _lcdDisplay.print(F(" "));
    }
}