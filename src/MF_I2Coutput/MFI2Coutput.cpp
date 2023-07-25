//
// MFI2Coutput.cpp
//
// (C) MobiFlight Project 2023
//

#include "MFI2Coutput.h"

void MFI2Coutput::display(const char *str)
{
    if (!_initialized) return;
    uint8_t cnt    = 0;
    uint8_t bufCnt = 0;
    Wire.beginTransmission(_address);
    // <str> is a fixed string of size <_lines*_cols>
    // while (*str) { // fixed-len, not terminated!
    while (cnt++ < _size) {
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

void MFI2Coutput::attach(byte address, byte cols, byte lines)
{
    _address     = address;
    _size        = cols * lines;
    _initialized = true;
    Wire.setClock(400000);
    test();
}

// MFI2Coutput.cpp
