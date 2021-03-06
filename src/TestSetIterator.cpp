//
//  TestSetIterator.cpp
//  AlgatorC
//
//  Created by Tadej Golobič on 01/07/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#include "TestSetIterator.hpp"

TestSetIterator::TestSetIterator()
{
    project_name = "";
}

TestSetIterator::TestSetIterator(ETestSet &test_set) : TestSetIterator()
{
    set_test_set(test_set);
}

void TestSetIterator::set_test_set(ETestSet &test_set)
{
    this->test_set = &test_set;
    init_iterator();
}

ETestSet* TestSetIterator::get_test_set()
{
    return test_set;
};

bool TestSetIterator::init_iterator()
{
    if (test_set != nullptr)
    {
        std::string file_name = test_set->get_test_set_description_file();
        if (file_name.empty())
        {
            LOG(ERROR) << __FILE__ << "(" << __LINE__ << ") : TestSet Description file doesn't exist";
            return false;
        }else
        {
            std::string test_file_name = getenv("ALGATOR_ROOT"); test_file_name.append("/data_root/projects/PROJ-");
            test_file_name.append(project_name); test_file_name.append("/tests/"); test_file_name.append(file_name);
            try
            {
                file.open(test_file_name.c_str());
            } catch (...)
            {
                LOG(ERROR) << __FILE__ << "(" << __LINE__ << ") : Something went wrong in opening file!";
                return false;
            }
            
            line_number = 0;
            return true;
        }
    }
    return false;
}

void TestSetIterator::set_project_name(const char *s)
{
    this->project_name = s;
}

bool TestSetIterator::has_next()
{
    return !file.eof();
}

void TestSetIterator::read_next()
{
    if (file.is_open() && has_next())
    {
        current_input_line = "";
        std::getline(file, current_input_line);
        line_number++;
    }
}

void TestSetIterator::close()
{
    file.close();
}

int TestSetIterator::get_number_of_test_instances()
{
    if (test_set != nullptr)
        return std::atoi(test_set->get(ETestSet::id_n).c_str());
    return 0;
}
