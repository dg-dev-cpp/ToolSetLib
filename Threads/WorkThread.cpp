#include "WorkThread.h"

using namespace std;

namespace ToolSetLib
{

namespace Threads
{

bool
WorkThread::Start()
{
    mp_last_error = "";

	try
	{
		std::lock_guard<std::mutex> scope_lock(m_start_stop_mutex);

		if( m_is_started != 0)
		{
            mp_last_error = "WorkThread::Start(): thread is already running";
			return true;
		}

        try
        {
            if( m_work_thread.joinable() == true)
            {
                m_work_thread.join();
            }
        }
        catch(...)
        {}

		m_is_need_stop = 0;

        try
        {
            m_work_thread = std::thread( &WorkThread::InternalThreadFunction, this);
        }
        catch(...)
        {
            mp_last_error = "WorkThread::Start(): failed to start std::thread";
            return false;
        }

		m_is_started = 1;

		return true;
	}
	catch(...)
	{
        mp_last_error = "WorkThread::Start(): lock mutex exception";
    }

	return false;
}


bool
WorkThread::Stop()
{
    mp_last_error = "";

	try
	{
		std::lock_guard<std::mutex> scope_lock(m_start_stop_mutex);

		m_is_need_stop = 1;

		try
		{
			if (m_work_thread.joinable() == true)
			    m_work_thread.join();
		}
		catch(...)
		{}

		m_is_started = 0;

		return true;
	}
	catch(...)
	{
        mp_last_error = "WorkThread::Stop(): lock mutex exception";
    }

	return false;
}


void
WorkThread::SetStopRequest()
{
    mp_last_error = "";

    try
	{
		std::lock_guard<std::mutex> scope_lock(m_start_stop_mutex);

		m_is_need_stop = 1;
	}
	catch(...)
	{
        mp_last_error = "WorkThread::SetStopRequest(): lock mutex exception";
    }
}


bool
WorkThread::InternalThreadFunction()
{
    mp_last_error = "";

	try
	{
		WorkThreadFunction();
	}
	catch(...)
	{}

	m_is_started = 0;

	return true;
}


bool
WorkThread::IsThreadStarted()
{
    mp_last_error = "";

	if( m_is_started.load() == 0)
		return false;

	return true;
}


bool
WorkThread::IsNeedStopThread()
{
    mp_last_error = "";

	if( m_is_need_stop.load() == 0)
		return false;

	return true;
}


WorkThread::WorkThread()
{
	m_is_started =      0;
	m_is_need_stop =    0;
    mp_last_error =     "";
}


WorkThread::~WorkThread()
{
	Stop();
}

}

}