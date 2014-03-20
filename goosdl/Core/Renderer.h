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

/**
 * render the layers
 */
class Renderer : public Object
{
public:
    
    // strongly typed anonynous type for the underlying
    // graphics texture
    struct HardwareTexture{};
    
    
    // get default renderer. A renderer *must* be instantiated first
    // simply returns currently the first renderer
    static Renderer * getDefaultRenderer();
 
    // create
    Renderer(SDL_Window * wnd);
    
    // clean up
    virtual ~Renderer();
    
    // render
    void render(Layer * root);
    
    // create platform specific texture
    HardwareTexture * createTexture(Size size);
    
    // destroy platform texture
    void destroyTexture(HardwareTexture * texture);
    
    // set the colour
    void setColour(const Color & color);
    
    // fill the surface
    void fill();
    
    // push the state
    void pushState();
    
    // pop the state
    void popState();
    
private:
    
    // set the target rendering surface
    void setTargetTexture(HardwareTexture * texture);
    
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
    HardwareTexture * m_targetTexture;
};

} // ~namespace goo
