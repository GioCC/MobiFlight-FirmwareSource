#include <Arduino.h>
#include "MFBoards.h"
#include "config.h"
#include "MFEEPROM.h"
#include "commandmessenger.h"
#include "StowManager.h"
#include "inputHub.h"

#include "output.h"
#if MF_SEGMENT_SUPPORT == 1
#include "segment.h"
#endif
#if MF_STEPPER_SUPPORT == 1
#include "stepper.h"
#endif
#if MF_SERVO_SUPPORT == 1
#include "servos.h"
#endif
#if MF_LCD_SUPPORT == 1
#include "lcddisplay.h"
#endif
#if MF_OUTPUT_SHIFTER_SUPPORT == 1
#include "outputshifter.h"
#endif


// The build version comes from an environment variable
#define STRINGIZER(arg) #arg
#define STR_VALUE(arg) STRINGIZER(arg)
#define VERSION STR_VALUE(BUILD_VERSION)

MFEEPROM      MFeeprom;

const int     MEM_LEN_STOW = MEMLEN_CONFIG/2;   //TODO
uint8_t       stowBuf[MEM_LEN_STOW];
StowManager   Stowage(stowBuf, MEM_LEN_STOW);

const uint8_t MEM_OFFSET_NAME = 0;
const uint8_t MEM_LEN_NAME = 48;
const uint8_t MEM_OFFSET_SERIAL = MEM_OFFSET_NAME + MEM_LEN_NAME;
const uint8_t MEM_LEN_SERIAL = 11;
const uint8_t MEM_OFFSET_CONFIG = MEM_OFFSET_NAME + MEM_LEN_NAME + MEM_LEN_SERIAL;

const char    type[sizeof(MOBIFLIGHT_TYPE)] = MOBIFLIGHT_TYPE;
char          serial[MEM_LEN_SERIAL] = MOBIFLIGHT_SERIAL;
char          name[MEM_LEN_NAME] = MOBIFLIGHT_NAME;
//const int     MEM_LEN_CONFIG = MEMLEN_CONFIG;
const int     MEM_LEN_CONFIG = MEMLEN_CONFIG/2;   //TODO
char          configBuffer[MEM_LEN_CONFIG] = "";
uint16_t      configLength = 0;
boolean       configActivated = false;

void resetConfig();
void readConfig();
void _storeConfig();
void _activateConfig();

void eepromInit()
{
  MFeeprom.init();
}

// ************************************************************
// configBuffer handling
// ************************************************************
void loadConfig()
{
  resetConfig();
  MFeeprom.read_block(MEM_OFFSET_CONFIG, configBuffer, MEM_LEN_CONFIG);
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Restored config"));
  cmdMessenger.sendCmd(kStatus, configBuffer);
#endif
  configLength = strlen(configBuffer);
  readConfig();
  _activateConfig();
}

void _storeConfig()
{
  MFeeprom.write_block(MEM_OFFSET_CONFIG, configBuffer, MEM_LEN_CONFIG);
}

void OnSetConfig()
{
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Setting config start"));
#endif
  setLastCommandMillis(millis());
  char *cfg = cmdMessenger.readStringArg();
  uint8_t cfgLen = strlen(cfg);
  uint16_t bufferSize = MEM_LEN_CONFIG - (configLength + cfgLen);

  if (bufferSize > 1)
  {
    memcpy(&configBuffer[configLength], cfg, bufferSize);
    configLength += cfgLen;
    cmdMessenger.sendCmd(kStatus, configLength);
  }
  else
    cmdMessenger.sendCmd(kStatus, -1);
#ifdef DEBUG
  cmdMessenger.sendCmd(kStatus, F("Setting config end"));
#endif
}

void resetConfig()
{
  ClearDeviceConfig();  // This will do all devices

// #if MF_ANALOG_SUPPORT == 1
//   Analog::Clear();
// #endif
// #if MF_INPUT_SHIFTER_SUPPORT == 1
//   InputShifter::Clear();
// #endif

  ClearOutputs();       // As in "Digital output pins"
#if MF_SEGMENT_SUPPORT == 1
  ClearLedSegments();
#endif

#if MF_SERVO_SUPPORT == 1
  ClearServos();
#endif

#if MF_STEPPER_SUPPORT == 1
  ClearSteppers();
#endif

#if MF_LCD_SUPPORT == 1
  ClearLcdDisplays();
#endif

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
  ClearShifters();
#endif

  configLength = 0;
  configActivated = false;
}

void OnResetConfig()
{
  resetConfig();
  cmdMessenger.sendCmd(kStatus, F("OK"));
}

void OnSaveConfig()
{
  _storeConfig();
  cmdMessenger.sendCmd(kConfigSaved, F("OK"));
}

