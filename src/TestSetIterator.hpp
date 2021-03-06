//
//  DefaultTestSetIterator.h
//  AlgatorC
//
//  Created by Tadej Golobič on 01/07/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#ifndef DefaultTestSetIterator_h
#define DefaultTestSetIterator_h

#include <fstream>

#include "ETestSet.hpp"
#include "TestCase.hpp"
#include "Log.hpp"

class TestSetIterator
{
public:
    //! Default constructor. If the default constructor is used, a setTestSet method has to be called after the instantination.
    TestSetIterator();
    
    //! Adds ETestSet to class
    //! \param ETestSet
    TestSetIterator(ETestSet&);
    
    //! Adds ETestSet to class
    //! \param ETestSet
    void set_test_set(ETestSet&);
    
    //! Get number of test instances
    //! \return number of test instances
    int get_number_of_test_instances();
    
    ETestSet* get_test_set();
    
    void set_project_name(const char*);
    
    //! Initiates the iterator (i.e. opens files, set envoronment, ...) so that
    //! the first call to hasNext and next methods will iterate through the test set data
    virtual bool init_iterator() = 0;
    
    //! Check if iterator is not at the end
    //! \return true if this iterator has next test case
    virtual bool has_next();
    
    //! Reads next test case (as a raw data)
    virtual void read_next();
    
    //! Creates a new TestCase object from a raw data read by readNext() method.
    //! Consecutive calls to getCurrent method must return different objects constructed
    //! from the same input data.
    virtual TestCase *get_current() = 0;
    
    //! Close the iterator source
    virtual void close();
    
    std::string current_input_line;
protected:
    const char *project_name;
    ETestSet *test_set;
    int line_number;
    
private:
    std::ifstream file;
    
};


#endif /* DefaultTestSetIterator_h */
