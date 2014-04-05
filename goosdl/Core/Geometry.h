//
//  Geometry.h
//  goosdl
//
//  Created by Albert Varaksin on 18/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once

namespace goo {

    
struct Size;
struct Rect;


/**
 * represent a coordinate on screen
 */
struct Point
{
    int x, y;
    Point & operator = (const Point & rhs) = default;
    
    // compare two points for (in)equality
    inline bool operator == (const Point & rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    inline bool operator != (const Point & rhs) const {
        return x != rhs.x || y != rhs.y;
    }
    
    // subtract point from another
    inline Point operator - (const Point & rhs) const {
        return {x - rhs.x, y - rhs.y};
    }
    
    // add points together
    inline Point operator +(const Point & rhs) const {
        return {x + rhs.x, y + rhs.y};
    }
    
    // offset location by relative amount
    inline Point & offset(int xdiff, int ydiff)
    {
        x += xdiff;
        y += ydiff;
        return *this;
    }
};


/**
 * represent a size on screen
 */
struct Size
{
    int width, height;
    Size & operator = (const Size & rhs) = default;
    
    // compare two sizes for (in)equality
    inline bool operator == (const Size & rhs) const {
        return width == rhs.width && height == rhs.height;
    }
    inline bool operator != (const Size & rhs) const {
        return width != rhs.width || height != rhs.height;
    }
    
    // test if width and/or height is smaller than rhs
    inline bool operator < (const Size & rhs) const {
        return width < rhs.width || height < rhs.height;
    }
};
    

// create new point offset by size
inline Point operator + (const Point & lhs, const Size & rhs)
{
    return {lhs.x + rhs.width, lhs.y + rhs.height };
}
    
    
// create new point offset by size
inline Point operator - (const Point & lhs, const Size & rhs)
{
    return {lhs.x - rhs.width, lhs.y - rhs.height };
}


/**
 * reoresent a rectangular area on screen
 */
struct Rect
{
    Point position;
    Size  size;
    Rect & operator = (const Rect & rhs) = default;
    
    // compare two rects for (in)equality
    inline bool operator == (const Rect & rhs) const {
        return position == rhs.position && size == rhs.size;
    }
    inline bool operator != (const Rect & rhs) const {
        return position != rhs.position || size != rhs.size;
    }
    
    // check if this is empty rect
    explicit inline operator bool() const
    {
        return size.width == 0 && size.height == 0;
    }
    
    // shortcut methods to get the position and size
    inline int x1() const { return position.x; }
    inline int x2() const { return position.x + size.width; }
    inline int y1() const { return position.y; }
    inline int y2() const { return position.y + size.height; }
    inline int w() const { return size.width; }
    inline int h() const { return size.height; }
    
    // get corner positions
    inline Point topLeft() const { return position; }
    inline Point topRight() const { return Point{x2(), y1()}; }
    inline Point bottomLeft() const { return Point{x1(), y2()}; }
    inline Point bottomRight() const { return Point{x2(), y2()}; }
    
    // rect center position
    inline void setCenter(int x, int y)
    {
        position.x = x - size.width / 2;
        position.y = y - size.height / 2;
    }
    inline void setCenter(const Point & p)
    {
        setCenter(p.x, p.y);
    }
    inline Point center() const { return Point{x1() + w() / 2, y1() + h() / 2}; }
    
    // check if rect contains a point
    inline bool contains(int x, int y) const
    {
        return x >= x1() && x <= x2() && y >= y1() && y <= y2();
    }
    inline bool contains(const Point & p) const
    {
        return contains(p.x, p.y);
    }
    
    // check if rect contains another rect within itself
    inline bool contains(const Rect & rect) const
    {
        return x1() <= rect.x1() && x2() >= rect.x2()
            && y1() <= rect.y1() && y2() >= rect.y2();
    }
    
    // offset location by relative amount
    inline void offset(int xdiff, int ydiff)
    {
        position.offset(xdiff, ydiff);
    }
    
    // add size
    inline Rect & operator += (const Size & rhs)
    {
        size.width += rhs.width;
        size.height += rhs.height;
        return *this;
    }
};


} // ~namespace goo
