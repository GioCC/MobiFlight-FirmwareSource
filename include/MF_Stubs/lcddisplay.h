#ifndef _lcddisplay_h
#define _lcddisplay_h


// ==== Inherited ====  (-> MF_device.cpp)
// - 
void MFLCDDisplay::clear() {}   // This might be useful to add
// - 
void MFLCDDisplay::powerSave(bool state) {}
// ClearLcdDisplays
void MFLCDDisplay::detach();

// ==== Specialized ====
// setup
void AddLcdDisplay(uint8_t address, uint8_t cols, uint8_t lines, char const *name);
// setval
void OnSetLcdDisplayI2C();

// ==== Additional ====
// -

#endif