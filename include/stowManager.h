//
// stowManager.h
//
// A class that manages the stowage of BLOBs in an assigned buffer
// 

#ifndef _STOWMGR_H
#define _STOWMGR_H

#include <stddef.h>
#include <stdint.h>

// Templating in not really necessary...
class StowManager
{
    //public typedef uint8_t      t_index;
    private:
        typedef uint8_t      t_index;
        
        uint8_t *buf;
        uint8_t *bufTop;
        uint8_t *tail;
        uint8_t *curr;
        
        t_index i_curr;
        t_index i_count;

        uint8_t* _next(uint8_t **ptr);

    public:

        StowManager(uint8_t *buffer, uint16_t maxSize)
        : buf(buffer), bufTop(buffer+maxSize), tail(buffer), curr(buffer), i_count(0)
        {}
        
        uint8_t* add(uint8_t reqdSize, uint8_t typeTag);  // Adds item to end of buffer, returns null if full
        
        void reset(void);           // Resets current item to first one
        void wipe(void);            // Empties buffer (resets pointers)
        
        t_index  getCount(void) { return i_count; }

        uint8_t* getNext(void);           // returns the next item
        uint8_t* getNext(uint8_t type);   // returns the next item with typecode <type> (AFFECTS current item pointer)
        
        uint8_t* getNth(t_index nth);     // returns the n-th item (base 0) from the start (does not affect current item pointer)
        uint8_t* getNth(uint8_t type, t_index nth);    // returns the n-th item (base 0) with typecode <type> from the start (does not affect current item pointer)

        // THIS METHOD IS STRICTLY APPLICATION-SPECIFIC
        // It requires <T> objects to have two methods, T::getSize(), T::getType(), to be passed 
        // as arguments to the ordinary "add()" 
        // Beside reserving space, it performs a "placement new" of the object of type <T>.
        // Please beware that the argument is a _pointer to_ an object pointer:
        // since an argument referencing the templated type is required in order to set the 
        // method signature, we may as well pass a reference to a pointer that will carry 
        // the return value.
        template <typename T> T* AddItem(T** itemPtr);
};

#endif // _STOWMGR_H

