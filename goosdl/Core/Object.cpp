//
//  Object.cpp
//  goosdl
//
//  Created by Albert Varaksin on 17/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Object.h"
using namespace goo;

/**
 * Generate new object ID
 */
size_t Object::genNewId()
{
    static size_t couner = 0;
    return ++couner;
}