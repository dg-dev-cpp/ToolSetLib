#ifndef TOOL_SET_LIB_ENCODING_HEADER
#define TOOL_SET_LIB_ENCODING_HEADER

#include "Encoding/Base64.h"
#include "Encoding/HexString.h"


namespace ToolSetLib
{

namespace Encoding
{

class VersionInfo
{
public:
    static const uint32_t api =         0;
    static const uint32_t features =    2;
    static const uint32_t revision =    3;
};

}

}

#endif