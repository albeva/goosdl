//
//  Layer.cpp
//  goosdl
//
//  Created by Albert Varaksin on 17/03/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Layer.h"
#include "Renderer.h"
#include "Surface.h"
#include "Constraint.h"
#include "LayoutManager.h"
#include <algorithm>
using namespace goo;


/**
 * initialize the layer
 */
Layer::Layer()
: m_parent(nullptr),
  m_backgroundColor(Color::Black),
  m_layout(nullptr),
  m_surface(nullptr)
{
    m_renderer = Renderer::getDefaultRenderer();
    m_alpha = 255;
    m_needRender = true;
}


/**
 * initialize the layer
 */
Layer::Layer(Rect frame)
: Layer()
{
    m_frame = frame;
}


/**
 * Clean up the layer. Remove all children
 */
Layer::~Layer()
{
    // remove surface
    if (m_surface) {
        delete m_surface;
    }
    
    // remove any possible constraints
    auto layout = getLayoutManager();
    for (Constraint * constraint : m_constraints) {
        layout->removeConstraint(constraint);
        delete constraint;
    }
    
    // if this is a root view of the layout manager
    if (m_layout && m_layout->getRootLayer() == this) {
        m_layout->setRootLayout(nullptr);
    }
    
    // remove this from the parent
    if (m_parent) {
        m_parent->removeLayer(this);
    }
    
    // remove all child layers
    for (Layer * layer : m_layers) {
        delete layer;
    }
}


// get layer surface. Surface is owned by the layer
Surface * Layer::getSurface()
{
    if (m_surface == nullptr) {
        m_surface = new Surface(m_renderer, m_frame.size);
    }
    return m_surface;
}



#pragma mark - Layer Layout

/**
 * set layer frame
 */
void Layer::setFrame(const Rect & rect)
{
    m_needRender = rect.size != m_frame.size;
    if (m_needRender) {
        if (m_surface) {
            m_surface->setSize(rect.size);
        }
    }
    
    m_frame = rect;
    m_needUpdate = true;
}


// add constraint
void Layer::addConstraint(Constraint * constraint)
{
    m_constraints.push_back(constraint);
    getLayoutManager()->addConstraint(constraint);
}


#pragma mark - Layer attributes


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


#pragma mark - Layer rendering


// render this layer
void Layer::render()
{
    m_needRender = false;
    m_needUpdate = true;
    
    // clear the surface
    m_renderer->setColour(m_backgroundColor);
    m_renderer->fill();
}


#pragma mark - Query child layers


// find layer at given coordinates. Coordinate is relative the parent layer
Layer * Layer::findLayer(const Point & point) const
{
    if (m_frame.contains(point) == false) return nullptr;
    
    // iterate in reverse order.
    for (auto iter = m_layers.rbegin(); iter != m_layers.rend(); iter++) {
        auto child = (*iter)->findLayer(point - m_frame.position);
        if (child != nullptr) return child;
    }
    
    return const_cast<Layer*>(this);
}


#pragma mark - Manage Child layers


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
        m_layers.insert(m_layers.begin() + (std::vector<Layer *>::difference_type)position, layer);
    }
    
    layer->m_parent = nullptr;
}


// insert layer below the target layer
void Layer::insertLayerAbove(Layer * layer, Layer * target)
{
    auto iter = std::find(m_layers.begin(), m_layers.end(), target);
    if (iter != m_layers.end()) {
        insertLayer(layer, (size_t)(iter - m_layers.begin() + 1));
    } else {
        // TODO error
    }
}


// insert layer below the target layer
void Layer::insertLayerBelow(Layer * layer, Layer * target)
{
    auto iter = std::find(m_layers.begin(), m_layers.end(), target);
    if (iter != m_layers.end()) {
        insertLayer(layer, (size_t)(iter - m_layers.begin()));
    } else {
        // TODO error
    }
}
