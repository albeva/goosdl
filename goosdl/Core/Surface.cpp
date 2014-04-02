//
//  Surface.cpp
//  goosdl
//
//  Created by Albert Varaksin on 20/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Surface.h"
#include "Renderer.h"
#include <SDL2/SDL.h>

using namespace goo;


/**
 * Create new texture
 */
Surface::Surface(Renderer * renderer, Size size)
: m_renderer(renderer), m_size(size)
{
    m_texture = SDL_CreateTexture(renderer->getNativeRenderer(),
                                  SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_TARGET,
                                  size.width,
                                  size.height);

    if (m_texture == nullptr) {
        std::cout << "Failed to create SDL_Texture. Error: " << SDL_GetError() << '\n';
    }
}


/**
 * clean up
 */
Surface::~Surface()
{
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
    }
}


/**
 * apply new size to the surface
 */
void Surface::setSize(const goo::Size &size)
{
    if (m_size < size) {
        SDL_DestroyTexture(m_texture);
        m_texture = SDL_CreateTexture(m_renderer->getNativeRenderer(),
                                      SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      size.width,
                                      size.height);
    }
    m_size = size;
}
