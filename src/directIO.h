//
// directIO.h
//
// (C) MobiFlight Project 2023
//

#pragma once

#ifdef ARDUINO_ARCH_AVR

#include <Arduino.h>

void directPinMode(uint8_t port, uint8_t pinMsk, uint8_t mode);
void directPinOut(uint8_t port, uint8_t pinMsk, uint8_t val);
int  directPinin(uint8_t port, uint8_t pinMsk);

#endif

// directIO.h
