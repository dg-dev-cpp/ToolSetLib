#ifndef TOOL_SET_LIB_BASE_CALLONEXITSCOPE_HEADER
#define TOOL_SET_LIB_BASE_CALLONEXITSCOPE_HEADER

#include <functional>

// Class for call given function at the time of exit scope ( through the call ~CallOnExitScope())
// Usage sample: CallOnExitScope close_event( [&]()->void{ CloseHandle(g_start_event); g_start_event = 0; });

namespace ToolSetLib
{

namespace Base
{

class CallOnExitScope
{
    std::function<void(void)> m_function;

public:

    class VersionInfo
    {
    public:
        static const uint32_t api =         1;
        static const uint32_t features =    1;
        static const uint32_t revision =    2;
    };


    CallOnExitScope(std::function<void(void)> a_function)
    {
        m_function = a_function;
    }

    ~CallOnExitScope()
    {
        if( m_function != nullptr)
            m_function();
    }

    CallOnExitScope(const CallOnExitScope&) = delete;
    CallOnExitScope& operator=(const CallOnExitScope&) = delete;
};

}

}

#endif