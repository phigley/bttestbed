//
//  maybe.h
//  bttestbed
//
//  Created by Peter Higley on 9/13/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_maybe_h
#define bttestbed_maybe_h

#include <cassert>

// This is a simplistic implentation of std::optional.

template<typename T>
class Maybe
{
public:

    Maybe()
        : valid(false)
    { }
    
    Maybe(T&& value_)
        : value(std::forward(value_))
        , valid(true)
    { }
    
    Maybe(const T& value_)
        : value(value_)
        , valid(true)
    {
    }

    operator bool() const { return valid; }
    
    Maybe& operator=(const T&& rhs)
    {
        value = std::move(rhs);
        valid = true;
        return *this;
    }
    
    T& operator*() { assert(valid); return value; }
    const T& operator*() const { assert(valid); return value; }
    
    T* operator->() { assert(valid); return &value; }
    const T* operator->() const { assert(valid); return &value; }
    
    friend bool operator==(const Maybe& lhs, const Maybe& rhs)
    {
        // Valids don't match.
        if( lhs.valid != rhs.valid )
            return false;
        
        // Both valids are false
        if( !lhs.valid )
            return true;
        
        // Compare values.
        return lhs.value == rhs.value;
    }
    
    friend bool operator!=(const Maybe& lhs, const Maybe& rhs)
    {
        // Valids don't match.
        if( lhs.valid != rhs.valid )
            return true;
        
        // Both valids are false
        if( !lhs.valid )
            return false;
        
        // Compare values.
        return lhs.value != rhs.value;
    }
    
private:

    T       value;
    bool    valid;
};

#endif
