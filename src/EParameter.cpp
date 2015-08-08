//
//  EParameter.cpp
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#include "EParameter.hpp"

const std::string EParameter::id_result_parameter = "Parameter";
const std::string EParameter::id_name = "Name";
const std::string EParameter::id_desc = "Description";
const std::string EParameter::id_type = "Type";
const std::string EParameter::id_subtype = "Subtype";
const std::string EParameter::id_value = "Value";

//id_name, id_desc, id_type, id_subtype, id_value
EParameter::EParameter()
{
    std::vector<std::string> v;
    v.push_back(EParameter::id_name);
    v.push_back(EParameter::id_desc);
    v.push_back(EParameter::id_type);
    v.push_back(EParameter::id_subtype);
    v.push_back(EParameter::id_value);
    init(EParameter::id_result_parameter, v);
}

EParameter::EParameter(const std::string &file_name, bool json) : EParameter()
{
    std::vector<std::string> v;
    if (json)
        init_from_json(file_name);
    else
        init_from_file(file_name);
    v.clear();
    v.push_back(EParameter::id_name);
    v.push_back(EParameter::id_value);
    set_representatives(v);
}

EParameter::EParameter(const std::string &name, const std::string &desc, const std::string &value) : EParameter()
{
    set(EParameter::id_name, name);
    set(EParameter::id_desc, desc);
    set(EParameter::id_value, value);
}

void EParameter::set(const std::string &key, const std::string &value)
{
    fields.insert(std::pair<std::string, std::string>(key, value));
}

bool EParameter::operator==(const EParameter &param)
{
    return this->get(id_name) == param.get(id_name);
}

