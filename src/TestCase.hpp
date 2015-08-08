//
//  TestCase.h
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#ifndef TestCase_h
#define TestCase_h

#include <iostream>
#include "ParameterSet.hpp"
#include "EParameter.hpp"

class TestCase
{
public:
	virtual ~TestCase();
    //! Add parameter to TestCase
    //! \param EParameter
    virtual void add_parameter(EParameter&);
    
    //! Get all parameters
    //! \return ParameterSet
    ParameterSet get_parameters();
    
    //! Print TestCase
    //! \return string
    std::string print();
    
protected:
    ParameterSet parameters;
    
};

#endif /* TestCase_h */
