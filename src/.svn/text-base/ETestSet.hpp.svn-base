//
//  ETestSet.h
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#ifndef ETestSet_h
#define ETestSet_h

#include <vector>
#include "Entity.hpp"



class ETestSet : public Entity
{
public:
    const static std::string id_test_set;
    const static std::string id_short_name;
    const static std::string id_desc;
    const static std::string id_html_desc_file;
    const static std::string id_n;
    const static std::string id_test_repeat;
    const static std::string id_time_limit;
    const static std::string id_quick_test;
    const static std::string id_test_set_files;
    const static std::string id_desc_file;
    
    //! Default constructor
    ETestSet();
    
    //! Get JSON file and call init from file
    ETestSet(const std::string&);
    
    //! The value of the "DescriptionFile" field
    std::string get_test_set_description_file();
    
protected:
private:
    
};

#endif /* ETestSet_h */
