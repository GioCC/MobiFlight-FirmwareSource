//
// InputHub.cpp
//

#include "inputHub.h"
#include <Arduino.h>

#define MF_BUTTON_DEBOUNCE_MS 10     // time between updating the buttons
#define MF_ANALOGAVERAGE_DELAY_MS 10 // time between updating the analog average calculation
#define MF_ANALOGREAD_DELAY_MS 50    // time between sending analog values

// =============================================
//  Variables
// =============================================

uint32_t lastButtonUpdate = 0;
uint32_t lastEncoderUpdate = 0;

#if MF_ANALOG_SUPPORT == 1
uint32_t lastAnalogAverage = 0;
uint32_t lastAnalogRead = 0;
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
uint32_t lastInputShifterUpdate = 0;
#endif

#if MF_DIGIN_MUX_SUPPORT == 1
uint32_t lastDigInMuxUpdate = 0;
#endif

MFButton *buttons[MAX_BUTTONS];
uint8_t buttonsRegistered = 0;

MFEncoder *encoders[MAX_ENCODERS];
uint8_t encodersRegistered = 0;

#if MF_ANALOG_SUPPORT == 1
MFAnalog *analog[MAX_ANALOG_INPUTS];
uint8_t analogRegistered = 0;
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
MFInputShifter *inputShifters[MAX_INPUT_SHIFTERS];
uint8_t inputShiftersRegistered = 0;
#endif

#if MF_MUX_SUPPORT == 1
MFMuxDriver MUX;
#endif

#if MF_DIGIN_MUX_SUPPORT == 1
MFDigInMux digInMux[MAX_DIGIN_MUX];
uint8_t digInMuxRegistered = 0;
#endif

// =============================================
//  General functions
// =============================================

// ---------------------------------------------------
//  Callbacks that define what commands we issue upon internal events
// ---------------------------------------------------
void attachEventCallbacks()
{
    MFButton::attachHandler(handlerOnButton);
    MFEncoder::attachHandler(handlerOnEncoder);
#if MF_ANALOG_SUPPORT == 1
    MFAnalog::attachHandler(handlerOnAnalogChange);
#endif
#ifdef MF_INPUT_SHIFTER_SUPPORT
    MFInputShifter::attachHandler(handlerInputShifterOnChange);
#endif
#if MF_DIGIN_MUX_SUPPORT == 1
    MFDigInMux::setMux(&MUX);
    MFDigInMux::attachHandler(handlerDigInMuxOnChange);
#endif
}

void handlerOnButton(uint8_t eventId, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kButtonChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};

void handlerOnEncoder(uint8_t eventId, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kEncoderChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};

#if MF_ANALOG_SUPPORT == 1
void handlerOnAnalogChange(int value, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kAnalogChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(value);
    cmdMessenger.sendCmdEnd();
};
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
void handlerInputShifterOnChange(uint8_t eventId, uint8_t pin, const char *name)
{
    cmdMessenger.sendCmdStart(kInputShifterChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(pin);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};
#endif

#if MF_DIGIN_MUX_SUPPORT == 1
void handlerDigInMuxOnChange(uint8_t eventId, uint8_t channel, const char *name)
{
    cmdMessenger.sendCmdStart(kDigInMuxChange);
    cmdMessenger.sendCmdArg(name);
    cmdMessenger.sendCmdArg(channel);
    cmdMessenger.sendCmdArg(eventId);
    cmdMessenger.sendCmdEnd();
};
#endif

void initInputTimings(uint32_t time)
{
    // Init Time Gap between Inputs, do not read at the same loop
#if MF_DIGIN_MUX_SUPPORT == 1
    lastDigInMuxUpdate = time + 8;
#endif
#if MF_INPUT_SHIFTER_SUPPORT == 1
    lastInputShifterUpdate = time + 6;
#endif
#if MF_ANALOG_SUPPORT == 1
    lastAnalogAverage = time + 4;
    lastAnalogRead    = time + 4;
#endif
    lastButtonUpdate  = time;
    lastEncoderUpdate = time + 2;
}

void OnTrigger()
{
    // Trigger all button release events first...
    for (int i = 0; i != buttonsRegistered; i++) {
        buttons[i]->triggerOnRelease();
    }
    // ... then trigger all the press events
    for (int i = 0; i != buttonsRegistered; i++) {
        buttons[i]->triggerOnPress();
    }

// Retrigger all the input shifters. This automatically sends
// the release events first followed by press events.
#if MF_INPUT_SHIFTER_SUPPORT == 1
    for (int i = 0; i != inputShiftersRegistered; i++) {
        inputShifters[i]->retrigger();
    }
#endif

// Retrigger all the DigInMux devices. This automatically sends
// the release events first followed by press events.
#if MF_DIGIN_MUX_SUPPORT == 1
    for (int i = 0; i != digInMuxRegistered; i++) {
        digInMux[i].retrigger();
    }
#endif
}

// ---------------------------------------------------
//  Button
// ---------------------------------------------------
void AddButton(uint8_t pin = 1, char const *name)
{
    if (buttonsRegistered == MAX_BUTTONS)
        return;
    if (!FitInMemory(sizeof(MFButton))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("Button does not fit in Memory"));
        return;
    }
    buttons[buttonsRegistered] = new (allocateMemory(sizeof(MFButton))) MFButton(pin, name);
    buttonsRegistered++;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added button ") /* + name */);
#endif
}

void ClearButtons()
{
    buttonsRegistered = 0;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Cleared buttons"));
#endif
}

void readButtons()
{
    if (millis() - lastButtonUpdate < MF_BUTTON_DEBOUNCE_MS)
        return;
    lastButtonUpdate = millis();
    for (int i = 0; i != buttonsRegistered; i++) {
        buttons[i]->update();
    }
}

