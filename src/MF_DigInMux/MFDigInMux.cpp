//
// MFDigInMux.cpp
//
// (C) MobiFlight Project 2022
//

#include "mobiflight.h"
#include "MFDigInMux.h"
#include "MFMuxDriver.h"

MFMuxDriver *MFDigInMux::_MUX;

MuxDigInEvent MFDigInMux::_inputHandler = NULL;

MFDigInMux::MFDigInMux(void)
{
    _MUX   = NULL;
    _name  = "MUXDigIn";
    _flags = 0x00;
    clear();
}

MFDigInMux::MFDigInMux(MFMuxDriver *MUX, const char *name)
    : _name(name)
{
    if (MUX) _MUX = MUX;
    _flags = 0x00;
    clear();
}

void MFDigInMux::setMux(MFMuxDriver *MUX)
{
    _MUX = MUX;
}

// Registers a new MUX input block and configures the driver pins
void MFDigInMux::attach(uint8_t dataPin, bool halfSize, char const *name)
{
    // if(!_MUX) return;     // no need to check, the object can be set up in advance before the MUX is configured
    _dataPin = dataPin;
    _name    = name;
    _flags   = 0x00;
    if (halfSize) bitSet(_flags, MUX_HALFSIZE);
    pinMode(_dataPin, INPUT_PULLUP);
    bitSet(_flags, MUX_INITED);

    // Initialize all inputs with current status
    poll(DONT_TRIGGER);
}

void MFDigInMux::detach()
{
    if (bitRead(_flags, MUX_INITED)) {
        pinMode(_dataPin, INPUT_PULLUP);
        bitClear(_flags, MUX_INITED);
    }
}

// Reads the values from the attached modules, compares them to the previously
// read values, and calls the registered event handler for any inputs that
// changed from the previously read state.
void MFDigInMux::update()
{
    poll(DO_TRIGGER);
}

// Helper function for update() and retrigger()
void MFDigInMux::poll(bool doTrigger)
{
    if (!_MUX) return;

    uint8_t  selMax       = (bitRead(_flags, MUX_HALFSIZE) ? 8 : 16);
    uint16_t currentState = 0x0000;
    uint8_t  pinVal;

    for (uint8_t sel = selMax; sel > 0; sel--) {
        pinVal = _MUX->readChannel(sel - 1, _dataPin);
        currentState <<= 1;
        currentState |= (pinVal ? 1 : 0);
    }

    if (_lastState != currentState) {
        if (doTrigger) detectChanges(_lastState, currentState);
        _lastState = currentState;
    }
}

// Detects changes between the current state and the previously saved state
void MFDigInMux::detectChanges(uint16_t lastState, uint16_t currentState)
{
    if (!_MUX) return;
    uint8_t  selMax = (bitRead(_flags, MUX_HALFSIZE) ? 8 : 16);
    uint16_t diff   = lastState ^ currentState;
    for (uint8_t i = 0; i < selMax; i++) {
        if (diff & 0x0001) {
            trigger(i, ((currentState & 0x0001) != 0));
        }
        diff >>= 1;
        currentState >>= 1;
    }
}

// Reads the current state for all connected modules then fires
// 'release' events for every 'off' input, followed by
// 'press' events for every 'on' input.
// (Remember that 'off' inputs actually have physical status '1')
void MFDigInMux::retrigger()
{
    // The current state for all attached modules is stored,
    // so future update() calls will work off whatever was read by the
    // retrigger flow.
    poll(DONT_TRIGGER); // just read, do not retrigger

    // Pass 1/2: Trigger all the 'off' inputs (released buttons) first
    detectChanges(0x0000, _lastState);

    // Pass 2/2: Trigger all the 'on' inputs (pressed buttons)
    detectChanges(0xFFFF, _lastState);
}

// Triggers the event handler for the associated input channel
void MFDigInMux::trigger(uint8_t channel, bool state)
{
    if (!_MUX) return;
    if (!_inputHandler) return;
    (*_inputHandler)((state ? MuxDigInOnRelease : MuxDigInOnPress), channel, _name);
}

// Attaches a new event handler for the specified event.
void MFDigInMux::attachHandler(MuxDigInEvent newHandler)
{
    _inputHandler = newHandler;
}

// Clears the internal state
void MFDigInMux::clear()
{
    _lastState = 0;
}

// MFDigInMux.cpp