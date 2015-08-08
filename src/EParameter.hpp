//
//  EParameter.h
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#ifndef EParameter_h
#define EParameter_h

#include <string>

#include "Entity.hpp"

class EParameter : public Entity
{
    //TODO set_type_and_subtype
public:
    static const std::string id_result_parameter;
    static const std::string id_name;
    static const std::string id_desc;
    static const std::string id_type;
    static const std::string id_subtype;
    static const std::string id_value;
    
    //! Default constructor
    EParameter();
    
    //! Reads JSON file and adds key-value pair to fields
    EParameter(const std::string&, bool);
    
    EParameter(const std::string&, const std::string&, const std::string&);
    
    void set(const std::string&, const std::string&);
    
    //! Check if two parameters are identical (if they have same name)
    bool operator==(const EParameter &param);
protected:
private:
    std::string subtype;
    
};

#endif /* EParameter_h */