// ---------------------------------------------------
//  Encoder
// ---------------------------------------------------
void AddEncoder(uint8_t pin1 = 1, uint8_t pin2 = 2, uint8_t encoder_type = 0, char const *name)
{
    if (encodersRegistered == MAX_ENCODERS)
        return;

    if (!FitInMemory(sizeof(MFEncoder))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("Encoders does not fit in Memory"));
        return;
    }
    encoders[encodersRegistered] = new (allocateMemory(sizeof(MFEncoder))) MFEncoder;
    encoders[encodersRegistered]->attach(pin1, pin2, encoder_type, name);
    encodersRegistered++;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added encoder"));
#endif
}

void ClearEncoders()
{
    encodersRegistered = 0;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Cleared encoders"));
#endif
}

void readEncoder()
{
    if (millis() - lastEncoderUpdate < 1)
        return;
    lastEncoderUpdate = millis();
    for (int i = 0; i != encodersRegistered; i++) {
        encoders[i]->update();
    }
}

// ---------------------------------------------------
//  Analog
// ---------------------------------------------------
#if MF_ANALOG_SUPPORT == 1
void AddAnalog(uint8_t pin = 1, char const *name, uint8_t sensitivity)
{
    if (analogRegistered == MAX_ANALOG_INPUTS)
        return;
    if (!FitInMemory(sizeof(MFAnalog))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("AnalogIn does not fit in Memory"));
        return;
    }
    analog[analogRegistered] = new (allocateMemory(sizeof(MFAnalog))) MFAnalog(pin, name, sensitivity);
    analogRegistered++;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added analog device "));
#endif
}

void ClearAnalog()
{
    analogRegistered = 0;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Cleared analog devices"));
#endif
}

void readAnalog()
{
    if (millis() - lastAnalogAverage > MF_ANALOGAVERAGE_DELAY_MS - 1) {
        for (int i = 0; i != analogRegistered; i++) {
            analog[i]->readBuffer();
        }
        lastAnalogAverage = millis();
    }
    if (millis() - lastAnalogRead < MF_ANALOGREAD_DELAY_MS)
        return;
    lastAnalogRead = millis();
    for (int i = 0; i != analogRegistered; i++) {
        analog[i]->update();
    }
}
#endif

// ---------------------------------------------------
//  ShiftReg
// ---------------------------------------------------
#if MF_INPUT_SHIFTER_SUPPORT == 1
void AddInputShifter(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name)
{
    if (inputShiftersRegistered == MAX_INPUT_SHIFTERS)
        return;
    if (!FitInMemory(sizeof(MFInputShifter))) {
        // Error Message to Connector
        cmdMessenger.sendCmd(kStatus, F("InputShifter does not fit in Memory"));
        return;
    }
    inputShifters[inputShiftersRegistered] = new (allocateMemory(sizeof(MFInputShifter))) MFInputShifter;
    inputShifters[inputShiftersRegistered]->attach(latchPin, clockPin, dataPin, modules, name);
    inputShifters[inputShiftersRegistered]->clear();
    inputShiftersRegistered++;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added input shifter"));
#endif
}

void ClearInputShifters()
{
    for (int i = 0; i < inputShiftersRegistered; i++) {
        inputShifters[i]->detach();
    }
    inputShiftersRegistered = 0;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Cleared input shifter"));
#endif
}

void readInputShifters()
{
    if (millis() - lastInputShifterUpdate <= MF_BUTTON_DEBOUNCE_MS)
        return;
    lastInputShifterUpdate = millis();

    for (int i = 0; i != inputShiftersRegistered; i++) {
        inputShifters[i]->update();
    }
}
#endif

// ---------------------------------------------------
//  Digital Input Mux
// ---------------------------------------------------
#if MF_DIGIN_MUX_SUPPORT == 1
void AddDigInMux(uint8_t dataPin, uint8_t nRegs, char const *name, bool mode)
{
    if (digInMuxRegistered == MAX_DIGIN_MUX)
        return;
    MFDigInMux *DIMUX = &digInMux[digInMuxRegistered];
    DIMUX->attach(dataPin, (nRegs == 1), name);
    DIMUX->clear();
    DIMUX->setLazyMode(mode == MFDigInMux::MUX_MODE_LAZY);
    DIMUX->attachHandler(handlerDigInMuxOnChange);
    digInMuxRegistered++;

#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added digital input MUX"));
#endif
}

void ClearDigInMux()
{
    for (int i = 0; i < digInMuxRegistered; i++) {
        digInMux[digInMuxRegistered].detach();
    }
    digInMuxRegistered = 0;
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Cleared digital input MUX"));
#endif
}

void readDigInMux()
{
    if (millis() - lastDigInMuxUpdate <= MF_BUTTON_DEBOUNCE_MS)
        return;
    lastDigInMuxUpdate = millis();

    for (int i = 0; i != digInMuxRegistered; i++) {
        digInMux[i].update();
    }
}
#endif

#if MF_MUX_SUPPORT == 1
void AddMultiplexer(uint8_t Sel0Pin, uint8_t Sel1Pin, uint8_t Sel2Pin, uint8_t Sel3Pin)
{
    MUX.attach(Sel0Pin, Sel1Pin, Sel2Pin, Sel3Pin);
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Added multiplexer"));
#endif
}
//// MULTIPLEXER SELECTOR /////
void ClearMultiplexer()
{
    MUX.detach();
#ifdef DEBUG
    cmdMessenger.sendCmd(kStatus, F("Cleared Multiplexer selector"));
#endif
}
#endif
// inputHub.cpp
