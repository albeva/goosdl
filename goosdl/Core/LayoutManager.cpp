//
//  LayoutManager.cpp
//  goosdl
//
//  Created by Albert Varaksin on 05/04/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "LayoutManager.h"
#include "Geometry.h"
#include "Layer.h"
#include "Constraint.h"

#include <algorithm>
#include <map>
#include <unordered_map>

#include "rhea/iostream.hpp"

using namespace goo;

namespace  {
    
    // define the layotu variables each view has
    struct LayoutVariables
    {
        bool _intialized   = false;
        bool _updated      = false;
        
        // set values
        rhea::variable x1  = 0;
        rhea::variable y1  = 0;
        rhea::variable w   = 0;
        rhea::variable h   = 0;
        
        // calculated values
        rhea::variable x2  = 0;
        rhea::variable y2  = 0;
        rhea::variable c_x = 0;
        rhea::variable c_y = 0;
        rhea::variable w_2 = 0;
        rhea::variable h_2 = 0;
        
        // default constructor
        LayoutVariables() = default;
        
        // no copying
        LayoutVariables( const LayoutVariables& ) = delete;
        LayoutVariables& operator=( const LayoutVariables& ) = delete;
    };
    
    // send the content to output stream (debugging)
    std::ostream& operator<<(std::ostream& os, const LayoutVariables & vars)
    {
        os  << "LayoutVariables {\n"
            << "  x1  = " << vars.x1.int_value()  << '\n'
            << "  y1  = " << vars.y1.int_value()  << '\n'
            << "  x2  = " << vars.x2.int_value()  << '\n'
            << "  y2  = " << vars.y2.int_value()  << '\n'
            << "  w   = " << vars.w.int_value()   << ", w / 2 = " << vars.w_2.int_value() << '\n'
            << "  h   = " << vars.h.int_value()   << ", h / 2 = " << vars.h_2.int_value() << '\n'
            << "  c_x = " << vars.c_x.int_value() << '\n'
            << "  c_y = " << vars.c_y.int_value() << '\n'
            << "}\n";
        
        return os;
    }
    
    // make it easy to assign layout variables to a rect
    inline void operator << (Rect & lhs, const LayoutVariables & rhs)
    {
        lhs.position.x  = rhs.x1.int_value();
        lhs.position.y  = rhs.y1.int_value();
        lhs.size.width  = rhs.w.int_value();
        lhs.size.height = rhs.h.int_value();
    }
    
    // store in a simple map. Associate layer vairables with a layer
    std::map<Layer*, LayoutVariables> _layerVarsMap;
    
    // usable null variable
    rhea::variable null_variable(0);
    
    // get vairable for attribute
    rhea::variable getVariable(LayoutVariables & vars, Constraint::Attribute attrib)
    {
        switch (attrib) {
            case Constraint::Attribute::Top:
                return vars.y1;
            case Constraint::Attribute::Bottom:
                return vars.y2;
            case Constraint::Attribute::Left:
                return vars.x1;
            case Constraint::Attribute::Right:
                return vars.x2;
            case Constraint::Attribute::Width:
                return vars.w;
            case Constraint::Attribute::Height:
                return vars.h;
            case Constraint::Attribute::CenterX:
                return vars.c_x;
            case Constraint::Attribute::CenterY:
                return vars.c_y;
            default:
                throw "Invalid";
        }
    }
    
    // get vairable for attribute based on relation between views
    rhea::variable getVariable(LayoutVariables & vars,
                               Constraint::Attribute attrib,
                               Layer * srcView,
                               Layer * dstView)
    {
        if (dstView->getParent() != srcView) return getVariable(vars, attrib);
        
        switch (attrib) {
            case Constraint::Attribute::Top:
            case Constraint::Attribute::Left:
                return null_variable;
            case Constraint::Attribute::Bottom:
            case Constraint::Attribute::Height:
                return vars.h;
            case Constraint::Attribute::Right:
            case Constraint::Attribute::Width:
                return vars.w;
            case Constraint::Attribute::CenterX:
                return vars.w_2;
            case Constraint::Attribute::CenterY:
                return vars.h_2;
            default:
                throw "Invalid";
        }
    }
}


