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
    
    // create surface
    Surface(Renderer * renderer, Size size);
    
    // clean up
    virtual ~Surface();
    
    // get underlying SDL_Texture object
    SDL_Texture * getTexture() const { return m_texture; }
    
    // get texture size. Actual unerlying texture *can* be bigger!
    Size getSize() const { return m_size; }
    
    // apply new size. This might reallocate the texture and invalidate
    // the content
    void setSize(const Size & size);
    
private:
    Size m_size;
    SDL_Texture * m_texture;
    Renderer * m_renderer;
};
    
    
}
