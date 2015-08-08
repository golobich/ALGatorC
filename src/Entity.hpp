//
//  Entity.h
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#ifndef Entity_h
#define Entity_h

#include <set>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include "JSON.hpp"
class Entity
{
    //TODO to JSON string, get string array
public:
    //! The folder of the entity file (if entity was read from a file), empty otherwise
    std::string entity_rootdir;
    
    //! Default constructor
    Entity();
    
    //! Adds vector values to field_names and sets entity_id
    //! \param entity_id
    //! \param vector_of_field_names
    Entity(const std::string&, std::vector<std::string>&);
    
    //! Adds vector values to field_names and sets entity_id
    //! \param entity_id
    //! \param vector_of_field_names
    void init(const std::string&, std::vector<std::string>&);
    
    //! Reads a JSON file. If entity_id is empty, then the whole file represents a JSON
    //! object to be read (i.e. the file contains only this object); else, the
    //! file contains a JSON object in which the field with key=entity_id is read.
    //! \param path_to_entity_file
    bool init_from_file(const std::string&);
    
    bool init_from_json(JSONObject);
    
    //! Method reads a JSON object and fills the map with correspnding values.
    //! \param json
    bool init_from_json(const std::string&);
    
    //! Convert string to wchar_t
    //! \param string
    //! \return wstring
    std::wstring from_string(std::string&);
    
    //! Convert wchar_t to string
    //! \param wchar_t
    //! \return string
    std::string from_wchar(const wchar_t*);
    
    //! Convert wstring to string
    //! \param wstring
    //! \return string
    std::string from_wstring(std::wstring);
    
    //! Get value of key from fields
    //! \param key
    //! \return string
    const std::string get(const std::string &) const;
    
    //! Get entity name
    //! \return string
    std::string get_name();
    
    //!
    std::string print();
    
    //! Add key-value pair to fields
    //! \param string key
    //! \param string value
    void set(const std::string&, const std::string&);
    
    //! Add fields that represent this entity to list
    //! \param vector
    void set_representatives(const std::vector<std::string>);
    
protected:
    //! The ID of an entity in an JSON file
    std::string entity_id;
    std::set<std::string> field_names;
    std::map<std::string, std::string> fields;
    
    //! A list of representative fields (fields that represent this entity)
    //! this list is used to construct print message
    std::vector<std::string> representatives;
    
    
private:
    std::string unknown_value = "?";
    
    //! The prefix of the name of the file from which the entity was read (if entity was read from a file), empty
    std::string entity_name;
    
    //! The extension of the file where entity was written
    std::string entity_file_ext;
    
};

#endif /* Entity_h */
