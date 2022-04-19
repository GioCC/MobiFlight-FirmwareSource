//
// MFMCP23017.h
//
// (C) MobiFlight Project 2022
//

#pragma once

#include <stdint.h>

//! TODO: Test the added / modified functions:
// - ReadRegisterWord(...)
// - WriteRegisterWord(...)
// - pinMode(uint16_t mask);
// - pullUp(uint16_t mask);

class MFMCP23017
{
public:
    MFMCP23017(void)
        : GPIObuf(0){};

    void setAddr(uint8_t addr) { i2caddr = (addr > 7 ? 7 : addr); }
    void begin(uint8_t addr);
    void begin();

    static uint8_t checkConnection(uint8_t addr);
    uint8_t        checkConnection(void) { return checkConnection(i2caddr); }

    void pinMode(uint8_t p, uint8_t d);
    void pinMode(uint16_t mask); // Added by GCC
    void pullUp(uint8_t p, uint8_t d);
    void pullUp(uint16_t mask); // Added by GCC

    // Single-pin R/W
    void     digitalWrite(uint8_t p, uint8_t d);
    uint8_t  digitalRead(uint8_t p);

    // Port/2Port R/W
    uint8_t  readGPIO(uint8_t b);
    uint16_t readGPIOAB();
    void     writeGPIOAB(uint16_t);

#if !defined(MF_REDUCE_FUNCT_MCP23017)
    void     setupInterrupts(uint8_t mirroring, uint8_t open, uint8_t polarity);
    void     setupInterruptPin(uint8_t pin, uint8_t mode);
    void     disableInterruptPin(uint8_t pin);
    uint8_t  getLastInterruptPin();
    uint8_t  getLastInterruptPinValue();
#endif

    uint8_t  readRegister(uint8_t addr);                 // Made public by GCC
    void     writeRegister(uint8_t addr, uint8_t value); // Made public by GCC

    uint16_t readRegisterWord(uint8_t addr);                  // Added by GCC
    void     writeRegisterWord(uint8_t addr, uint16_t value); // Added by GCC

private:
    uint8_t  i2caddr;
    uint16_t GPIObuf;

    // Helper functions
    uint8_t bitForPin(uint8_t pin) { return pin % 8; }                   // Bit number associated to a given Pin
    uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr) // Port associated to a given Pin
        { return (pin < 8) ? portAaddr : portBaddr;  }

    uint16_t _read(uint8_t isWord, uint8_t addr);
    void     _write(uint8_t isWord, uint8_t addr, uint16_t val);
    void     _updateRegBit(uint8_t p, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);
};

#define MCP23017_ADDRESS 0x20 // MCP23017 Address

// Constants for register addressing
#define MCP23017_IODIRA   0x00 // I/O direction register A
#define MCP23017_IPOLA    0x02 // Input polarity port register A
#define MCP23017_GPINTENA 0x04 // Interrupt-on-change pins A
#define MCP23017_DEFVALA  0x06 // Default value register A
#define MCP23017_INTCONA  0x08 // Interrupt-on-change control register A
#define MCP23017_IOCONA   0x0A // I/O expander configuration register A
#define MCP23017_GPPUA    0x0C // GPIO pull-up resistor register A
#define MCP23017_INTFA    0x0E // Interrupt flag register A
#define MCP23017_INTCAPA  0x10 // Interrupt captured value for port register A
#define MCP23017_GPIOA    0x12 // General purpose I/O port register A
#define MCP23017_OLATA    0x14 // Output latch register 0 A

#define MCP23017_IODIRB   0x01 // I/O direction register B
#define MCP23017_IPOLB    0x03 // Input polarity port register B
#define MCP23017_GPINTENB 0x05 // Interrupt-on-change pins B
#define MCP23017_DEFVALB  0x07 // Default value register B
#define MCP23017_INTCONB  0x09 // Interrupt-on-change control register B
#define MCP23017_IOCONB   0x0B // I/O expander configuration register B
#define MCP23017_GPPUB    0x0D // GPIO pull-up resistor register B
#define MCP23017_INTFB    0x0F // Interrupt flag register B
#define MCP23017_INTCAPB  0x11 // Interrupt captured value for port register B
#define MCP23017_GPIOB    0x13 // General purpose I/O port register B
#define MCP23017_OLATB    0x15 // Output latch register 0 B

#define MCP23017_INT_ERR 255 // Interrupt error

// MFMCP23017.h
