#ifndef TOOL_SET_LIB_BASE_COMPONENTS_HEADER
#define TOOL_SET_LIB_BASE_COMPONENTS_HEADER

#include "Base/CallOnExitScope.h"
#include "Base/IntegerValue.h"

namespace ToolSetLib
{

namespace Base
{

class VersionInfo
{
public:
    static const uint32_t api =         0;
    static const uint32_t features =    2;
    static const uint32_t revision =    2;
};

}

}

#endif