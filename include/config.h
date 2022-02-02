#ifndef _config_h
#define _config_h

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