/**
 * Instantiate new layout manager
 * with the root view object
 */
LayoutManager::LayoutManager(Layer * m_root)
: m_root(m_root)
{
    if (m_root) {
        m_root->setLayoutManager(this);
    }
    
    m_solver.add_stay(null_variable);
}


/**
 * set the root layout
 */
void LayoutManager::setRootLayout(Layer * root)
{
    m_root = root;
}


/**
 * add new constraint
 */
void LayoutManager::addConstraint(Constraint * constraint)
{
    m_constraints.push_back(constraint);
    
    auto srcView = constraint->m_sourceView;
    auto dstView = constraint->m_targetView;
    
    auto srcAttrib = constraint->m_sourceAttrib;
    auto dstAttrib = constraint->m_targetAttrib;
    
    auto & dstVars = _layerVarsMap[dstView];
    if (dstVars._intialized == false) {
        dstVars._intialized = true;
        
        // constraints
        rhea::constraint center_x ( dstVars.c_x == dstVars.x1 + dstVars.w / 2 ),
                         center_y ( dstVars.c_y == dstVars.y1 + dstVars.h / 2 ),
                         pos_x2   ( dstVars.x2  == dstVars.x1 + dstVars.w     ),
                         pos_y2   ( dstVars.y2  == dstVars.y1 + dstVars.h     ),
                         half_w   ( dstVars.w_2 == dstVars.w / 2              ),
                         half_h   ( dstVars.h_2 == dstVars.h / 2              );
        
        // add to the solver
        m_solver.add_constraint(center_x);
        m_solver.add_constraint(center_y);
        m_solver.add_constraint(pos_x2);
        m_solver.add_constraint(pos_y2);
        m_solver.add_constraint(half_w);
        m_solver.add_constraint(half_h);
    }
    
    // if is equal then "stay" this vaitable
    m_solver.add_stay(constraint->m_variable);
    
    // width
    if (srcView == nullptr) {
        rhea::variable dstV = getVariable(dstVars, dstAttrib);
        rhea::constraint c;
        switch (constraint->m_relation) {
            case Constraint::Relation::Equal:
                c = (dstV == constraint->m_variable);
                break;
            case Constraint::Relation::GreaterOrEqual:
                c = (dstV >= constraint->m_variable);
                break;
            case Constraint::Relation::LessOrEqual:
                c = (dstV <= constraint->m_variable);
                break;
        }
        m_solver.add_constraint(c);
    } else {
        rhea::variable dstV   = getVariable(_layerVarsMap[dstView], dstAttrib);
        rhea::variable srcV   = getVariable(_layerVarsMap[srcView], srcAttrib, srcView, dstView);
        rhea::constraint c(dstV == srcV + constraint->m_variable);
        m_solver.add_constraint(c);
    }
}


/**
 * Remove the constraint
 */
void LayoutManager::removeConstraint(Constraint * constraint)
{
    auto iter = std::find(m_constraints.begin(), m_constraints.end(), constraint);
    if (iter != m_constraints.end()) {
        m_constraints.erase(iter);
    }
}


/**
 * update constraints
 */
void LayoutManager::solve()
{
    bool resolve = false;
    for (auto constraint : m_constraints) {
        if (constraint->m_updated) {
            resolve = true;
            m_solver.add_edit_var(constraint->m_variable);
        }
    }
    if (!resolve) return;
    
    m_solver.begin_edit();
    for (auto constraint : m_constraints) {
        if (constraint->m_updated) {
            m_solver.suggest_value(constraint->m_variable, constraint->m_constant);
            constraint->m_updated = false;
        }
    }
    m_solver.end_edit();

    // update all child constraints
    for (auto constraint : m_constraints) {
        // destination view
        auto view = constraint->m_targetView;
        // constraint bars
        auto & vars = _layerVarsMap[view];
        // update only once per solve
        if (vars._updated) continue;
        vars._updated = true;
        
        Rect frame = view->getFrame();
        frame << vars;
        view->setFrame(frame);
        
        std::cout << view->getId() << " : " << vars;
    }
    
    // reset all updates
    for (auto & k : _layerVarsMap) {
        k.second._updated = false;
    }
}














