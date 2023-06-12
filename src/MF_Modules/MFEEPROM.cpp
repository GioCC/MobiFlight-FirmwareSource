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
    : _pos(0) {}

void MFEEPROM::init(void)
{
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.begin(EEPROM_SIZE);
#endif
}

bool MFEEPROM::read_block(uint16_t adr, char data[], uint16_t len)
{
    uint16_t i;
    // If length is exceeded, return only the legitimate part
    for (i = 0; i < len && adr < EEPROM.length(); i++, adr++) {
        data[i] = read_byte(adr);
    }
    return (adr < EEPROM.length()) || (i == len);
}

bool MFEEPROM::write_block(uint16_t adr, char data[], uint16_t len)
{
    // If length is exceeded, do not write anything
    if (adr + len > EEPROM.length()) return false;
    for (uint16_t i = 0; i < len; i++, adr++) {
        EEPROM.put(adr, data[i]);
    }
    return true;
}

uint8_t MFEEPROM::read_byte(uint16_t adr)
{
    if (adr >= EEPROM.length()) return 0;
    return EEPROM.read(adr);
}

bool MFEEPROM::write_byte(uint16_t adr, const uint8_t data)
{
    if (adr >= EEPROM.length()) return false;
    EEPROM.write(adr, data);
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.commit();
#endif
    return true;
}
