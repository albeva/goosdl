//
//  Object.h
//  goosdl
//
//  Created by Albert Varaksin on 17/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once
#include <cstddef>

/**
 * Object
 *
 * this is the base class for nearly all GUI classes
 */
class Object
{
    size_t m_ID = genNewId();
    
protected:
    
    Object() = default;
    virtual ~Object() = default;
    
    Object( const Object& ) = delete;
    Object& operator=( const Object& ) = delete;
    
public:
    
    // generate new object ID
    static size_t genNewId();
    
    // get current object ID
    size_t getId() const { return m_ID; }
};
