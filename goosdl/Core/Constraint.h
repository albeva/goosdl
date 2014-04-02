//
//  Constraint.h
//  goosdl
//
//  Created by Albert Varaksin on 02/04/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#include "Geometry.h"

namespace goo {
    
    class Layer;
    
    
    /**
     * simple iOS autolayout inspiered constraint system
     */
    class Constraint
    {
        
        // layout attribute
        enum Attribute {
            Top,
            Bottom,
            Left,
            Right,
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
        
       
    private:
        
        Layer     * m_firstView;
        Attribute   m_firstAttrib;
        Relation    m_relation;
        Layer     * m_secondView;
        Attribute   m_secondAttrib;
        int         m_constant;
    };
    
    
} // ~namespace goo
