//
//  ETestSet.cpp
//  AlgatorC
//
//  Created by Tadej Golobič on 01/07/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#include "ETestSet.hpp"

const std::string ETestSet::id_test_set = "TestSet";
const std::string ETestSet::id_short_name = "ShortName";
const std::string ETestSet::id_desc = "Description";
const std::string ETestSet::id_html_desc_file = "HTMLDescFile";
const std::string ETestSet::id_n = "N";
const std::string ETestSet::id_test_repeat = "TestRepeat";
const std::string ETestSet::id_time_limit = "TimeLimit";
const std::string ETestSet::id_quick_test = "QuickTest";
const std::string ETestSet::id_test_set_files = "TestSetFiles";
const std::string ETestSet::id_desc_file = "DescriptionFile";

ETestSet::ETestSet()
{
    std::vector<std::string> v;
    v.push_back(ETestSet::id_short_name);
    v.push_back(ETestSet::id_desc);
    v.push_back(ETestSet::id_html_desc_file);
    v.push_back(ETestSet::id_n);
    v.push_back(ETestSet::id_test_repeat);
    v.push_back(ETestSet::id_time_limit);
    v.push_back(ETestSet::id_quick_test);
    v.push_back(ETestSet::id_test_set_files);
    v.push_back(ETestSet::id_desc_file);
    init(ETestSet::id_test_set, v);
    v.clear();
}

ETestSet::ETestSet(const std::string &fname) : ETestSet()
{
    init_from_file(fname);
    std::vector<std::string> v;
    v.push_back(ETestSet::id_short_name);
    v.push_back(ETestSet::id_desc);
    set_representatives(v);
    v.clear();
}

std::string ETestSet::get_test_set_description_file()
{
    return get(ETestSet::id_desc_file);
}