//
//  Renderer.cpp
//  goosdl
//
//  Created by Albert Varaksin on 19/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Renderer.h"
#include "Layer.h"
#include "Surface.h"
#include <SDL2/SDL.h>

using namespace goo;

/**
 * the default renderer
 */
static Renderer * _defaultRenderer = nullptr;


/**
 * get the default renderer
 */
Renderer * Renderer::getDefaultRenderer()
{
    return _defaultRenderer;
}


/**
 * create the renderer
 */
Renderer::Renderer(SDL_Window * wnd)
: m_window(wnd),
  m_drawingColor(Color::White),
  m_targetSurface(nullptr)
{
    m_renderer = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    if (m_renderer == nullptr) {
        std::cout << "Failed to create a renderer. Error: " << SDL_GetError() << '\n';
    }
    if (!_defaultRenderer) _defaultRenderer = this;
}


/**
 * clean up
 */
Renderer::~Renderer()
{
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }
}


// set the rendering target
void Renderer::setTargetSurface(Surface * surface)
{
    m_targetSurface = surface;
    SDL_SetRenderTarget(m_renderer, surface ? surface->getTexture() : nullptr);
}


// set the colour
void Renderer::setColour(const Color & color)
{
    m_drawingColor = color;
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}


// draw a box
void Renderer::fill()
{
    SDL_RenderClear(m_renderer);
}


// push the state
void Renderer::pushState()
{
    m_stateStack.emplace(m_drawingColor);
}


// pop the state
void Renderer::popState()
{
    if (m_stateStack.empty()) return;
    
    const State & top = m_stateStack.top();
    setColour(top.drawColour);
    m_stateStack.pop();
}


/**
 * render the layer
 */
void Renderer::render(Layer *layer)
{
    Surface * current = m_targetSurface;
    
    // need to render the content?
    if (layer->getNeedRender()) {
        pushState();
        setTargetSurface(layer->getSurface());
        layer->render();
        for (Layer * child : layer->getLayers()) {
            child->setNeedUpdate(true);
            render(child);
        }
        setTargetSurface(current);
        popState();
    }
    // nothing to update? return.
    else if (!layer->getNeedUpdate()) return;
    layer->setNeedUpdate(false);
    
    // clear the main screen
    if (current == nullptr) {
        fill();
        
//        // draw loads of dots
//        for (int i = 0; i < 100000; i++) {
//            SDL_SetRenderDrawColor(m_renderer, ((unsigned char)(rand() % 255)), ((unsigned char)(rand() % 255)), ((unsigned char)(rand() % 255)), 255);
//            SDL_RenderDrawPoint(m_renderer, rand() % 640, rand() % 480);
//        }
//        
//        setColour(m_drawingColor);
    }
    
    // target texture
    SDL_Texture * texture = layer->getSurface()->getTexture();
    
    // layer transparency
    // ideally update when opacity *has* changed.
    if (layer->getOpacity() == SDL_ALPHA_OPAQUE) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
    } else {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetTextureAlphaMod(texture, layer->getOpacity());
    
    // copy the texture to the target
    auto rect = reinterpret_cast<const SDL_Rect &>(layer->getFrame());
    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
    
    // update the screen
    if (current == nullptr) {
        SDL_RenderPresent(m_renderer);
    }
}
