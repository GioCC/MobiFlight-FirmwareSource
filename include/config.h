#ifndef _config_h
#define _config_h

enum
{
  kTypeNotSet,              // 0
  kTypeButton,              // 1
  kTypeEncoderSingleDetent, // 2 (retained for backwards compatibility, use kTypeEncoder for new configs)
  kTypeOutput,              // 3
  kTypeLedSegment,          // 4
  kTypeStepperDeprecated,   // 5 (keep for backwards compatibility, doesn't support autohome)
  kTypeServo,               // 6
  kTypeLcdDisplayI2C,       // 7
  kTypeEncoder,             // 8
  kTypeStepper,             // 9 (new stepper type with auto zero support if btnPin is > 0)
  kShiftRegister,           // 10 Shift register support (example: 74HC595, TLC592X)
  kTypeAnalogInput,         // 11 Analog Device with 1 pin
  kTypeInputShifter         // 12 Input shift register support (example: 74HC165)
};

void eepromInit();

void OnSetConfig();
void OnResetConfig();
void OnSaveConfig();
void OnActivateConfig();
void loadConfig();
void OnGetConfig();
void OnGetInfo();
bool getStatusConfig();
void generateSerial(bool);
void OnGenNewSerial();
void OnSetName();
void _restoreName();

#endif
