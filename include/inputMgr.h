//
// Everything moved to MFAnalog.h
//

#ifndef _INPUTMGR_H
#define _INPUTMGR_H

#include <stdint.h>

#include "MFBoards.h"
#include "config.h"

class In_Base
{
    public:
        typedef enum {
            NONE        = kTypeNotSet,
            BUTTON      = kTypeButton,
            ENCODER_SD  = kTypeEncoderSingleDetent, //deprecated
            ENCODER     = kTypeEncoder,
            ANALOG      = kTypeAnalogInput,
            SHIFTREG    = kTypeInputShifter,
        } InType;

        uint8_t Type = NONE;

        In_Base() {}
        virtual void Update(void)   = 0;
        //!virtual void OnChange(void) = 0;
        //!virtual void Add(void) = 0;
        virtual void OnReset(void)      {};
        virtual void Detach(void)       {};
        virtual void Retrigger(void)    {};
        //void Add(uint8_t pin, char const *name);    // ...how?
};


#include "MFButton.h"
class In_Button : public In_Base
{
    private:
        MFButton    _button;
    
    public:
        In_Button()     { Type = BUTTON; }
        void Update()   { _button.update(); }
        void OnReset()  {_button.triggerOnRelease(); _button.triggerOnPress(); }
        
        void OnChange(uint8_t eventId, uint8_t pin, const char *name);
        void Add(uint8_t pin, char const *name);

};

#include "MFEncoder.h"
class In_Encoder : public In_Base
{
    private:
        MFEncoder    _encoder;

    public:
        In_Encoder()    { Type = ENCODER; }
        void Update()   { _encoder.update(); }
        
        void OnChange(uint8_t eventId, uint8_t pin, const char *name);
        void Add(uint8_t pin1, uint8_t pin2, uint8_t encoder_type, char const *name);
};

#if MF_ANALOG_SUPPORT == 1
#include "MFAnalog.h"
class In_Analog : public In_Base
{
    private:
        MFAnalog    _analog;

    public:
        In_Analog()     {  Type = ANALOG; };
        void Update()   { _analog.update(); }

        void OnChange(int value, uint8_t pin, const char *name);
        void Add(uint8_t pin, char const *name, uint8_t sensitivity);
        
        void readAverage()   { _analog.readBuffer(); }
    
};
#endif

#if MF_INPUT_SHIFTER_SUPPORT == 1
#include "MFInputShifter.h"
class In_ShiftReg : public In_Base
{
    private:
        MFInputShifter    _shiftreg;

    public:
        In_ShiftReg()   { Type = SHIFTREG; };
        void Update()   { _shiftreg.update(); }
        void OnReset()  { _shiftreg.retrigger(); }

        void OnChange(uint8_t eventId, uint8_t pin, const char *name);
        void Add(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t modules, char const *name);
        
        void Detach()   { _shiftreg.detach(); }
};
#endif

#endif // _INPUTMGR_H

