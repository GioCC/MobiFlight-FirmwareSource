//
// MFEEPROM.cpp
//
// (C) MobiFlight Project 2022
//

// WARNING: This code is based on Arduino / Atmel AVR libraries (although itself not AVR-specific).
// When compiling for other platforms, check that the available libraries are compatible.

#include <Arduino.h>
#include "MFBoards.h"
#include "MFEEPROM.h"
#include "MFBoards.h"

MFEEPROM::MFEEPROM() 
{}

void MFEEPROM::init(void)
{
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.begin(EEPROM_SIZE);
#endif
    _eepromLength = EEPROM.length();
}

bool MFEEPROM::read_block(uint16_t adr, char data[], uint16_t len) {
    uint16_t i;
    // If length is exceeded, return only the legitimate part
    for (i = 0; i < len && adr < EEPROM.length(); i++, adr++) {
        data[i] = read_char(adr);
    }
    return (adr < EEPROM.length())||(i == len); 
}

uint8_t MFEEPROM::read_byte(uint16_t adr)
{
    if (adr >= _eepromLength) return 0;
    return EEPROM.read(adr);
}

bool MFEEPROM::write_byte(uint16_t adr, const uint8_t data)
{
    if (adr >= _eepromLength) return false;
    EEPROM.write(adr, data);
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.commit();
#endif
    return true;
}

bool MFEEPROM::setPosition(uint16_t pos)
{
    return ((pos < EEPROM.length()) ? ((_pos = pos), true) : false);
}
