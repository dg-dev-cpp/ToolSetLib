#ifndef TOOL_SET_LIB_THREADS_HEADER
#define TOOL_SET_LIB_THREADS_HEADER

#include "Threads/WorkThread.h"
#include "Threads/WorkThreadsPool.h"

namespace ToolSetLib
{

namespace Threads
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