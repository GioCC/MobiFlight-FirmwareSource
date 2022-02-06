#ifndef _stepper_h
#define _stepper_h


// ==== Inherited ====  (-> MF_device.cpp)
// - 
void MFStepper::clear() {}
// - 
void MFStepper::powerSave(bool state) {}
// ClearSteppers
void MFStepper::detach();

// ==== Specialized ====
// setup
void AddStepper(int pin1, int pin2, int pin3, int pin4, int btnPin1);
// setval
void OnSetStepper();

// ==== Additional ====
void OnResetStepper();
void OnSetZeroStepper();
void updateSteppers();


#endif