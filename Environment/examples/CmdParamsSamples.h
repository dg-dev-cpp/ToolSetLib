#ifndef TOOL_SET_LIB_ENVIRONMENT_CMDPARAMS_EXAMPLES_HEADER
#define TOOL_SET_LIB_ENVIRONMENT_CMDPARAMS_EXAMPLES_HEADER

#include "ToolSetLib/Environment/CmdParams.h"

#include <string>
#include <iostream>
#include <vector>


namespace ToolSetLib
{

namespace Environment
{

namespace Examples
{

using namespace ToolSetLib::Environment;

bool
CmdParamsSampleBase(int argc,const char* argv[])
{
    const std::string param1_key_str =  "--sample_param1";
    const std::string param2_key_str =  "--sample_param2";

    std::vector<CmdFindKeyParam> cmd_params_spec;
    cmd_params_spec.push_back(CmdFindKeyParam( param1_key_str, CmdKeyType::unique_required, CmdKeyValueType::string, "sample param1 description"));
    cmd_params_spec.push_back(CmdFindKeyParam( param2_key_str, CmdKeyType::unique_required, CmdKeyValueType::string,CmdKeyValueType::string, "sample param2 description"));

    CmdParams       cmd_params;
    std::string     error_string;

    if( false == cmd_params.GetCmdParams(   argc,
                                            argv,
                                            cmd_params_spec,
                                            &error_string))
    {
        std::cout<<"error: CmdParamsSampleBase(): fail to get command line params: "<<error_string<<""<<std::endl;

        std::cout<<" "<<std::endl;
        std::cout<<"command line params info:"<<std::endl;

        auto cmd_params_description = cmd_params.GetCmdParamsDescription(cmd_params_spec);

        for(auto& next_description: cmd_params_description)
        {
            std::cout<<"\t"<<next_description<<std::endl;
        }

        return false;
    }
    
    auto param1_value =     cmd_params.GetFrontKeyValue(param1_key_str);
    auto param2_value1 =    cmd_params.GetFrontKeyValue(param2_key_str, 0);
    auto param2_value2 =    cmd_params.GetFrontKeyValue(param2_key_str, 1);

    std::cout<<"info: CmdParamsSampleBase(): command line keys:"<<std::endl;
    std::cout<<"\t"<<"key: '"<<param1_key_str<<"', value: '"<<param1_value<<"'"<<std::endl;
    std::cout<<"\t"<<"key: '"<<param2_key_str<<"', values: '"<<param2_value1<<"' '"<<param2_value2<<"'"<<std::endl;

    return true;
}

}

}

}
#endif