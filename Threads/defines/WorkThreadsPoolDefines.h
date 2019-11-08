#ifndef TOOL_SET_LIB_THREADS_WORK_THREADS_POOL_DEFINES
#define TOOL_SET_LIB_THREADS_WORK_THREADS_POOL_DEFINES

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace ToolSetLib
{

namespace Threads
{

#define POOL_QUEUE_ELEMENT_MAX_ID ((uint32_t)UINT32_MAX - uint32_t(16*1024*1024))


class ThreadBaseDataState
{
public:
    std::atomic_uint32_t    is_data_set;    // value == 0: data not set
};


class ThreadsQueueDataState
{
public:
    std::atomic_uint32_t    element_id;     // value == 0: data not set
    std::atomic_uint32_t    is_data_locked; // value == 0: the data is not locked 
};


template<typename   DataElementType>
class PoolThreadData
{
public:
	std::atomic_int         m_is_started;        // value == 0: thread is not started
    std::thread				m_thread;

	std::atomic_uint32_t    m_set_data_count;  

    std::condition_variable	m_event;            // locked by m_event_mutex
	std::mutex				m_event_mutex;

    time_t                  m_last_processing_time;  

    std::pair<ThreadBaseDataState, DataElementType>      m_thread_base_data;

    PoolThreadData()
    {
        Clear();
    }

    void
    Clear()
    {
        m_is_started =	            0;
        m_set_data_count =          0;
        m_last_processing_time =    0;
        m_thread_base_data.first.is_data_set = 0;
    }
};
}
}
#endif