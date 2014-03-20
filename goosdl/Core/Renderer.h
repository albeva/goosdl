//
//  Renderer.h
//  goosdl
//
//  Created by Albert Varaksin on 19/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once

#include "Object.h"
#include "Color.h"
#include "Geometry.h"
#include <stack>

struct SDL_Renderer;
struct SDL_Window;

namespace goo {

class Layer;
class Surface;
    
/**
 * render the layers
 */
class Renderer : public Object
{
public:
    
    // get default renderer. A renderer *must* be instantiated first
    // simply returns currently the first renderer
    static Renderer * getDefaultRenderer();
 
    // create
    Renderer(SDL_Window * wnd);
    
    // clean up
    virtual ~Renderer();
    
    // render
    void render(Layer * root);
    
    // set the colour
    void setColour(const Color & color);
    
    // fill the surface
    void fill();
    
    // push the state
    void pushState();
    
    // pop the state
    void popState();
    
    // get native renderer
    SDL_Renderer * getNativeRenderer() const { return m_renderer; }
    
private:
    
    // set the target rendering surface
    void setTargetSurface(Surface * texture);
    
    // drawing context state
    struct State {
        State(const Color & color) : drawColour(color) {}
        Color drawColour;
    };
    std::stack<State> m_stateStack;
    Color m_drawingColor;
    
    // data
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
    Surface * m_targetSurface;
};

} // ~namespace goo
