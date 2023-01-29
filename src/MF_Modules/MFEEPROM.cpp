//
// MFEEPROM.cpp
//
// (C) MobiFlight Project 2022
//

#include <Arduino.h>
#include "MFEEPROM.h"
#include "MFBoards.h"

void MFEEPROM::init(void)
{
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.begin(EEPROM_SIZE);
#endif
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

// MFEEPROM.cpp