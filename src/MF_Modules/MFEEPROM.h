//
// MFEEPROM.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <EEPROM.h>

class MFEEPROM
{

public:
    MFEEPROM(){};
    void init(void)
    {
#if defined(ARDUINO_ARCH_RP2040)
        EEPROM.begin(EEPROM_SIZE);
#endif
    }
    uint16_t get_length(void) { return EEPROM.length(); }
    uint8_t  read_byte(uint16_t adr) { return ((adr >= EEPROM.length()) ? 0 : EEPROM.read(adr)); }
    bool     write_byte(uint16_t adr, const uint8_t data);

    template <typename T>
    bool read_block(uint16_t adr, T &t)
    {
        if (adr + sizeof(T) > EEPROM.length()) return false;
        EEPROM.get(adr, t);
        return true;
    }

    template <typename T>
    bool read_block(uint16_t adr, T &t, uint16_t len)
    {
        if (adr + len > EEPROM.length()) return false;
        uint8_t *ptr = (uint8_t *)&t;
        for (uint16_t i = 0; i < len; i++) {
            *ptr++ = EEPROM.read(adr + i);
        }
        return true;
    }

    template <typename T>
    const bool write_block(uint16_t adr, const T &t)
    {
        if (adr + sizeof(T) > EEPROM.length()) return false;
        EEPROM.put(adr, t);
#if defined(ARDUINO_ARCH_RP2040)
        EEPROM.commit();
#endif
        return true;
    }

    template <typename T>
    const bool write_block(uint16_t adr, const T &t, uint16_t len)
    {
        if (adr + len > EEPROM.length()) return false;
        for (uint16_t i = 0; i < len; i++) {
            EEPROM.put(adr + i, t[i]);
        }
#if defined(ARDUINO_ARCH_RP2040)
        EEPROM.commit();
#endif
        return true;
    }
};

// MFEEPROM.h