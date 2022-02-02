#ifndef _output_h
#define _output_h


void AddOutput(uint8_t pin, char const *name);
void ClearOutputs();
void OnSetPin();
void PowerSaveOutputs(bool state);


#endif