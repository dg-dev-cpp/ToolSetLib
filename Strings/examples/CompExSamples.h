#ifndef TOOL_SET_LIB_COMPEX_EXAMPLES_HEADER
#define TOOL_SET_LIB_COMPEX_EXAMPLES_HEADER

#include "ToolSetLib/Strings/CompEx.h"

#include <string>
#include <iostream>


namespace ToolSetLib
{

namespace Strings
{

namespace Examples
{

using namespace ToolSetLib::Strings;

bool
CompExSampleBase()
{
    string test_text = 
        "sample_value_no_params;sample_value1{val1};sample_value2{val2};sample_value3{val3};sample_value4{val4}"
        "\r\nend_header_params\r\n;sample_value5{val5};sample_value6{val6};";

    vector<CxSearchSample> test_cx = {  {front, 0, {"sample_value"}, 54, {"\r\nend_header_params\r\n"}, false},
                                        {front, 12, {"{"}, 10, {"sample_value", ";"}, false},
                                        {front, 32, {"}"}, 1, {";", "{"}, false},
                                        {front, 40, {" ", ";"}, 1, {}, true}};

    CompEx compex;

    std::vector<std::vector<CxFoundSamplePos>>    found_samples_pos;

    if( false == compex.Find(   test_cx,
                                test_text.data(),
                                test_text.size(),
                                0,
                                found_samples_pos))
    {
        cout<<"error: CompExSampleBase(): fail to find compex sample: '"<<compex.m_last_error<<"'"<<endl;
        return 0;
    }

    cout<<"success: CompExSampleBase(): compex samples found, total results: "<<found_samples_pos.size()<<endl;

    string internal_string;

    uint32_t strings_pos = 1;

    cout<<"info: CompExSampleBase(): samples results: internal strings for pos "<<2<<" idx:"<<endl;

    for(auto& next_found_sample: found_samples_pos)
    {
        if( false == compex.GetInternalString(  next_found_sample, 
                                                strings_pos, 
                                                test_text.data(),
                                                test_text.size(),
                                                internal_string))
        {
            cout<<"\terror: '"<<compex.m_last_error<<"'"<<endl;
            continue;
        }

        cout<<"\t'"<<internal_string<<"'"<<endl;
    }

    return true;
}

}

}

}
#endif