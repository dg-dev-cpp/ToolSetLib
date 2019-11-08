#ifndef TOOL_SET_LIB_THREADS_WORK_THREAD_HEADER
#define TOOL_SET_LIB_THREADS_WORK_THREAD_HEADER

#include "defines/WorkThreadDefines.h"

#include <thread>
#include <mutex>
#include <atomic>

namespace ToolSetLib
{

namespace Threads
{

class WorkThread
{
	// Base class to create work thread

private:

    std::atomic_int m_is_need_stop;
    std::atomic_int m_is_started;
    std::mutex      m_start_stop_mutex;

    std::thread     m_work_thread;

    bool
    InternalThreadFunction();

public:

    class VersionInfo
    {
    public:
        static const uint32_t api =         1;
        static const uint32_t features =    2;
        static const uint32_t revision =    2;
    };


    bool
    Start();

    bool
    Stop();

    void
    SetStopRequest();

    bool
    IsThreadStarted();

    bool 
    IsNeedStopThread();

    WorkThread();

    virtual ~WorkThread();

    WorkThread(const WorkThread &) =                delete;
    WorkThread(WorkThread &&) =                     delete;
    WorkThread & operator=(const WorkThread &) =    delete;
    WorkThread & operator=(WorkThread &&) =         delete;

protected:

    const char*     mp_last_error;

    virtual bool
    WorkThreadFunction() = 0;
};

}

}
#endif