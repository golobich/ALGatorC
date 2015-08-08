//
//  ParameterSet.cpp
//  ALGatorC
//
//  Created by Tadej Golobič on 31/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#include "ParameterSet.hpp"

ParameterSet::ParameterSet(ParameterSet &params)
{
    for (int i = 0; i<params.size(); i++)
    {
        EParameter *p = params.get_parameter(i);
        if (p != nullptr)
            add_parameter(*p, true);
        else
            LOG(WARNING) << __FILE__ << "(" << __LINE__ << ") : Null pointer";
        delete p;
    }
}

void ParameterSet::add_parameter(EParameter &param, bool replace_existing)
{
    std::vector<EParameter>::iterator it = std::find(parameters.begin(), parameters.end(), param);
    if (it != parameters.end())
    {
        if (replace_existing)
        {
            parameters.erase(it);
            parameters.push_back(param);
        }
    }else
    {
        parameters.push_back(param);
    }
}

void ParameterSet::add_parameters(ParameterSet &params, bool replace_existing)
{
    for (int i = 0; i < params.size(); i++)
    {
        EParameter *param = params.get_parameter(i);
        if (param != nullptr)
            add_parameter(*param, replace_existing);
        else
            LOG(WARNING) << __FILE__ << "(" << __LINE__ << ") : Null pointer";
    }
}

EParameter* ParameterSet::get_parameter(const int &i)
{
    if (i < parameters.size() && i >= 0)
        return &parameters.at(i);
    LOG(WARNING) << __FILE__ << "(" << __LINE__ << ") : Null pointer. Requested parameter at: " << i << ", size of parameters: " << parameters.size();
    return nullptr;
}

EParameter* ParameterSet::get_parameter(const std::string &name)
{
    for (int i = 0; i<parameters.size(); i++)
    {
        if (parameters.at(i).get(EParameter::id_name) == name)
            return &parameters.at(i);
    }
    LOG(WARNING) << __FILE__ << "(" << __LINE__ << ") : Null pointer: Requested parameter with name '" << name << "' does not exists";
    return nullptr;
}

size_t ParameterSet::size()
{
    return parameters.size();
}

std::string ParameterSet::print()
{
    std::string result = "";
    for (int i = 0; i<parameters.size(); i++)
    {
        if (!result.empty())
            result += "; ";
        EParameter p = parameters.at(i);
        result += p.get(EParameter::id_name) + " = " + p.get(EParameter::id_value);
    }
    return result;
}
