//
// MFMCP23017.cpp
//
// (C) MobiFlight Project 2022
//

// Library for the MCP23017 i2c port expander
// Based on Adafruit MCP23017 Library

#include "Arduino.h"
#include <avr/pgmspace.h>
#include <Wire.h>
#include "MCP23017.h"

// Reads an 8- or 16-bit register (helper function)
uint16_t MFMCP23017::
_read(uint8_t isWord, uint8_t addr)
{
    uint16_t res;
    Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
    Wire.write(addr);
    Wire.endTransmission();
    Wire.requestFrom(MCP23017_ADDRESS | i2caddr, (isWord ? 2 : 1));
    res = Wire.read();
    if (isWord) res |= ((uint16_t)Wire.read() << 8);
    return res;
}

// Reads an 8-bit register
uint8_t MFMCP23017::
readRegister(uint8_t addr)
{
    return (uint8_t)_read(false, addr);
}

// Reads a 16-bit register [pair]
uint16_t MFMCP23017::
readRegisterWord(uint8_t addr)
{
    return _read(true, addr);
}

// Writes an 8- or 16-bit register (helper function)
void MFMCP23017::
_write(uint8_t isWord, uint8_t addr, uint16_t val)
{
    Wire.beginTransmission(MCP23017_ADDRESS | i2caddr);
    Wire.write(addr);
    Wire.write((val & 0xFF));
    if (isWord) Wire.write(((val >> 8) & 0xFF));
    Wire.endTransmission();
}

// Writes an 8-bit register
void MFMCP23017::
writeRegister(uint8_t addr, uint8_t val)
{
    _write(false, addr, val);
}

// Writes a 16-bit register [pair]
void MFMCP23017::
writeRegisterWord(uint8_t addr, uint16_t val)
{
    _write(true, addr, val);
}

// Private helper method to update a single bit of an A/B register
// - Reads the current register value
// - updates the particular bit
// - Writes the new register value
void MFMCP23017::
_updateRegBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr)
{
    uint8_t regValue;
    uint8_t regAddr = regForPin(pin, portAaddr, portBaddr);
    uint8_t bit     = bitForPin(pin);

    regValue = readRegister(regAddr);
    bitWrite(regValue, bit, pValue);
    writeRegister(regAddr, regValue);
}

/*!
 * Initializes the MCP23017 given its HW selected address, see datasheet for
 * Address selection.
 * @param addr Selected address
 * @param theWire the I2C object to use, defaults to &Wire
 */
void MFMCP23017::
begin(uint8_t addr)
{
    i2caddr = (addr > 7 ? 7 : addr);

    Wire.begin();

    // set defaults: all inputs on both port A and port B
    writeRegister(MCP23017_IODIRA, 0xff);
    writeRegister(MCP23017_IODIRB, 0xff);

    // Turn off interrupt triggers
    writeRegister(MCP23017_GPINTENA, 0x00);
    writeRegister(MCP23017_GPINTENB, 0x00);

    // Turn off pull up resistors
    writeRegister(MCP23017_GPPUA, 0x00);
    writeRegister(MCP23017_GPPUB, 0x00);
}

/**
 * Initializes the default MCP23017, with 000 for the configurable part of the
 * address
 * @param theWire the I2C object to use, defaults to &Wire
 */
void MFMCP23017::
begin(void)
{
    begin(0);
}

/**
 * Tests if addressed unit is responding on the bus
 */
uint8_t
MFMCP23017::
checkConnection(uint8_t addr)
{
    uint8_t res = 0;
    Wire.begin();
    Wire.beginTransmission(MCP23017_ADDRESS | addr);
    return !(Wire.endTransmission());
}

/**
 * Sets the pin mode to either INPUT or OUTPUT
 * @param p Pin to set
 * @param d Mode to set the pin
 */
void MFMCP23017::
pinMode(uint8_t p, uint8_t d)
{
    _updateRegBit(p, (d == INPUT), MCP23017_IODIRA, MCP23017_IODIRB);
}

/**
 * Sets the pin mode to either INPUT or OUTPUT for all pins
 * @param mask Pin status pattern (1=In, 0=0ut)
 */
void MFMCP23017::
pinMode(uint16_t mask)
{
    writeRegisterWord(MCP23017_IODIRA, mask);
}

/*!
 * @brief Enables the pull-up resistor on the specified pin
 * @param p Pin to set
 * @param d Value to set the pin
 */
void MFMCP23017::
pullUp(uint8_t p, uint8_t d)
{
    _updateRegBit(p, d, MCP23017_GPPUA, MCP23017_GPPUB);
}

/*!
 * @brief Enables the pull-up resistor for specified pins
 * @param mask Pin pull-up pattern (1=active)
 */
void MFMCP23017::
pullUp(uint16_t mask)
{
    writeRegisterWord(MCP23017_GPPUA, mask);
}

/**
 * Read a single port, A or B, and return its current 8 bit value.
 * @param b Decided what gpio to use. Should be 0 for GPIOA, and 1 for GPIOB.
 * @return Returns the b bit value of the port
 */
uint8_t MFMCP23017::
readGPIO(uint8_t b)
{
    // read the current GPIO output latches
    return (uint8_t)_read(false, (b == 0 ? MCP23017_GPIOA : MCP23017_GPIOB));
}

/**
 * Reads all 16 pins (port A and B) into a single 16 bits variable.
 * @return Returns the 16 bit variable representing all 16 pins
 */
uint16_t MFMCP23017::
readGPIOAB()
{
    return _read(true, MCP23017_GPIOA);
}

