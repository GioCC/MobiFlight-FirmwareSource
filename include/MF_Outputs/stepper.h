#ifndef _stepper_h
#define _stepper_h


void AddStepper(int pin1, int pin2, int pin3, int pin4, int btnPin1);
void ClearSteppers();
void OnSetStepper();
void OnResetStepper();
void OnSetZeroStepper();
void updateSteppers();



#endif