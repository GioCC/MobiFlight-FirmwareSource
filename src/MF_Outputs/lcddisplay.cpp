#include <Arduino.h>
#include "MFBoards.h"
#include "commandmessenger.h"
#include "registerpin.h"
#include "MFLCDDisplay.h"


#if MF_LCD_SUPPORT == 1

MFLCDDisplay lcd_I2C[MAX_MFLCD_I2C];
uint8_t lcd_12cRegistered = 0;


void AddLcdDisplay(uint8_t address = 0x24, uint8_t cols = 16, uint8_t lines = 2, char const *name = "LCD")
{
  if (lcd_12cRegistered == MAX_MFLCD_I2C)
    return;
  registerPin(SDA, kTypeLcdDisplayI2C);
  registerPin(SCL, kTypeLcdDisplayI2C);

  lcd_I2C[lcd_12cRegistered].attach(address, cols, lines);
  lcd_12cRegistered++;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Added lcdDisplay"));
#endif
}

void ClearLcdDisplays()
{
  for (int i = 0; i != lcd_12cRegistered; i++)
  {
    lcd_I2C[lcd_12cRegistered].detach();
  }
  clearRegisteredPins(kTypeLcdDisplayI2C);
  lcd_12cRegistered = 0;
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Cleared lcdDisplays"));
#endif
}

void OnSetLcdDisplayI2C()
{
  int address = cmdMessenger.readInt16Arg();
  char *output = cmdMessenger.readStringArg();
  lcd_I2C[address].display(output);
  setLastCommandMillis(millis());
}

#endif