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

using namespace goo;

int main(int argc, char *argv[])
{    
    // the window
    SDL_Window * win = SDL_CreateWindow("goosdl", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    
    // the renderer
    auto renderer = new Renderer(win);
    
    // create the layer
    Layer * box = new Layer({50, 50, 100, 100});
    box->setBackground(Color::DarkBlue);
    
    Layer * inner = new Layer({20, 20, 60, 60});
    inner->setBackground(Color::Blue);
    box->addLayer(inner);
    
    Layer * resizeGrip = new Layer({90, 90, 10, 10});
    resizeGrip->setBackground(Color::LightBlue);
    box->addLayer(resizeGrip);
    
    // do until quit
    bool dragging = false;
    bool resizing = false;
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
                } else if (resizing) {
                    Rect frame = box->getFrame();
                    frame += Size{e.motion.xrel, e.motion.yrel};
                    if (frame.size.width >= 20 && frame.size.height >= 20) {
                        box->setFrame(frame);
                        
                        Rect resizeRect = resizeGrip->getFrame();
                        resizeRect.position = Point{frame.w(), frame.h()} - resizeRect.size;
                        resizeGrip->setFrame(resizeRect);
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                Point point{e.motion.x, e.motion.y};
                if (box->findLayer(point) == resizeGrip) {
                    resizing = true;
                } else {
                    dragging = box->getFrame().contains(point);
                    if (dragging) {
                        box->setOpacity(127);
                    }
                }
            }else if (e.type == SDL_MOUSEBUTTONUP) {
                dragging = false;
                resizing = false;
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