void OnActivateConfig()
{
  readConfig();
  _activateConfig();
}

void _activateConfig()
{
  configActivated = true;
  cmdMessenger.sendCmd(kConfigActivated, F("OK"));
}

void readConfig()
{
  if (configLength == 0) return;
  char *p = NULL;

  char *command = strtok_r(configBuffer, ".", &p);
  char *params[6];
  if (*command == 0)
    return;

  do
  {
    switch (atoi(command))
    {
    case kTypeButton:
      params[0] = strtok_r(NULL, ".", &p); // pin
      params[1] = strtok_r(NULL, ":", &p); // name
      //Button::Add(atoi(params[0]), params[1]);
      AddButton(atoi(params[0]), params[1]);
      
      break;

    case kTypeOutput:
      params[0] = strtok_r(NULL, ".", &p); // pin
      params[1] = strtok_r(NULL, ":", &p); // Name
      AddOutput(atoi(params[0]), params[1]);
      break;

#if MF_SEGMENT_SUPPORT == 1
    case kTypeLedSegment:
      params[0] = strtok_r(NULL, ".", &p); // pin Data
      params[1] = strtok_r(NULL, ".", &p); // pin Cs
      params[2] = strtok_r(NULL, ".", &p); // pin Clk
      params[3] = strtok_r(NULL, ".", &p); // brightness
      params[4] = strtok_r(NULL, ".", &p); // numModules
      params[5] = strtok_r(NULL, ":", &p); // Name
      AddLedSegment(atoi(params[0]), atoi(params[1]), atoi(params[2]), atoi(params[4]), atoi(params[3]));
#endif
      break;

#if MF_STEPPER_SUPPORT == 1
    case kTypeStepperDeprecated:
      // this is for backwards compatibility
      params[0] = strtok_r(NULL, ".", &p); // pin1
      params[1] = strtok_r(NULL, ".", &p); // pin2
      params[2] = strtok_r(NULL, ".", &p); // pin3
      params[3] = strtok_r(NULL, ".", &p); // pin4
      params[4] = strtok_r(NULL, ".", &p); // btnPin1
      params[5] = strtok_r(NULL, ":", &p); // Name
      AddStepper(atoi(params[0]), atoi(params[1]), atoi(params[2]), atoi(params[3]), 0);
#endif
      break;

#if MF_STEPPER_SUPPORT == 1
    case kTypeStepper:
      params[0] = strtok_r(NULL, ".", &p); // pin1
      params[1] = strtok_r(NULL, ".", &p); // pin2
      params[2] = strtok_r(NULL, ".", &p); // pin3
      params[3] = strtok_r(NULL, ".", &p); // pin4
      params[4] = strtok_r(NULL, ".", &p); // btnPin1
      params[5] = strtok_r(NULL, ":", &p); // Name
      AddStepper(atoi(params[0]), atoi(params[1]), atoi(params[2]), atoi(params[3]), atoi(params[4]));
#endif
      break;

#if MF_SERVO_SUPPORT == 1
    case kTypeServo:
      params[0] = strtok_r(NULL, ".", &p); // pin1
      params[1] = strtok_r(NULL, ":", &p); // Name
      AddServo(atoi(params[0]));
#endif
      break;

    case kTypeEncoderSingleDetent:
      params[0] = strtok_r(NULL, ".", &p); // pin1
      params[1] = strtok_r(NULL, ".", &p); // pin2
      params[2] = strtok_r(NULL, ":", &p); // Name
      //Encoder::Add(atoi(params[0]), atoi(params[1]), 0, params[2]);
      AddEncoder(atoi(params[0]), atoi(params[1]), 0, params[2]);
      break;

    case kTypeEncoder:
      params[0] = strtok_r(NULL, ".", &p); // pin1
      params[1] = strtok_r(NULL, ".", &p); // pin2
      params[2] = strtok_r(NULL, ".", &p); // encoder_type
      params[3] = strtok_r(NULL, ":", &p); // Name
      //Encoder::Add(atoi(params[0]), atoi(params[1]), atoi(params[2]), params[3]);
      AddEncoder(atoi(params[0]), atoi(params[1]), atoi(params[2]), params[3]);
      break;

#if MF_LCD_SUPPORT == 1
    case kTypeLcdDisplayI2C:
      params[0] = strtok_r(NULL, ".", &p); // address
      params[1] = strtok_r(NULL, ".", &p); // cols
      params[2] = strtok_r(NULL, ".", &p); // lines
      params[3] = strtok_r(NULL, ":", &p); // Name
      AddLcdDisplay(atoi(params[0]), atoi(params[1]), atoi(params[2]), params[3]);
#endif
      break;

#if MF_ANALOG_SUPPORT == 1
    case kTypeAnalogInput:
      params[0] = strtok_r(NULL, ".", &p); // pin
      params[1] = strtok_r(NULL, ".", &p); // sensitivity
      params[2] = strtok_r(NULL, ":", &p); // name
      //Analog::Add(atoi(params[0]), params[2], atoi(params[1]));
      AddAnalog(atoi(params[0]), atoi(params[1]), params[2]);
#endif
      break;

#if MF_OUTPUT_SHIFTER_SUPPORT == 1
    case kShiftRegister:
      params[0] = strtok_r(NULL, ".", &p); // pin latch
      params[1] = strtok_r(NULL, ".", &p); // pin clock
      params[2] = strtok_r(NULL, ".", &p); // pin data
      params[3] = strtok_r(NULL, ".", &p); // number of daisy chained modules
      params[4] = strtok_r(NULL, ":", &p); // name
      AddShifter(atoi(params[0]), atoi(params[1]), atoi(params[2]), atoi(params[3]), params[4]);
#endif
      break;

#if MF_INPUT_SHIFTER_SUPPORT == 1
    case kTypeInputShifter:
      params[0] = strtok_r(NULL, ".", &p); // pin latch
      params[1] = strtok_r(NULL, ".", &p); // pin clock
      params[2] = strtok_r(NULL, ".", &p); // pin data
      params[3] = strtok_r(NULL, ".", &p); // number of daisy chained modules
      params[4] = strtok_r(NULL, ":", &p); // name
      //InputShifter::Add(atoi(params[0]), atoi(params[1]), atoi(params[2]), atoi(params[3]), params[4]);
      AddInputShiftReg(atoi(params[0]), atoi(params[1]), atoi(params[2]), atoi(params[3]), params[4]);
      break;
#endif

    default:
      // read to the end of the current command which is apparently not understood
      params[0] = strtok_r(NULL, ":", &p); // read to end of unknown command
    }
    command = strtok_r(NULL, ".", &p);
  } while (command != NULL);
}

