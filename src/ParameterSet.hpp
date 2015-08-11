//
//  ParameterSet.h
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#ifndef ParameterSet_h
#define ParameterSet_h

#include <vector>
#include <memory>
#include "EParameter.hpp"
#include "Log.hpp"

class ParameterSet
{
public:
    //! Default constructor. Initialize class
    ParameterSet() {}
    
    //! Create vector of added parameters
    //! \param params
    ParameterSet(ParameterSet&);
    
    ~ParameterSet() {}

    //! Add parameter to vector of parameters.
    //! \param param
    //! \param replace_existing If adding parameter is already in vector and this parameter is set to true, then existing parameter is replaced with a new one
    void add_parameter(EParameter&, bool);
    
    //! Add parameters to vector of parameters.
    //! \param params
    //! \param replace_existing If parameter is already in vector and this parameter is set to true, then existing parameter is replaced with a new one
    void add_parameters(ParameterSet&, bool);
    
    //! Get parameter at specified position
    //! \param i
    //! \return EParameter* If specified position is between 0 and size of vector, then the pointer to parameter is returned. Otherwise, function returns null pointer
    EParameter *get_parameter(const unsigned int&);
    
    //! Get parameter with specified name
    //! \param name
    //! \return EParameter* If specified name is valid, then function returns pointer to EParameter, otherwise, function returns null pointer
    EParameter *get_parameter(const std::string&);
    
    //! Return size of vector
    //! \return size_t
    size_t size();
    
    //! Print ParameterSet
    std::string print();

    //! Print ParameterSet in specified format and with specified delimiter. currently only in CSV
    std::string print_for_results(std::string, const char&);
    
private:
    std::vector<EParameter> parameters;
    
    
};

#endif /* ParameterSet_h */
