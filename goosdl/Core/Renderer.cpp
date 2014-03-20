//
//  Renderer.cpp
//  goosdl
//
//  Created by Albert Varaksin on 19/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Renderer.h"
#include "Layer.h"
#include <SDL2/SDL.h>


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
  m_targetTexture(nullptr)
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


// create platform specific texture
Renderer::HardwareTexture * Renderer::createTexture(Size size)
{
    auto texture = SDL_CreateTexture(m_renderer,
                                     SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET,
                                     size.width,
                                     size.height);

    if (texture == nullptr) {
        std::cout << "Failed to create SDL_Texture. Error: " << SDL_GetError() << '\n';
    }
    return reinterpret_cast<HardwareTexture*>(texture);
}


// destroy platform texture
void Renderer::destroyTexture(HardwareTexture * texture)
{
    SDL_DestroyTexture(reinterpret_cast<SDL_Texture *>(texture));
}


// set the rendering target
void Renderer::setTargetTexture(HardwareTexture * texture)
{
    m_targetTexture = texture;
    SDL_SetRenderTarget(m_renderer, reinterpret_cast<SDL_Texture *>(texture));
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
    HardwareTexture * current = m_targetTexture;
    
    // need to render the content?
    if (layer->getNeedRender()) {
        pushState();
        setTargetTexture(layer->getTexture());
        layer->render();
        for (Layer * child : layer->getLayers()) {
            child->setNeedUpdate(true);
            render(child);
        }
        setTargetTexture(current);
        popState();
    }
    // nothing to update? return.
    else if (!layer->getNeedUpdate()) return;
    layer->setNeedUpdate(false);
    
    // clear the main screen
    if (current == nullptr) {
        fill();
    }
    
    // target texture
    SDL_Texture * texture = reinterpret_cast<SDL_Texture *>(layer->getTexture());
    
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
