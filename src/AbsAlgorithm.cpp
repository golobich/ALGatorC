//
//  AbsAlgorithm.cpp
//  ALGatorC
//
//  Created by Tadej Golobič on 23/07/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#include "AbsAlgorithm.hpp"

AbsAlgorithm::AbsAlgorithm()
{
    timer = Timer();
}

std::map<int, std::vector<long>> AbsAlgorithm::get_execution_times()
{
    return execution_times;
}

void AbsAlgorithm::set_execution_time(const int &timer, const long &time)
{
    if (execution_times.find(timer) != execution_times.end())
    {
        std::vector<long> tmp = execution_times.at(timer);
        tmp.push_back(time);
        execution_times.insert(std::pair<int, std::vector<long> >(timer, tmp));
    }else
    {
        std::vector<long> tmp;
        tmp.push_back(time);
        execution_times.insert(std::pair<int, std::vector<long>>(timer, tmp));
    }
}