void OnGetConfig()
{
  setLastCommandMillis(millis());
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(MFeeprom.read_char(MEM_OFFSET_CONFIG));
  for (uint16_t i=1; i<configLength; i++) {
    cmdMessenger.sendArg(MFeeprom.read_char(MEM_OFFSET_CONFIG+i));
  }
  cmdMessenger.sendCmdEnd();
}

void OnGetInfo()
{
  setLastCommandMillis(millis());
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(type);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdArg(serial);
  cmdMessenger.sendCmdArg(VERSION);
  cmdMessenger.sendCmdEnd();
}

bool getStatusConfig()
{
  return configActivated;
}

// ************************************************************
// serial handling
// ************************************************************
void generateSerial(bool force)
{
  MFeeprom.read_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
  if (!force && serial[0] == 'S' && serial[1] == 'N')
    return;
  randomSeed(analogRead(0));
  sprintf(serial, "SN-%03x-", (unsigned int)random(4095));
  sprintf(&serial[7], "%03x", (unsigned int)random(4095));
  MFeeprom.write_block(MEM_OFFSET_SERIAL, serial, MEM_LEN_SERIAL);
  if (!force) {
    MFeeprom.write_byte(MEM_OFFSET_CONFIG, 0x00);           // First byte of config to 0x00 to ensure to start 1st time with empty config, but not if forced from the connector to generate a new one
    configBuffer[0] = '\0';
  }
}

void OnGenNewSerial()
{
  generateSerial(true);
  cmdMessenger.sendCmdStart(kInfo);
  cmdMessenger.sendCmdArg(serial);
  cmdMessenger.sendCmdEnd();
}

// ************************************************************
// Naming handling
// ************************************************************
void _storeName()
{
  char prefix[] = "#";
  MFeeprom.write_block(MEM_OFFSET_NAME, prefix, 1);
  MFeeprom.write_block(MEM_OFFSET_NAME + 1, name, MEM_LEN_NAME - 1);
}

void _restoreName()
{
  char testHasName[1] = "";
  MFeeprom.read_block(MEM_OFFSET_NAME, testHasName, 1);
  if (testHasName[0] != '#')
    return;

  MFeeprom.read_block(MEM_OFFSET_NAME + 1, name, MEM_LEN_NAME - 1);
}

void OnSetName()
{
  char *cfg = cmdMessenger.readStringArg();
  memcpy(name, cfg, MEM_LEN_NAME);
  _storeName();
  cmdMessenger.sendCmdStart(kStatus);
  cmdMessenger.sendCmdArg(name);
  cmdMessenger.sendCmdEnd();
}