/**
 * Writes all the pins in one go. This method is very useful if you are
 * implementing a multiplexed matrix and want to get a decent refresh rate.
 */
void MFMCP23017::
writeGPIOAB(uint16_t val)
{
    _write(true, MCP23017_GPIOA, val);
}

/*!
 * @brief Writes to a pin on the MCP23017
 * @param pin Pin to write to
 * @param d What to write to the pin
 */
void MFMCP23017::
digitalWrite(uint8_t pin, uint8_t d)
{

    // uint8_t gpio;
    uint8_t bit     = bitForPin(pin);
    uint8_t regAddr = regForPin(pin, MCP23017_OLATA, MCP23017_OLATB);

    // read the current GPIO output latches
    // (unnecessary if buffered)
    // gpio = readRegister(regAddr);

    // set the pin and direction
    bitWrite(GPIObuf, bit, d);

    // write the new GPIO
    regAddr = regForPin(pin, MCP23017_GPIOA, MCP23017_GPIOB);
    writeRegister(regAddr, GPIObuf);
}

/*!
 * @brief Reads the specified pin
 * @param pin Pin to read
 * @return Value of the pin
 */
uint8_t MFMCP23017::
digitalRead(uint8_t pin)
{
    uint8_t bit     = bitForPin(pin);
    uint8_t regAddr = regForPin(pin, MCP23017_GPIOA, MCP23017_GPIOB);
    return (readRegister(regAddr) >> bit) & 0x1;
}

#if !defined(MF_REDUCE_FUNCT_MCP23017)
/**
 * Configures the interrupt system. both port A and B are assigned the same
 * configuration.
 * @param mirroring Mirroring will OR both INTA and INTB pins.
 * @param openDrain Opendrain will set the INT pin to value or open drain.
 * @param polarity polarity will set LOW or HIGH on interrupt.
 * Default values after Power On Reset are: (false, false, LOW)
 * If you are connecting the INTA/B pin to arduino 2/3, you should configure the
 * interupt handling as FALLING with the default configuration.
 */
void MFMCP23017::
setupInterrupts(uint8_t mirroring, uint8_t openDrain, uint8_t polarity)
{
    // configure the port A
    uint8_t ioconfValue = readRegister(MCP23017_IOCONA);
    bitWrite(ioconfValue, 6, mirroring);
    bitWrite(ioconfValue, 2, openDrain);
    bitWrite(ioconfValue, 1, polarity);
    writeRegister(MCP23017_IOCONA, ioconfValue);

    // Configure the port B
    ioconfValue = readRegister(MCP23017_IOCONB);
    bitWrite(ioconfValue, 6, mirroring);
    bitWrite(ioconfValue, 2, openDrain);
    bitWrite(ioconfValue, 1, polarity);
    writeRegister(MCP23017_IOCONB, ioconfValue);
}

/**
 * Set's up a pin for interrupt. uses arduino MODEs: CHANGE, FALLING, RISING.
 *
 * Note that the interrupt condition finishes when you read the information
 * about the port / value that caused the interrupt or you read the port itself.
 * Check the datasheet can be confusing.
 * @param pin Pin to set
 * @param mode Mode to set the pin
 *
 */
void MFMCP23017::
setupInterruptPin(uint8_t pin, uint8_t mode)
{

    // set the pin interrupt control (0 means change, 1 means compare against
    // given value);
    _updateRegBit(pin, (mode != CHANGE), MCP23017_INTCONA, MCP23017_INTCONB);
    // if the mode is not CHANGE, we need to set up a default value, different
    // value triggers interrupt

    // In a RISING interrupt the default value is 0, interrupt is triggered when
    // the pin goes to 1. In a FALLING interrupt the default value is 1, interrupt
    // is triggered when pin goes to 0.
    _updateRegBit(pin, (mode == FALLING), MCP23017_DEFVALA, MCP23017_DEFVALB);

    // enable the pin for interrupt
    _updateRegBit(pin, HIGH, MCP23017_GPINTENA, MCP23017_GPINTENB);
}

/**
 * Disable a pin for interrupt.
 *
 * @param pin Pin to set
 *
 */
void MFMCP23017::
disableInterruptPin(uint8_t pin)
{
    // disable the pin for interrupt
    _updateRegBit(pin, LOW, MCP23017_GPINTENA, MCP23017_GPINTENB);
}

/*!
 * @brief Gets the last interrupt pin
 * @return Returns the last interrupt pin
 */
uint8_t MFMCP23017::
getLastInterruptPin()
{
    uint8_t intf;

    // try port A
    intf = readRegister(MCP23017_INTFA);
    for (int i = 0; i < 8; i++)
        if (bitRead(intf, i))
            return i;

    // try port B
    intf = readRegister(MCP23017_INTFB);
    for (int i = 0; i < 8; i++)
        if (bitRead(intf, i))
            return i + 8;

    return MCP23017_INT_ERR;
}
/*!
 * @brief Gets the value of the last interrupt pin
 * @return Returns the value of the last interrupt pin
 */
uint8_t MFMCP23017::
getLastInterruptPinValue()
{
    uint8_t intPin = getLastInterruptPin();
    if (intPin != MCP23017_INT_ERR) {
        uint8_t intcapreg = regForPin(intPin, MCP23017_INTCAPA, MCP23017_INTCAPB);
        uint8_t bit       = bitForPin(intPin);
        return (readRegister(intcapreg) >> bit) & (0x01);
    }

    return MCP23017_INT_ERR;
}
#endif  // !defined(MF_REDUCE_FUNCT_MCP23017)

// MFMCP23017.cpp