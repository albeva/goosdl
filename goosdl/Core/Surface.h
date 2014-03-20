//
//  Surface.h
//  goosdl
//
//  Created by Albert Varaksin on 20/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once
#include "Object.h"
#include "Geometry.h"

struct SDL_Texture;
struct SDL_Surface;

namespace goo {

class Renderer;

/**
 * encapsulate SDL_Texture, SDL_Surface behaviour
 */
class Surface : public Object
{
public:
    
    Surface(Renderer * renderer, Size size);
    
    virtual ~Surface();
    
    SDL_Texture * getTexture() const { return m_texture; }
    
    Size getSize() const { return m_size; }
    
private:
    Size m_size;
    SDL_Texture * m_texture;
};
    
    
}
