#ifndef _servo_h
#define _servo_h
#include"MFServo.h"

// ==== Inherited ====  (-> MF_device.cpp)
//  -
void clear() {}
//  -
void powerSave(bool state) {}
// ClearServos
void MFServo::detach();

// ==== Specialized ====
// setup
void AddServo(int pin);
// setval
void OnSetServo();

// ==== Additional ====
void updateServos();

#endif