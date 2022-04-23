//
// MFDigInMux.cpp
//
// (C) MobiFlight Project 2022
//

#include "MFDigInMux.h"
#include "MFMuxDriver.h"
#include "mobiflight.h"

MFMuxDriver * MFDigInMux::_MUX;

MuxDigInEvent MFDigInMux::_handler = NULL;


MFDigInMux::MFDigInMux(void)
{
    _MUX = NULL;
    _name = "MUXDigIn";
    _flags = 0x00;

}

void MFDigInMux::setMux(MFMuxDriver *MUX)
{
    _MUX = MUX;
}

// Registers a new MUX input block and configures the driver pins
void MFDigInMux::attach(uint8_t dataPin, bool halfSize, char const *name)
{
    //if(!_MUX) return;     // no need to check, the object can be set up in advance before the MUX is configured
    _dataPin    = dataPin;
    _name       = name;
    _flags      = 0x00; 
    if(halfSize) bitSet(_flags, MUX_HALFSIZE);
    pinMode(_dataPin, INPUT_PULLUP);
    bitSet(_flags, MUX_INITED);

    // Initialize all inputs with current status
    poll(DONT_TRIGGER);

}

void MFDigInMux::detach()
{
    if(bitRead(_flags, MUX_INITED)) {
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
    if(!_MUX) return;

    uint8_t  selMax = (bitRead(_flags, MUX_HALFSIZE) ? 8 : 16);

        pinVal = digitalRead(_dataPin);
        pinVal = digitalRead(_dataPin);
        // delayMicroseconds(5);  // This is overkill
        currentState <<= 1;
        currentState |= (pinVal ? 1 : 0);
    }
    _MUX->restoreChannel(); // tidy up

        uint16_t    currentState = 0x0000;
        volatile uint8_t pinVal;

        _MUX->saveChannel();
        for (uint8_t sel = selMax; sel > 0; sel--) {
            _MUX->setChannel(sel-1);
            
            // Allow the output to settle from voltage transients:
            // transients towards 0 (GND) are negligible, but transients towards 1 (Vcc)
            // require a pullup to charge parasitic capacities.
            // These are examples of delay times measured for 0->1 transitions with different pull-ups:
            // integrated PU -> 1.4us
            // external, 10k -> 400ns
            // external, 4k7 -> 250ns
            // A digitalRead() takes about 5us, therefore even the integrated pullup should be sufficient;
            // for added safety, we perform one more (useless) digitalRead().
            // NB An external pullup (10k or 4k7) is recommended anyway for better interference immunity.

            pinVal = digitalRead(_dataPin);
            pinVal = digitalRead(_dataPin);
            // delayMicroseconds(5);  // This is overkill
            currentState <<= 1;
            currentState |= (pinVal ? 1 : 0);
        }
        _MUX->restoreChannel(); // tidy up

        if (_lastState != currentState) {
            if (doTrigger) detectChanges(_lastState, currentState);
        }
        _lastState = currentState;
}

// Detects changes between the current state and the previously saved state
void MFDigInMux::detectChanges(uint16_t lastState, uint16_t currentState)
{
    if(!_MUX) return;
    uint8_t     selMax = (bitRead(_flags, MUX_HALFSIZE) ? 8 : 16);
    uint16_t    diff   = lastState ^ currentState;
    for (uint8_t i = 0; i < selMax; i++) {
        if (diff & 0x0001) {
            trigger(i, ((currentState & 0x0001)!=0));
        }
        diff >>= 1;
        currentState >>= 1;
    }
}

// Clears the internal state
void MFDigInMux::reset(uint8_t action)
{
    // Handle retrigger logic according to:
    // https://github.com/MobiFlight/MobiFlight-Connector/issues/497
    // and  https://github.com/MobiFlight/MobiFlight-Connector/pull/502.
    if(action == ONRESET_RELEASE) {
        // The current state for all attached modules is stored,
        // so future update() calls will work off whatever was read by the
        // retrigger flow.
        poll(DONT_TRIGGER);    // just read, do not retrigger
    }
    // (Remember that 'off' inputs actually have physical status '1')
    if(action == ONRESET_RELEASE) {
        // Pass 1/2: Trigger all the 'off' inputs (released buttons) first
        detectChanges(0x0000, _lastState);  
    } else 
    if(action == ONRESET_PRESS) {
        // Pass 2/2: Trigger all the 'on' inputs (pressed buttons)
        detectChanges(0xFFFF, _lastState);  
    }
}

// Triggers the event handler for the associated input channel
void MFDigInMux::trigger(uint8_t channel, bool state)
{
    if(_handler) {
        (*_handler)((state ? MuxDigInOnRelease : MuxDigInOnPress), channel, _name);
    }
}


void MFDigInMux::setLazyMode(bool mode)
{
    if(mode) {
        bitSet(_flags, MUX_LAZY);
    } else {
        bitClear(_flags, MUX_LAZY);
    }
}

// MFDigInMux.cpp