//
// MFLCDDisplay.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFLCDDisplay.h"

MFLCDDisplay::MFLCDDisplay()
{
    _initialized = false;
}

void MFLCDDisplay::_sendStr(const char *str, uint8_t len)
{
    uint8_t cnt    = 0;
    uint8_t bufCnt = 0;
    Wire.beginTransmission(_address);
    // while (*str) { // fixed-len, not terminated!
    while (cnt++ < len) {
        Wire.write(*str++);
        bufCnt++;
        if (bufCnt >= (BUFFER_LENGTH >> 2)) { // BUFFER_LENGTH defined in Wire.h
            Wire.endTransmission(false);
            Wire.beginTransmission(_address);
            bufCnt = 0;
        }
    }
    Wire.endTransmission();
}

void MFLCDDisplay::display(const char *str)
{
    // <str> is a fixed string of size <_lines*_cols>
    if (!_initialized) return;
    if (_isLCD()) {
        for (uint8_t line = 0; line != _lines; line++) {
            _lcdDisplay.setCursor(0, line);
            _lcdDisplay.writeString(&str[line * _cols], _cols);
        }
    } else {
        _sendStr(str, _lines * _cols);
    }
}

void MFLCDDisplay::attach(byte address, byte cols, byte lines)
{
    _address     = address;
    _cols        = cols;
    _lines       = lines;
    _initialized = true;
    if (_isLCD()) {
        _lcdDisplay.init((uint8_t)address, (uint8_t)cols, (uint8_t)lines);
        _lcdDisplay.backlight();
    }
    Wire.setClock(400000);
    test();
}

void MFLCDDisplay::detach()
{
    _initialized = false;
}

void MFLCDDisplay::powerSavingMode(bool state)
{
    if (!_isLCD()) return;
    if (state)
        _lcdDisplay.noBacklight();
    else
        _lcdDisplay.backlight();
}

void MFLCDDisplay::test()
{
    if (!_initialized || !_isLCD()) return;
    uint8_t preLines = 0;
    _lcdDisplay.clear();

    if (_lines > 2) {
        preLines = (_lines / 2) - 1; // floor needs much Flash and for integer it's the same
    }

    _printCentered("MobiFlight", preLines++);
    if (_lines > 1) {
        _printCentered("Rocks!", preLines++);
    }

    _lcdDisplay.setCursor(0, 0);
}

void MFLCDDisplay::_printCentered(const char *str, uint8_t line)
{
    if (!_isLCD()) return;
    uint8_t startCol  = 0;
    uint8_t printChar = _cols;

    if (_cols > strlen(str)) {
        startCol  = (_cols - strlen(str)) / 2;
        printChar = strlen(str);
    }

    _lcdDisplay.setCursor(startCol, line);

    for (uint8_t i = 0; i < printChar; i++) {
        _lcdDisplay.write(str[i]);
    }
}

bool MFLCDDisplay::_isLCD(void)
{
    uint8_t a = _address & 0x07;
    return (a == 0x20 || a == 0x38);
}

// MFLCDDisplay.cpp
