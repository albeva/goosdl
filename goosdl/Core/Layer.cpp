//
//  Layer.cpp
//  goosdl
//
//  Created by Albert Varaksin on 17/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Layer.h"
#include <algorithm>


/**
 * initialize the layer
 */
Layer::Layer(Rect frame)
: m_parent(nullptr), m_frame(frame), m_backgroundColor(Color::Black)
{
    m_renderer = Renderer::getDefaultRenderer();
    m_texture = m_renderer->createTexture(frame.size);
    m_alpha = 255;
    m_needRender = true;
}


/**
 * Clean up the layer. Remove all children
 */
Layer::~Layer()
{
    // remove this from the parent
    if (m_parent) {
        m_parent->removeLayer(this);
    }
    
    // remove surface
    if (m_texture) {
        m_renderer->destroyTexture(m_texture);
    }
    
    // remove all child layers
    for (Layer * layer : m_layers) {
        layer->m_parent = nullptr;
        delete layer;
    }
}


#pragma - Layer attributes

/**
 * set layer frame
 */
void Layer::setFrame(const Rect & rect)
{
    m_needRender = rect.size != m_frame.size;
    m_frame = rect;
    m_needUpdate = true;
}


// set the background
void Layer::setBackground(const Color & color)
{
    m_backgroundColor = color;
    m_needRender = true;
}


// set layer opacity
void Layer::setOpacity(unsigned char alpha)
{
    m_alpha = alpha;
    m_needUpdate = true;
}


#pragma - Layer rendering


// render this layer
void Layer::render()
{
    m_needRender = false;
    m_needUpdate = true;
    
    // clear the surface
    m_renderer->setColour(m_backgroundColor);
    m_renderer->fill();
}


#pragma - Manage Child layers


/**
 * Add layer as a child item. Remove from
 * existing parent
 */
void Layer::addLayer(Layer *layer)
{
    if (layer->m_parent != nullptr) {
        layer->m_parent->removeLayer(layer);
    }
    m_layers.push_back(layer);
    layer->m_parent = this;
}


/**
 * Remove child layer and release the ownership
 */
void Layer::removeLayer(Layer * layer)
{
    auto iter = std::find(m_layers.begin(), m_layers.end(), layer);
    if (iter != m_layers.end()) {
        layer->m_parent = nullptr;
        m_layers.erase(iter);
    } else {
        // TODO throw error
    }
}


/**
 * insert child layer at spedified index
 */
void Layer::insertLayer(Layer * layer, size_t position)
{
    removeLayer(layer);
    
    if (position >= m_layers.size()) {
        m_layers.push_back(layer);
    } else {
        m_layers.insert(m_layers.begin() + position, layer);
    }
    
    layer->m_parent = nullptr;
}


// insert layer below the target layer
void Layer::insertLayerAbove(Layer * layer, Layer * target)
{
    auto iter = std::find(m_layers.begin(), m_layers.end(), target);
    if (iter != m_layers.end()) {
        insertLayer(layer, iter - m_layers.begin() + 1);
    } else {
        // TODO error
    }
}


// insert layer below the target layer
void Layer::insertLayerBelow(Layer * layer, Layer * target)
{
    auto iter = std::find(m_layers.begin(), m_layers.end(), target);
    if (iter != m_layers.end()) {
        insertLayer(layer, iter - m_layers.begin());
    } else {
        // TODO error
    }
}
