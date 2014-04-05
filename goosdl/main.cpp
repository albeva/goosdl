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
#include "Core/Constraint.h"
#include "Core/LayoutManager.h"
#include <iostream>
#include <SDL2/SDL.h>

using namespace goo;

static const int SCREEN_WIDTH  = 640;
static const int SCREEN_HEIGHT = 480;

int main(int argc, char *argv[])
{    
    // the window
    SDL_Window * win = SDL_CreateWindow("goosdl",
                                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT,
                                        0);
    
    // the renderer
    auto renderer = new Renderer(win);
    
    // create the layer
    Layer * box = new Layer();
    box->setBackground(Color::DarkBlue);
    
    Layer * inner = new Layer();
    inner->setBackground(Color::Blue);
    box->addLayer(inner);
    
    Layer * resizeGrip = new Layer();
    resizeGrip->setBackground(Color::LightBlue);
    box->addLayer(resizeGrip);
    
    // layout manager
    LayoutManager layout(box);
    
    // Set main box with minimum of 50x50 pixels
    auto width = new Constraint{
        box,
        Constraint::Width,
        50,
        Constraint::Equal
    };
    box->addConstraint(width);
    auto height = new Constraint{
        box,
        Constraint::Height,
        50,
        Constraint::Equal
    };
    box->addConstraint(height);
    
    // center the box
    auto pos_x = new Constraint{
        box,
        Constraint::Left,
        SCREEN_WIDTH / 2
    };
    box->addConstraint(pos_x);
    auto pos_y = new Constraint{
        box,
        Constraint::Top,
        SCREEN_HEIGHT / 2
    };
    box->addConstraint(pos_y);
    
    // fill the inner box
    inner->addConstraint(new Constraint{
        inner,
        Constraint::Left,
        box,
        Constraint::Left,
        20
    });
    inner->addConstraint(new Constraint{
        inner,
        Constraint::Top,
        box,
        Constraint::Top,
        20
    });
    inner->addConstraint(new Constraint{
        inner,
        Constraint::Right,
        box,
        Constraint::Right,
        -20
    });
    inner->addConstraint(new Constraint{
        inner,
        Constraint::Bottom,
        box,
        Constraint::Bottom,
        -20
    });
    
    
    // set gripper to 10x10 pixels
    resizeGrip->addConstraint(new Constraint{
        resizeGrip,
        Constraint::Width,
        10
    });
    resizeGrip->addConstraint(new Constraint{
        resizeGrip,
        Constraint::Height,
        10
    });
    
    // align the gripper to the bottom right corner
    // of the box.
    box->addConstraint(new Constraint{
        resizeGrip,
        Constraint::Right,
        box,
        Constraint::Width
    });
    box->addConstraint(new Constraint{
        resizeGrip,
        Constraint::Bottom,
        box,
        Constraint::Height
    });
    
    // layout
    layout.solve();
    
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
                    pos_x->setConstant(pos_x->getConstant() + e.motion.xrel);
                    pos_y->setConstant(pos_y->getConstant() + e.motion.yrel);
                    layout.solve();
                } else if (resizing) {
                    width->setConstant(width->getConstant() + e.motion.xrel);
                    height->setConstant(height->getConstant() + e.motion.yrel);
                    layout.solve();
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
        SDL_Delay(1);
    }
    
    // clean up
    delete box;
    delete renderer;
    SDL_DestroyWindow(win);
    
    // done
    return 0;
}
