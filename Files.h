#ifndef TOOL_SET_LIB_FILESYSTEM_HEADER
#define TOOL_SET_LIB_FILESYSTEM_HEADER

#include "Files/FileBaseFunctions.h"

namespace ToolSetLib
{

namespace Files
{

class VersionInfo
{
public:
    static const uint32_t api =         0;
    static const uint32_t features =    1;
    static const uint32_t revision =    3;
};

}

}

#endif