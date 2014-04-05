//
//  LayoutManager.h
//  goosdl
//
//  Created by Albert Varaksin on 05/04/2014.
//  Copyright (c) 2014 Albert Varaksin. All rights reserved.
//
#pragma once

#include "Object.h"
#include "rhea/simplex_solver.hpp"
#include <vector>

namespace goo {
    
    class Layer;
    class Constraint;
    
    /**
     * Manage constraint layouts
     */
    class LayoutManager : public Object
    {
    public:
        
        LayoutManager(Layer * root);
        
        void addConstraint(Constraint * constraint);
        void removeConstraint(Constraint * constraint);
        
        Layer * getRootLayer() const { return m_root; }
        void setRootLayout(Layer * root);
        
        void solve();
        
    private:
        
        Layer * m_root;
        rhea::simplex_solver m_solver;
        std::vector<Constraint *> m_constraints;
    };
    
}
