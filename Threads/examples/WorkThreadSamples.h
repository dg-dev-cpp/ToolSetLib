#ifndef TOOL_SET_LIB_WORK_THREAD_EXAMPLES_HEADER
#define TOOL_SET_LIB_WORK_THREAD_EXAMPLES_HEADER

#include "ToolSetLib/Threads/WorkThread.h"

#include <thread>


namespace ToolSetLib
{

namespace Threads
{

namespace Examples
{

// Create work thread class

class SampleThread: public ToolSetLib::Threads::WorkThread
{
	virtual bool
	WorkThreadFunction() override
	{
		for(;IsNeedStopThread() == false; std::this_thread::sleep_for(1ms))
		{
			// work thread code
	    }

		return true;
	}

};


void
WorkThreadSampleBase()
{
    SampleThread sample_thread;

    if(sample_thread.Start() == false)
    {
        // error: fail to start sample thread
        return;
    }

    // success: start sample thread complete

    std::this_thread::sleep_for(10s);

    // stop work thread
    sample_thread.Stop();
}

}

}

}
#endif