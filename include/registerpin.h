#ifndef _registerpin_h
#define _registerpin_h

// ************************************************************
// decide if needed for future use
// ************************************************************
bool isPinRegistered(uint8_t pin);
bool isPinRegisteredForType(uint8_t pin, uint8_t type);
void registerPin(uint8_t pin, uint8_t type);
void clearRegisteredPins(uint8_t type);
void clearRegisteredPins();

#endif
