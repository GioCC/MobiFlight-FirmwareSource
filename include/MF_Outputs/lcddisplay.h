#ifndef _lcddisplay_h
#define _lcddisplay_h


void AddLcdDisplay(uint8_t address, uint8_t cols, uint8_t lines, char const *name);
void ClearLcdDisplays();
void OnSetLcdDisplayI2C();



#endif