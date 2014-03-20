//
//  Color.h
//  goosdl
//
//  Created by Albert Varaksin on 18/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once
#include <iostream>
#include "ColorTable.def.h"


/**
 * Represent a colour
 */
struct Color
{
    union {
        struct { unsigned char r, g, b, a; };
        unsigned int color_tag;
    };
    
    // compare two colors for (in)equality
    inline bool operator == (const Color & rhs) const {
        return color_tag == rhs.color_tag;
    }
    inline bool operator != (const Color & rhs) const {
        return color_tag != rhs.color_tag;
    }
    
    /**
     * will find color with the given name or return
     * a black color
     */
    static const Color & findByName(std::string name);
    
    // get colours
    #define DECLARE_COLOR(_name, ...) static const Color _name;
    COLOR_TABLE(DECLARE_COLOR)
    #undef DECLARE_COLOR
    
    // get string representation
    std::string toString() const;
};
std::ostream& operator<<(std::ostream& os, const Color & color);