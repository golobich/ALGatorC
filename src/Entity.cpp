//
//  Entity.cpp
//  AlgatorC
//
//  Created by Tadej Golobič on 28/06/15.
//  Copyright © 2015 Tadej Golobič. All rights reserved.
//

#include "Entity.hpp"
Entity::Entity()
{
    entity_rootdir = "?";
    entity_name = "?";
}

Entity::Entity(const std::string &entity_id, std::vector<std::string> &field_names)
{
    init(entity_id, field_names);
}

void Entity::init(const std::string &entity_id, std::vector<std::string> &field_names)
{
    this->entity_id = entity_id;
    this->entity_name = "?";
    this->entity_file_ext = "?";
    this->entity_rootdir = "?";
    
    for (unsigned int i = 0; i<field_names.size(); i++)
    {
        this->field_names.insert(field_names.at(i));
        //printf("%s\n", field_names.at(i).c_str());
    }
}

std::string Entity::get_name()
{
    return entity_name;
}

bool Entity::init_from_file(const std::string &fname)
{
    size_t seperator_pos = fname.find_last_of("/");
    size_t dot_pos = fname.find_last_of(".");
    entity_file_ext = fname.substr(dot_pos);
    entity_rootdir = fname.substr(0, seperator_pos);
    entity_name = fname.substr(seperator_pos, dot_pos);
    
    std::ifstream file(fname);
    if (file.is_open())
    {
        std::string content = "";
        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            content += line;
        }
        file.close();
        
        if (entity_id == "")
        {
            return init_from_json(content);
        }
        JSONValue *value = JSON::Parse(content.c_str());
        if (value != NULL)
        {
            JSONObject object;
            if (value->IsObject())
            {
                object = value->AsObject();
                std::wstring tmp = from_string(entity_id);
                if (object.find(tmp.c_str()) != object.end())
                {
                    init_from_json(object[tmp.c_str()]->AsObject());
                }
                else
                {
                    return false;
                }
            }else
            {
                std::cout << __FILE__ << "(" << __LINE__ << ") : Content is not JSON object";
                return false;
            }
        }
    }else
    {
        std::cout << __FILE__ << "(" << __LINE__ << ") : Cannot open file => " << fname;
        return false;
    }
    
    return true;
}

bool Entity::init_from_json(JSONObject object)
{
    std::set<std::string>::iterator it;
    for (it = field_names.begin(); it != field_names.end(); it++)
    {
        std::string key = *it;
        std::wstring ws;
        for (unsigned int i = 0; i<key.length(); i++)
            ws += wchar_t(key[i]);
        if (object.find(ws) != object.end())
        {
            std::string value = "";
            if (object[ws]->IsString())
            {
                value = from_wstring(object[ws]->AsString());
            }
            else if (object[ws]->IsNumber())
            {
                value = std::to_string(object[ws]->AsNumber());
            }
            else if (object[ws]->IsBool())
            {
                value = object[ws]->AsBool() ? "1" : "0";
            }
            fields.insert(std::pair<std::string, std::string>(key, value));
        }
    }
    return true;
}

bool Entity::init_from_json(const std::string &content)
{
    JSONValue *value = JSON::Parse(content.c_str());
    if (value == NULL)
    {
        std::cout << __FILE__ << "(" << __LINE__ << ") : JSONValue is null";
        return false;
    }
    JSONObject object;
    if (value->IsObject())
    {
        object = value->AsObject();
        init_from_json(object);
    }else
    {
        std::cout << __FILE__ << "(" << __LINE__ << ") : Value is not a JSON Object";
    }
    return false;
}

const std::string Entity::get(const std::string &field) const
{
    if (field_names.find(field) != field_names.end())
    {
        return fields.at(field);
    }
    return unknown_value;
}

void Entity::set(const std::string &key, const std::string &value)
{
    fields.insert(std::pair<std::string, std::string>(key, value));
}

void Entity::set_representatives(const std::vector<std::string> repr)
{
    for (unsigned int i = 0; i<repr.size(); i++)
    {
        representatives.push_back(repr.at(i));
    }
}

std::string Entity::print()
{
    std::string desc = get_name();
    for (unsigned int i = 0; i<representatives.size(); i++)
    {
        if (desc.empty())
            desc += "";
        else
            desc += ", ";
        desc += representatives.at(i) + " = " + fields.at(representatives.at(i));
    }
    return desc;
}


std::wstring Entity::from_string(std::string &content)
{
    std::wstring result;
    for (unsigned int i = 0; i<content.length(); i++)
        result += wchar_t(content[i]);
    return result;
}

std::string Entity::from_wchar(const wchar_t *con)
{
    std::wstring str(con);
    return from_wstring(str);
}

std::string Entity::from_wstring(std::wstring con)
{
    std::string s(con.begin(), con.end());
    return s;
}
