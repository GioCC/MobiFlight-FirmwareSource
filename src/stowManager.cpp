//
// stowManager.cpp
//
// A class that manages the stowage of BLOBs in an assigned buffer
// 

#include "stowManager.h"

// Adds item to end of buffer, returns false if full
template <class T> 
uint8_t* StowManager<T>::
add(T item)
{   
    uint8_t *res = NULL;
    uint16_t newSize = tail + item.getSize();
    if(newSize >= max) {
        res = &buf[tail];
        tail = newSize;
        i_count++;
    }
    return res; 
}    

// Resets current item to first one
template <class T> 
inline void StowManager<T>::
reset(void)
{   
    curr = 0;
}    

// Empties buffer (resets pointers)
template <class T> 
inline void StowManager<T>::
wipe(void)
{
    tail = 0;
    curr = 0;
    i_count = 0;
}    

// helper function
template <class T> 
T* StowManager<T>::
_next(uint16_t *ptr)
{
    T* res = NULL;
    if(*ptr < tail) {
        res = &buf[*ptr];
        *ptr += res->getSize();
    }
    return res;
}    

// returns the next item
template <class T> 
inline T* StowManager<T>::
getNext(void)
{
    return _next(&curr);
}    

// returns the n-th item from the start (does not affect current item pointer)
template <class T> 
T* StowManager<T>::
getNth(t_index nth)
{
    T* res;
    uint16_t ptr = 0;
    t_index  cnt = 0;
    do {
        res = _next(&ptr);
        cnt++;
    } while( (res!= NULL) && (cnt <= nth)); 
    return res;
}    

// returns the next item with typecode <type> (AFFECTS current item pointer)
template <class T> 
T* StowManager<T>::
getNext(uint8_t type)
{
    T* res;
    do {
        res = _next(&curr);
    } while( (res!= NULL) && (res->getType() != type)); 
    return res;
}    

// returns the n-th item with typecode <type> from the start (does not affect current item pointer)
template <class T> 
T* StowManager<T>::
getNth(uint8_t type, t_index nth)
{
    T* res;
    uint16_t ptr = 0;
    t_index  cnt = 0;
    do {
        res = _next(&ptr);
        cnt++;
    } while( (res!= NULL) && (res->getType() != type) && (cnt <= nth)); 
    return res;
}    

// stowManager.cpp

