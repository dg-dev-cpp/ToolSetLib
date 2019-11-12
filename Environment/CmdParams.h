#ifndef TOOL_SET_LIB_ENVIRONMENT_CMDPARAMS_HEADER
#define TOOL_SET_LIB_ENVIRONMENT_CMDPARAMS_HEADER

#include "defines/CmdParamsDefines.h"

#include "stdint.h"

namespace ToolSetLib
{

namespace Environment
{

class CmdParams
{
public:

    class VersionInfo
    {
    public:
        static const uint32_t api =         2;
        static const uint32_t features =    4;
        static const uint32_t revision =    8;
    };

    std::vector<CmdKey> cmd_keys_found;

    bool
    GetCmdParams(   uint32_t                        total_params,
            const   char*                           p_params_strings[],
                    std::vector<CmdFindKeyParam>&   find_params,
                    std::string*                    p_error_str = 0);

    bool
    IsKeyPresent(const std::string& key_str);

    uint32_t
    GetKeyCount(const std::string& key_str);

    std::pair<std::string, std::vector<std::string>>
    GetKeyValues(const std::string& key_str, uint32_t key_data_idx = 0);

    std::string
    GetFrontKeyValue(const std::string& a_key_str, uint32_t a_value_idx = 0);

    unsigned long long
    GetFrontKeyValueAsUllong(const  std::string&    a_key_str, 
                                    uint32_t        a_value_idx =       0,
                                    uint32_t        a_return_default =  0);

    std::vector<std::string>
    GetCmdParamsDescription(const std::vector<CmdFindKeyParam>& a_cmd_params_spec);
};

}

}
#endif