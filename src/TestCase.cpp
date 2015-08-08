//
//  TestCase.cpp
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//
#include "TestCase.hpp"

TestCase::~TestCase()
{
	delete &parameters;
}

ParameterSet TestCase::get_parameters()
{
    return parameters;
}

void TestCase::add_parameter(EParameter &param)
{
    parameters.add_parameter(param, true);
}

std::string TestCase::print()
{
    return parameters.print();
}
