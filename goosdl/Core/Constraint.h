//
//  Constraint.h
//  goosdl
//
//  Created by Albert Varaksin on 02/04/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once

#include "rhea/variable.hpp"

namespace goo {
    
    class Layer;
    class LayoutManager;
    
    /**
     * simple iOS autolayout inspiered constraint system
     */
    class Constraint
    {
    public:
        
        // layout attribute
        enum Attribute {
            None,
            Top,
            Bottom,
            Left,
            Right,
            Width,
            Height,
            CenterX,
            CenterY
        };
        
        // constraint type
        enum Relation {
            LessOrEqual,
            Equal,
            GreaterOrEqual
        };
        
        // Create relational constraint
        // where attribute of one view
        // relates to another attribute
        // from potentiall anotehr view
        Constraint(Layer * targetView,
                   Attribute targetAttrib,
                   Layer * sourceView,
                   Attribute sourceAttrib,
                   int constant = 0,
                   Relation relation = Relation::Equal)
        : m_targetView(targetView),
          m_targetAttrib(targetAttrib),
          m_sourceView(sourceView),
          m_sourceAttrib(sourceAttrib),
          m_relation(relation),
          m_updated(true),
          m_constant(constant),
          m_variable(0)
        {}
        
        
        // create constant constraint
        // like fixed width or position
        // without relation to another view
        Constraint(Layer * targetView,
                   Attribute targetAttrib,
                   int constant = 0,
                   Relation relation = Relation::Equal)
        : m_targetView(targetView),
          m_targetAttrib(targetAttrib),
          m_sourceView(nullptr),
          m_sourceAttrib(None),
          m_relation(relation),
          m_updated(true),
          m_constant(constant),
          m_variable(0)
        {}
        
        
        // set the constant
        void setConstant(int constant)
        {
            m_updated = true;
            m_constant = constant;
        }
        
        // get constant value
        int getConstant() const
        {
            return m_constant;
        }

    private:
        friend class LayoutManager;
        Layer        * m_targetView;
        Attribute      m_targetAttrib;
        Layer        * m_sourceView;
        Attribute      m_sourceAttrib;
        Relation       m_relation;
        bool           m_updated;
        int            m_constant;
        rhea::variable m_variable;
    };
    
    
} // ~namespace goo
