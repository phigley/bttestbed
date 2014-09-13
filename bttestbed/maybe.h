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
        : valid{false}
    { }
    
    Maybe(T&& _value)
        : value{std::forward(_value)}
        , valid(true)
    { }
    
    Maybe(const T& _value)
        : value(_value)
        , valid{true}
    {
    }

    operator bool() const { return valid; }
    
    Maybe& operator=(const T&& rhs)
    {
        value = std::move(rhs);
        valid = true;
        return *this;
    }
    
    Maybe& operator=(const T& rhs)
    {
        value = rhs;
        valid = true;
        return *this;
    }

    T& operator*() { assert(valid); return value; }
    const T& operator*() const { assert(valid); return value; }
    
    T* operator->() { assert(valid); return &value; }
    const T* operator->() const { assert(valid); return &value; }
    
private:

    T       value;
    bool    valid;
};

#endif
