#ifndef TOOL_SET_LIB_ENCODING_BASE64_HEADER
#define TOOL_SET_LIB_ENCODING_BASE64_HEADER

#include <vector>
#include <string>

#include "defines/Base64Defines.h"

namespace ToolSetLib
{

namespace Encoding
{

class Base64
{
public:

    class VersionInfo
    {
    public:
        static const uint32_t api =         0;
        static const uint32_t features =    1;
        static const uint32_t revision =    5;
    };

    std::string m_last_error;

    std::string
    Encode(const std::string& source_str);

    bool
    Encode(const    char*           p_data,
                    uint64_t        data_size,
                    std::string&    encoded_string);

    bool
    Decode(const    std::string&        encoded_string,
                    std::vector<char>&    result_data);

    uint64_t
    EstimateDecodedBase64DataSize(const std::string& data);

private:

    static inline bool is_base64(unsigned char c)
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
};

}

}


#endif