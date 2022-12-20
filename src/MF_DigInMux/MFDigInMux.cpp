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
    : _name("MUXDigIn"), _dataPin(0xFF), _flags(0), _lastState(0), _tempState(0)
{
}

MFDigInMux::MFDigInMux(MFMuxDriver *MUX, const char *name)
    : _name(name), _dataPin(0xFF), _flags(0), _lastState(0), _tempState(0)
{
    if (MUX) _MUX = MUX;
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

// Helper for "poll" function: does the reading and triggering part
// for a single (the current) input, relying on an external setting
// for the Mux channel.
// Extrapolated from "poll" so it can be used with different
// mux address-setting strategies.
// Scan MUST occur sequentially!
// "pass" is the scan count, and MUST start from 0 upwards.
// BEWARE: scanning the channels from 0 upwards, bits in _lastState
// are in reverse order (bit 0 = channel 8 or 16, depending on mux type).
// This has no consequence beside being counterintuitive, but must be kept in mind
// in case of future modifications.
// In fact, the channels between 0 and the [max input for the mux type]-1 (ie 7 or 15)
// could be scanned in any order, as long as it is consistent between calls.
void MFDigInMux::pollNext(bool doTrigger, uint8_t pass)
{
    const uint8_t    selMax = (bitRead(_flags, MUX_HALFSIZE) ? 8 - 1 : 16 - 1);
    volatile uint8_t pinVal;

    if (pass > selMax) return;
    if (0 == pass) _tempState = 0x0000;

    // Allow the output to settle from voltage transients:
    // transients towards 0 (GND) are negligible, but transients towards 1 (Vcc)
    // require a pullup to charge parasitic capacities.
    // These are examples of delay times measured for 0->1 transitions with different pull-ups:
    // integrated PU -> 1.4us
    // external, 10k -> 400ns
    // external, 4k7 -> 250ns
    // A digitalRead() takes about 5us (also the time for function calling must be accounted for),
    // therefore even the integrated pullup should be sufficient.
    // NB An external pullup (10k or 4k7) is recommended anyway for better interference immunity.
    pinVal = digitalRead(_dataPin);
    _tempState <<= 1;
    _tempState |= (pinVal ? 1 : 0);

    // BEWARE: For optimization, the detection of changes (input triggering)
    // is made only once - for all inputs - at the last pass.
    // This means that:
    // - selector indexes before the last can be scanned in any order, but all must be scanned before the last;
    // - the last selector must be scanned (can't be skipped) in order to trigger change detection.
    if (doTrigger && (pass == selMax)) {
        if (detectChanges(_lastState, _tempState)) _lastState = _tempState;
    }
}

// Core function for update() and retrigger()
// Reads the values from the attached module, compares them to the previously
// read values, and calls the registered event handler for any inputs that
// changed from the previously read state.
void MFDigInMux::poll(bool doTrigger)
{
    if (!_MUX) return;

    uint8_t selMax = (bitRead(_flags, MUX_HALFSIZE) ? 8 : 16);

    for (uint8_t sel = 0; sel < selMax; sel++) {
        pollNext(doTrigger, _MUX->setChannel(sel));
    }
}

// Detects changes between the current state and the previously saved state
bool MFDigInMux::detectChanges(uint16_t lastState, uint16_t currentState)
{
    if (!_MUX || (lastState == currentState)) return false;
    uint8_t  selMax = (bitRead(_flags, MUX_HALFSIZE) ? 8 : 16);
    uint16_t diff   = lastState ^ currentState;
    for (uint8_t i = 0; i < selMax; i++) {
        if (diff & 0x0001) {
            trigger(i, ((currentState & 0x0001) != 0));
        }
        diff >>= 1;
        currentState >>= 1;
    }
    return true;
}

// Reads the current state then fires 'release' events for every 'off' input,
// followed by 'press' events for every 'on' input.
// (Remember that 'off' inputs actually have physical status '1')
void MFDigInMux::retrigger()
{
    // The current state is stored,
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