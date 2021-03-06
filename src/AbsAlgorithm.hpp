//
//  AbsAlgorithm.hpp
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#ifndef AbsAlgorithm_h
#define AbsAlgorithm_h

#include "Timer.hpp"
#include "TestCase.hpp"
#include "ParameterSet.hpp"

class AbsAlgorithm
{
public:
    Timer timer;
    
    AbsAlgorithm();
    std::map<int, std::vector<long>> get_execution_times();
    void set_execution_time(const int&, const long&);
    virtual void run() = 0;
    virtual bool init(TestCase*) = 0;
    virtual bool done() = 0;
    
private:
    std::map<int, std::vector<long>> execution_times;
};

#endif // AbsAlgorithm_hpp
