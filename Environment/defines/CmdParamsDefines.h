#ifndef TOOL_SET_LIB_ENVIRONMENT_CMDPARAMS_DEFINES
#define TOOL_SET_LIB_ENVIRONMENT_CMDPARAMS_DEFINES

#include <string>
#include <vector>
#include <iostream>

namespace ToolSetLib
{

namespace Environment
{

enum class CmdKeyValueType
{
    undefined = 0,
    string,
    int32
};


static std::string
GetKeyValueTypeString(CmdKeyValueType value_type)
{
    switch(value_type)
    {
    case CmdKeyValueType::undefined:return "any";break;
    case CmdKeyValueType::string:   return "string";break;
    case CmdKeyValueType::int32:    return "int32";break;
    default:                        return "unknown type"; break;
    }

    return "";
}


enum CmdKeyType
{
    undefined = 0,
    unique,
    required,
    unique_required
};

class CmdKeyValue
{
public:

    CmdKeyValueType type;
    std::string     value_str;
};

class CmdKey
{
public:

    std::string     key;
    CmdKeyType      key_type;
    std::vector<CmdKeyValue> values;
};

class CmdFindKeyParam
{
public:

    std::string                     key;
    CmdKeyType                      key_type;
    std::vector<CmdKeyValueType>    values_types;

    std::string                     description;

    CmdFindKeyParam()
    {
        key =       "";
        key_type =  CmdKeyType::undefined;
        values_types.clear();
        description.clear();
    }

    CmdFindKeyParam( std::string a_key, std::string a_description = "")
    {
        key =       a_key;
        key_type =  CmdKeyType::undefined;
        values_types.clear();
        description = a_description;
    }

    CmdFindKeyParam(std::string     a_key, 
                    CmdKeyValueType a_param_type, 
                    std::string     a_description = "")
    {
        key =       a_key;
        key_type =  CmdKeyType::undefined;
        values_types.clear();
        values_types.push_back(a_param_type);

        description = a_description;
    }

    CmdFindKeyParam(std::string     a_key,
                    CmdKeyValueType a_param_type, 
                    CmdKeyValueType a_param2_type,
                    std::string     a_description = "")
    {
        values_types.clear();

        key =       a_key;
        key_type =  CmdKeyType::undefined;
        values_types.push_back(a_param_type);
        values_types.push_back(a_param2_type);

        description = a_description;
    }

    CmdFindKeyParam(std::string    a_key,
                    CmdKeyType    a_key_type,
                    std::string a_description = "")
    {
        key =           a_key;
        key_type =      a_key_type;
        values_types.clear();

        description =   a_description;
    }

    CmdFindKeyParam(std::string        a_key,
                    CmdKeyType        a_key_type, 
                    CmdKeyValueType a_param_type,
                    std::string     a_description = "")
    {
        values_types.clear();

        key =       a_key;
        key_type =  a_key_type;
        values_types.push_back(a_param_type);

        description = a_description;
    }

    CmdFindKeyParam(std::string     a_key,
                    CmdKeyType      a_key_type, 
                    CmdKeyValueType a_param_type,
                    CmdKeyValueType a_param2_type,
                    std::string     a_description = "")
    {
        values_types.clear();

        key =           a_key;
        key_type =      a_key_type;
        values_types.push_back(a_param_type);
        values_types.push_back(a_param2_type);

        description =   a_description;
    }
};

}

}

#endif