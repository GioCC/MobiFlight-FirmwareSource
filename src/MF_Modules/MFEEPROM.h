//
// MFEEPROM.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>
#include <EEPROM.h>

class MFEEPROM
{
private:
    uint16_t _pos;

public:
    MFEEPROM();
    void     init(void);
    uint16_t get_length(void) { return EEPROM.length(); };
    bool     setPosition(uint16_t pos = 0) { return ((pos < EEPROM.length()) ? ((_pos = pos), true) : false); };
    uint8_t  read_byte(uint16_t adr);
    bool     write_byte(uint16_t adr, const uint8_t data);

    bool read_block(uint16_t addr, char data[], uint16_t len);
    bool write_block(uint16_t addr, char data[], uint16_t len);
    bool read_block(char data[], uint16_t len) { return (read_block(_pos, data, len) ? (_pos += len, true) : false); }
    bool write_block(char data[], uint16_t len) { return (write_block(_pos, data, len) ? (_pos += len, true) : false); }

    template <typename T>
    bool read_var(uint16_t adr, T &t);
    template <typename T>
    bool read_var(uint16_t adr, T &t, uint16_t len);
    template <typename T>
    const bool write_var(uint16_t adr, const T &t);
    template <typename T>
    const bool write_var(uint16_t adr, const T &t, uint16_t len);

    byte read_byte(void) { return read_byte(_pos++); }
    bool write_byte(char data) { return (write_byte(_pos, data) ? (_pos++, true) : false); }
};

// Template definitions =================================================

template <typename T>
bool MFEEPROM::read_var(uint16_t adr, T &t)
{
    if (adr + sizeof(T) > EEPROM.length()) return false;
    EEPROM.get(adr, t);
    return true;
}

template <typename T>
bool MFEEPROM::read_var(uint16_t adr, T &t, uint16_t len)
{
    if (adr + len > EEPROM.length()) return false;
    uint8_t *ptr = (uint8_t *)&t;
    for (uint16_t i = 0; i < len; i++) {
        *ptr++ = EEPROM.read(adr + i);
    }
    return true;
}

template <typename T>
const bool MFEEPROM::write_var(uint16_t adr, const T &t)
{
    if (adr + sizeof(T) > EEPROM.length()) return false;
    EEPROM.put(adr, t);
#if defined(ARDUINO_ARCH_RP2040)
    EEPROM.commit();
#endif
    return true;
}

template <typename T>
const bool MFEEPROM::write_var(uint16_t adr, const T &t, uint16_t len)
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

// MFEEPROM.h