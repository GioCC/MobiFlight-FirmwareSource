//
// directIO.cpp
//
// (C) MobiFlight Project 2023
//

#include "directIO.h"

#ifdef ARDUINO_ARCH_AVR

void directPinMode(uint8_t port, uint8_t pinMsk, uint8_t mode)
{
    volatile uint8_t *reg, *out;
    reg = portModeRegister(port);
    out = portOutputRegister(port);

    uint8_t oldSREG = SREG;
    cli();
    if (mode == INPUT_PULLUP) {
        *reg &= ~pinMsk;
        *out |= pinMsk;
    } else if (mode == OUTPUT) {
        *reg |= pinMsk;
    }
    SREG = oldSREG;
}

void directPinOut(uint8_t port, uint8_t pinMsk, uint8_t val)
{
    volatile uint8_t *out;
    out             = portOutputRegister(port);
    uint8_t oldSREG = SREG;
    cli();
    (val == LOW ? (*out &= ~pinMsk) : (*out |= pinMsk));
    SREG = oldSREG;
}

int directPinin(uint8_t port, uint8_t pinMsk)
{
    return ((*portInputRegister(port) & pinMsk) ? HIGH : LOW);
}

#endif

// directIO.cpp
