//
//  Layer.h
//  SdlTest
//
//  Created by Albert Varaksin on 17/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once
#include "Object.h"
#include "Geometry.h"
#include "Color.h"
#include "Renderer.h"

#include <vector>



/**
 * Layer represents a graphics context in the GUI
 */
class Layer : public Object
{
public:
    
    // create
    Layer(Rect frame);
    
    // destroy
    virtual ~Layer();
    
    // add new sublayer. This will take ownership of the layer
    void addLayer(Layer * layer);
    
    // remove child layer. releases the ownership
    void removeLayer(Layer * layer);
    
    // insert layer at an index
    void insertLayer(Layer * layer, size_t position);
    
    // insert layer above the target layer
    void insertLayerAbove(Layer * layer, Layer * target);
    
    // insert layer below the target layer
    void insertLayerBelow(Layer * layer, Layer * target);
    
    // the parent layer
    Layer * getParent() const { return m_parent; }
    
    // get layer surface. Surface is owned by the layer
    Renderer::HardwareTexture * getTexture() const { return m_texture; }
    
    // child layers
    const std::vector<Layer *> & getLayers() const { return m_layers; }
    
    // layer frame
    const Rect & getFrame() const { return m_frame; }
    void setFrame(const Rect & rect);
    
    // set the background
    void setBackground(const Color & color);
    const Color & getBackgroundColor() const { return m_backgroundColor; }
    
    // set alpha value
    void setOpacity(unsigned char alpha);
    unsigned char getOpacity() const { return m_alpha; }
    
    // need to re-render the layer?
    bool getNeedRender() const { return m_needRender; };
    
    // need to update the layer. This is different from render in that it indicates
    // the need only to redraw the surface onto the parent rather than repaint the surface
    bool getNeedUpdate() const { return m_needUpdate; }
    void setNeedUpdate(bool need) { m_needUpdate = need; }
    
    // render this layer
    void render();
    
private:
    
    // layer attributes
    Color m_backgroundColor;
    unsigned char m_alpha;
    
    // data
    bool m_needRender;
    bool m_needUpdate;
    Layer * m_parent;
    Rect m_frame;
    std::vector<Layer *> m_layers;
    
    Renderer * m_renderer;
    Renderer::HardwareTexture * m_texture;
};
