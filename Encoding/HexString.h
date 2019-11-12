#ifndef TOOL_SET_LIB_ENCODING_HEXSTRING_HEADER
#define TOOL_SET_LIB_ENCODING_HEXSTRING_HEADER

#include <vector>
#include <string>

#include "defines/HexStringDefines.h"

namespace ToolSetLib
{

namespace Encoding
{

class HexString
{
public:

    class VersionInfo
    {
    public:
        static const uint32_t api =         0;
        static const uint32_t features =    1;
        static const uint32_t revision =    3;
    };

    std::string m_last_error;

    std::string
    Encode(const std::string& source_str, bool is_upper_case = true);

    bool
    Encode(const    char*           p_data,
                    uint64_t        data_size,
                    std::string&    encoded_string,
                    bool            is_upper_case = true);

    bool
    Decode(const    std::string&        encoded_string,
                    std::vector<char>&    result_data);

private:

    bool
    HexCharToNum(unsigned char char_text, unsigned char& hex_value);
};

}

}


#endif