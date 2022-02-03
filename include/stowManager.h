//
// stowManager.h
//
// A class that manages the stowage of BLOBs in an assigned buffer
// 

#ifndef _STOWMGR_H
#define _STOWMGR_H

#include <stdint.h>

template <class T> class StowManager
{
    // Templated class T is required to have two methods:
    // uint8_t getSize(void) - returns the object size in bytes;
    // uint8_t getType(void) - returns an object type code;
    //
    // 'getSize()' is used to be allow stowage of (objects of) types 
    // derived from the templated one; mixed-size objects are also possible.
    // Size could not be passed as additional argument to 'add()', because
    // the size of each actual stored object must be readable in order to iterate. 

    //public typedef uint8_t      t_index;

    private:
        typedef uint8_t      t_index;
        
        uint8_t *buf;
        uint16_t max;
        uint16_t tail;
        uint16_t curr;
        
        t_index i_count;

        T* _next(uint16_t *ptr);

    public:

        StowManager(uint8_t *buffer, uint16_t maxSize)
        : buf(buffer), max(maxSize), tail(0), curr(0), i_count(0)
        {}
        
        uint8_t* add(T item);       // Adds item to end of buffer, returns null if full
        void reset(void);           // Resets current item to first one
        void wipe(void);            // Empties buffer (resets pointers)
        T* getNext(void);           // returns the next item
        T* getNth(t_index nth);     // returns the n-th item (base 0) from the start (does not affect current item pointer)
        T* getNext(uint8_t type);   // returns the next item with typecode <type> (AFFECTS current item pointer)
        T* getNth(uint8_t type, t_index nth);    // returns the n-th item (base 0) with typecode <type> from the start (does not affect current item pointer)
};


#endif // _STOWMGR_H

