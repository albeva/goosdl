//
//  Constraint.h
//  goosdl
//
//  Created by Albert Varaksin on 02/04/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once

#include "Geometry.h"

namespace goo {
    
    class Layer;
    
    
    /**
     * simple iOS autolayout inspiered constraint system
     */
    class Constraint
    {
    public:
        
        // layout attribute
        enum Attribute {
            Top,
            Bottom,
            Left,
            Right,
            Width,
            Height,
            CenterX,
            CenterY,
            None
        };
        
        // constraint type
        enum Relation {
            LessOrEqual,
            Equal,
            GreaterOrEqual
        };
        
        
        Constraint(Layer * firstView,
                   Attribute firstAttrib,
                   Relation relation,
                   Layer * secondView,
                   Attribute secondAttrib,
                   int constant)
        : m_firstView(firstView),
          m_firstAttrib(firstAttrib),
          m_relation(relation),
          m_secondView(secondView),
          m_secondAttrib(secondAttrib)
        {}
       
    private:
        
        Layer     * m_firstView;
        Attribute   m_firstAttrib;
        Relation    m_relation;
        Layer     * m_secondView;
        Attribute   m_secondAttrib;
        int         m_constant;
    };
    
    
} // ~namespace goo
