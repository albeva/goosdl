//
//  Color.cpp
//  goosdl
//
//  Created by Albert Varaksin on 18/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Color.h"
#include <unordered_map>
using namespace goo;


// initialize the colours
#define IMPLEMENT_COLOR(_name, r, g, b) const Color Color::_name = Color{r, g, b, 255};
COLOR_TABLE(IMPLEMENT_COLOR)
#undef DECLARE_COLOR


// color value to string map
static std::unordered_map<unsigned int, std::string> _colorNames = {
#define COLOR_TO_STRING(_name, r, g, b) \
    {Color::_name.color_tag, #_name},
COLOR_TABLE(COLOR_TO_STRING)
#undef COLOR_TO_STRING
};

// fetch color from name
static std::unordered_map<std::string, const Color &> _colorMap = {
#define STRING_TO_COLOR(_name, r, g, b) \
    {#_name, Color::_name},
    COLOR_TABLE(STRING_TO_COLOR)
#undef STRING_TO_COLOR
};


/**
 * Find color by given name or return a black color
 */
const Color & Color::findByName(std::string name)
{
    auto iter = _colorMap.find(name);
    if (iter != _colorMap.end()) return iter->second;
    return Color::Black;
}


/**
 * return a string representation of the colour
 */
std::string Color::toString() const
{
    std::string s = "Color{";
    auto iter = _colorNames.find(color_tag);
    if (iter == _colorNames.end()) {
        s += "r: " + std::to_string(r) + ", ";
        s += "g: " + std::to_string(g) + ", ";
        s += "b: " + std::to_string(b) + ", ";
        s += "a: " + std::to_string(a);
    } else {
        s += " " + iter->second + " ";
    }
    return s + "}";
}


/**
 * output the color to the output stream
 */
std::ostream& operator<<(std::ostream& os, const Color & color)
{
    os << color.toString();
    return os;
}
