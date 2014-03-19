//
//  Color.h
//  goosdl
//
//  Created by Albert Varaksin on 18/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Core/Geometry.h"
#include "Core/Layer.h"
#include "Core/Color.h"
#include "Core/Renderer.h"
#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{    
    // the window
    SDL_Window * win = SDL_CreateWindow("goosdl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, 0);
    
    // the renderer
    auto renderer = new Renderer(win);
    
    // create the layer
    Layer * box = new Layer(Rect{50, 50, 100, 100});
    box->setBackground(Color::DarkBlue);
    
    Layer * inner = new Layer(Rect{20, 20, 60, 60});
    inner->setBackground(Color::Blue);
    box->addLayer(inner);
    
    Layer * tiny = new Layer(Rect{20, 20, 20, 20});
    tiny->setBackground(Color::LightBlue);
    inner->addLayer(tiny);
    
    // do until quit
    bool dragging = false;
    while (1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }else if (e.type == SDL_MOUSEMOTION) {
                if (dragging) {
                    Rect frame = box->getFrame();
                    frame.offset(e.motion.xrel, e.motion.yrel);
                    box->setFrame(frame);
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                dragging = box->getFrame().contains(e.motion.x, e.motion.y);
                if (dragging) {
                    box->setOpacity(127);
                }
            }else if (e.type == SDL_MOUSEBUTTONUP) {
                dragging = false;
                box->setOpacity(255);
            }
        }
        // update ui
        renderer->render(box);
    }
    
    // clean up
    delete box;
    delete renderer;
    SDL_DestroyWindow(win);
    
    // done
    return 0;
}
