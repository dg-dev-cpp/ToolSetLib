#ifndef TOOL_SET_LIB_WORK_THREADS_POOL_HEADER
#define TOOL_SET_LIB_WORK_THREADS_POOL_HEADER

#include "defines/WorkThreadsPoolDefines.h"

#include "ToolSetLib/Base/CallOnExitScope.h"

#include <thread>
#include <atomic>
#include <mutex>


namespace ToolSetLib
{

namespace Threads
{

template <  typename    DataElementType, 
            uint32_t    max_pool_threads =              4,
            uint32_t    initial_start_threads =         max_pool_threads,
            uint32_t    max_data_queue_size =           std::min((max_pool_threads*20), 1024),
            uint32_t    stop_thread_idle_time_sec =     60,     // value == 0: don`t stop idle threads 
            bool        is_idle_stop_last_thread =      false>
class WorkThreadsPool
{
private:

    std::atomic_uint32_t    m_is_pool_started;
    std::mutex              m_start_stop_mutex;
    std::atomic_int         m_is_need_stop;

    std::atomic_uint32_t    m_set_data_counter;

    // Threads primary data
	PoolThreadData<DataElementType> m_threads_data[max_pool_threads];

    // Threads queue data,
    // a little trick: create array, if max_data_queue_size is 0
    std::pair<ThreadsQueueDataState,DataElementType>    m_threads_queue_data[max_data_queue_size + 1]; 

    std::atomic_uint32_t    m_queue_elements_id_counter;

    std::atomic_int         m_active_threads;

public:

    class VersionInfo
    {
    public:
        static const uint32_t api =         1;
        static const uint32_t features =    1;
        static const uint32_t revision =    2;
    };


	bool
	Start()
	{
        try
        {
            std::lock_guard<std::mutex> scope_lock(m_start_stop_mutex);

            if( m_is_pool_started != 0)
                return true;

            for(auto& next_data: m_threads_data)
            {
                if(true == next_data.m_thread.joinable())
                    return false;

                next_data.Clear();
            }

            for(auto& next_queue_elem: m_threads_queue_data)
            {
                next_queue_elem.first.is_data_locked =  0;
                next_queue_elem.first.element_id =      0;
            }

            m_is_need_stop =            0;
            m_queue_elements_id_counter =  1;
            m_active_threads =          0;

            // Start work threads
            for(	uint32_t	count	=	0;
                    			count	<	initial_start_threads;
                    			count	+=	1)
            {
                auto& next_thread = m_threads_data[count];

                next_thread.m_thread = std::move(std::thread(&WorkThreadsPool::InternalPoolThreadFunction, this, count));
                next_thread.m_is_started =    1;
            }

            m_is_pool_started = 1;

            return true;
        }
        catch(...)
        {}

        return false;
	}


	bool
	Stop(bool is_process_remaining_data = true)
	{
        try
        {
            std::lock_guard<std::mutex> scope_lock(m_start_stop_mutex);

            m_is_need_stop = 1;

            for(; m_set_data_counter.load() != 0; this_thread::sleep_for(1ms))
            {
                // waiting for setting data complete 
            }

            for(auto& next_data: m_threads_data)
            {
                next_data.m_event.notify_all();

                try
                {
                    if( next_data.m_thread.joinable() == true)
                        next_data.m_thread.join();
                }
                catch(...)
                {}
            }

            if( is_process_remaining_data == true)
            {
                for(auto& thread_data: m_threads_data)
                {
                    if( 0 != thread_data.m_thread_base_data.first.is_data_set)
                    {
                        try
                        {
                            // Process the remaining base threads data elements

                            ProcessDataCallback(thread_data.m_thread_base_data.second);
                        }
                        catch(...)
                        {}

                        thread_data.m_last_processing_time =                time(0);
                        thread_data.m_thread_base_data.first.is_data_set =  0;
                    }
                }

                if( std::size(m_threads_queue_data) >= max_data_queue_size)
                {
                    // Processing the remaining queue data

                    std::pair<uint32_t,uint32_t> queue_data[max_data_queue_size];
                    // first: element id, second: element idx

                    uint32_t total_elements = 0;
                
                    for(uint32_t    count2	=	0;
           				            count2	<	(max_data_queue_size - 1);
           				            count2	+=	1)
                    {
                        // Find queue array elements data 

                        auto& next_data = m_threads_queue_data[count2];

                        if( next_data.first.element_id != 0)
                        {
                            queue_data[total_elements].first =  next_data.first.element_id;
                            queue_data[total_elements].second = count2;
                            total_elements += 1;
                        }
                    }

                    if( total_elements > 0)
                    {
                        std::sort( std::begin(queue_data), std::begin(queue_data) + total_elements,
                                [](std::pair<uint32_t,uint32_t>& first,std::pair<uint32_t,uint32_t>& second )->bool
                                {
                                    if( first.first < second.first)
                                        return true;

                                    return false;
                                });

                        for(uint32_t    count3	=	0;
           				                count3	<	total_elements;
           				                count3	+=	1)
                        {
                            // Process queue thread data in id ascending order

                            auto& next_data = m_threads_queue_data[ queue_data[count3].second];

                            if( 0 != next_data.first.element_id )
                            {
                                try
                                {
                                    ProcessDataCallback( next_data.second);
                                }
                                catch(...)
                                {}
                        
                                next_data.first.element_id =    0;
                            }
                        }
                    }
                }
            }

            m_is_pool_started = 0;

            return true;
        }
        catch(...)
        {}

        return false;
	}


    bool
    SetStopRequest()
	{
        try
        {
            std::lock_guard<std::mutex> scope_lock(m_start_stop_mutex);

            m_is_need_stop = 1;

            return true;
        }
        catch(...)
        {}

        return false;
	}


	bool
	CopyWorkData(   const   DataElementType&    thread_data,
                            bool                is_skip_if_queue_full = true,
                            uint32_t            max_wait_time_sec =     0) //value == 0: endless waiting
	{
        if( m_is_need_stop != 0 || m_is_pool_started == 0)
            return false;

        DataElementType copy_thread_data;

        try
        {
            copy_thread_data = thread_data;
        }
        catch(...)
        {
            return false;
        }

        return MoveWorkData(copy_thread_data, is_skip_if_queue_full, max_wait_time_sec);
	}


    bool
	MoveWorkData(   DataElementType&    thread_data,
                    bool                is_skip_if_queue_full = true,
                    uint32_t            max_wait_time_sec =     0) //value == 0: endless waiting
	{
        if( m_is_need_stop != 0 || m_is_pool_started == 0)
            return false;

        time_t      start_time =         0;
        uint32_t    check_time_count =   UINT32_MAX;

        for(	uint64_t	count	=	0;;
            				count	+=	1, check_time_count += 1)
        {
            bool is_queue_full = false;

            if( true == MoveWorkDataInternal(thread_data, is_queue_full))
                return true;

            if( is_skip_if_queue_full == true)
            {
                // It is possible not wait copy data
                return false;
            }

            if( is_queue_full == false)
            {
                // Threads queue is not full, internal error possible
                return false;
            }

            if( m_is_need_stop != 0 || m_is_pool_started == 0)
                break;

            if(     is_skip_if_queue_full   ==  false
                &&  max_wait_time_sec       >   0
                &&  check_time_count        >=  200) // check time every 200 cycles (cpu usage optimization)
            {
                check_time_count = 0;

                auto current_time = time(0);

                if( start_time == 0)
                    start_time = current_time;

                if( start_time > current_time)
                {
                    start_time = current_time;
                    break;
                }

                if( (current_time - start_time) > (time_t)max_wait_time_sec) 
                    return false;
            }

            if( count > (8*1024))
            {
                // Thread delay in long fail set data case (cpu usage optimization)
                this_thread::sleep_for(1ms);
            }
        }

        return false;
	}


    bool 
    IsStarted()
    {
        return m_is_pool_started;
    }


    uint32_t
    GetThreadsPrimaryDataCount()
    {
        uint32_t assigned_data = 0;

        for(auto& next_data: m_threads_data)
        {
            if( next_data.m_thread_base_data.first.is_data_set != 0)
                assigned_data += 1;
        }

        return assigned_data;
    }


    uint32_t
    GetThreadsQueueDataCount()
    {
        uint32_t assigned_queue = 0;

        for(auto& next_data: m_threads_queue_data)
        {
            if( next_data.first.element_id != 0)
                assigned_queue += 1;
        }

        return assigned_queue;
    }


    uint32_t
    GetActiveThreadsCount()
    {
        return m_active_threads.load();
    }
    
private:

    virtual void
	ProcessDataCallback(DataElementType& thead_data) = 0;

	bool
	InternalPoolThreadFunction( uint32_t pool_thread_idx)
	{
        if( pool_thread_idx >= std::size(m_threads_data))
            return false;

        if( m_is_need_stop != 0 )
            return false;
        
        m_active_threads += 1;

        ToolSetLib::Base::CallOnExitScope decrease_threads_count([&]()->void{ m_active_threads -= 1; });

		auto& thread_data = m_threads_data[pool_thread_idx];

        thread_data.m_last_processing_time = time(0);

        for(;;)
        {
            if( m_is_need_stop != 0 )
                break;

	        try
	        {
                if( 0 == thread_data.m_thread_base_data.first.is_data_set)
                {
                    std::unique_lock<std::mutex> locked(thread_data.m_event_mutex);

                    if( 0 == thread_data.m_thread_base_data.first.is_data_set)
                    {
                        //thread_data.m_event.wait( locked);
                        thread_data.m_event.wait_for( locked, 250ms);
                    }
                }
	        }
	        catch (...)
	        {}

            if( m_is_need_stop != 0)
                break;

            if( 0 != thread_data.m_thread_base_data.first.is_data_set )
            {
                try
                {
                    // Process base thread data element

                    ProcessDataCallback( thread_data.m_thread_base_data.second);
                }
                catch(...)
                {}

                thread_data.m_thread_base_data.first.is_data_set =  0;
                thread_data.m_last_processing_time =                time(0);
            }
            else
            {
                if( m_is_need_stop != 0)
                    break;

                // Check for max idle time expired

                bool is_can_stop_thread = true;

                if( stop_thread_idle_time_sec == 0)
                    is_can_stop_thread = false;

                if(     pool_thread_idx             ==  0 
                    &&  is_idle_stop_last_thread    ==  false)
                    is_can_stop_thread = false;

                if( is_can_stop_thread  ==  true )
                {
                    auto current_time = time(0);

                    if( thread_data.m_last_processing_time > current_time)
                        thread_data.m_last_processing_time = current_time;

                    if( (current_time - thread_data.m_last_processing_time) > stop_thread_idle_time_sec)
                    {
                        if( thread_data.m_set_data_count.fetch_add(1) ==  0)
                        {
                            // idle time expired, close thread

                            thread_data.m_is_started =              0;
                            thread_data.m_set_data_count.fetch_sub(1);

                            return true;
                        }

                        thread_data.m_set_data_count.fetch_sub(1);
                    }
                }
            }
            
            if( m_is_need_stop != 0)
                break;
            
            {
                // Process queue data array

                std::pair<uint32_t,uint32_t> queue_data[max_data_queue_size];
                // first: element id, second: element idx

                uint32_t total_elements = 0;
                
                for(uint32_t    count2	=	0;
           				        count2	<	(max_data_queue_size - 1);
           				        count2	+=	1)
                {
                    // Find queue array elements data

                    auto& next_data = m_threads_queue_data[count2];

                    auto element_id = next_data.first.element_id.load();

                    if( element_id != 0)
                    {
                        queue_data[total_elements].first =  element_id;
                        queue_data[total_elements].second = count2;
                        total_elements += 1;
                    }
                }

                if( total_elements > 0)
                {
                    std::sort( std::begin(queue_data), std::begin(queue_data) + total_elements,
                            [](std::pair<uint32_t,uint32_t>& first,std::pair<uint32_t,uint32_t>& second )->bool
                            {
                                if( first.first < second.first)
                                    return true;

                                return false;
                            });

                    for(uint32_t    count3	=	0;
           				            count3	<	total_elements;
           				            count3	+=	1)
                    {
                        // Process queue thread data in id value ascending order

                        auto& next_data = m_threads_queue_data[ queue_data[count3].second];

                        if( m_is_need_stop != 0)
                            break;

                        if( next_data.first.element_id != queue_data[count3].first)
                            continue;

                        if(next_data.first.is_data_locked.fetch_add(1) != 0)
                        {
                            next_data.first.is_data_locked -= 1;
                            continue;
                        }

                        ToolSetLib::Base::CallOnExitScope decrease_use_count([&]()->void{ next_data.first.is_data_locked -= 1; });

                        if( next_data.first.element_id == queue_data[count3].first)
                        {
                            try
                            {
                                ProcessDataCallback( next_data.second);
                            }
                            catch(...)
                            {}
                        
                            next_data.first.element_id =            0;
                        }
                    }

                    thread_data.m_last_processing_time =    time(0);
                }
            }
        }

        thread_data.m_is_started =             0;

		return true;
	}


    bool
    MoveWorkDataInternal( DataElementType& thread_data, bool& is_queue_full)
    {
        is_queue_full = false;

        m_set_data_counter += 1;
        ToolSetLib::Base::CallOnExitScope decrease_set_count([&]()->void { m_set_data_counter -= 1; });

        if( m_is_pool_started == 0 || m_is_need_stop != 0)
            return false;

        for(uint32_t    count	=	0;
           				count	<	std::size(m_threads_data);
           				count	+=	1)
        {
            // Find free threads data array element

            if( m_is_pool_started == 0 || m_is_need_stop != 0)
                return false;

            auto& next_state = m_threads_data[count];

            {
                {
                    //if( next_state.m_set_data_count != 0)
                    //    continue;

                    if( next_state.m_set_data_count.fetch_add(1) != 0)
                    {
                        next_state.m_set_data_count -= 1;
                        continue;
                    }
                }

                ToolSetLib::Base::CallOnExitScope decrease_use_count([&]()->void{ next_state.m_set_data_count -= 1; });

                if( next_state.m_is_started == 0)
                {
                    try
                    {
                        if( next_state.m_thread.joinable() == true)
                            next_state.m_thread.join();
                    } 
                    catch(...)
                    {
                        continue;
                    }

                    if( m_is_need_stop != 0 || m_is_pool_started == 0)
                        return false;

                    try
                    {
                        next_state.m_thread =       std::move(std::thread(&WorkThreadsPool::InternalPoolThreadFunction, this, count));
                        next_state.m_is_started =   1;
                    } 
                    catch(...)
                    {
                        continue;
                    }
                }

                if( next_state.m_thread_base_data.first.is_data_set.load() != 0)
                {
                    // Data already set
                    continue;
                }

                try
                {
                    next_state.m_thread_base_data.second = move(thread_data);
                }
                catch(...)
                {
                    return false;
                }

                try
                {
                    std::unique_lock<std::mutex> locked(next_state.m_event_mutex);
                    next_state.m_thread_base_data.first.is_data_set += 1;
                    next_state.m_event.notify_all();
                }
                catch(...)
                {
                    try
                    {
                        thread_data = move(next_state.m_thread_base_data.second);
                    }
                    catch(...)
                    {
                        return false;
                    }

                    continue;
                }
            }

            return true;
        }

        if(     max_data_queue_size             == 0
            ||  std::size(m_threads_queue_data) <  max_data_queue_size)
            return false;

        if( m_queue_elements_id_counter > POOL_QUEUE_ELEMENT_MAX_ID)
        {
            // Need to reassign all data queue elements ids

            ReassignDataQueueIds();
        }

        for(uint32_t    count2	=	0;
           				count2	<	(max_data_queue_size - 1);
           				count2	+=	1)
        {
            // Try to find free queue array element and set data

            if( m_is_need_stop != 0 || m_is_pool_started == 0 )
                return false;

            auto& next_data = m_threads_queue_data[count2];

            if( next_data.first.element_id != 0)
            {
                // Data is already set
                continue;
            }

            {
                if( next_data.first.is_data_locked.fetch_add(1) != 0)
                {
                    next_data.first.is_data_locked -= 1;
                    continue;
                }
            }

            ToolSetLib::Base::CallOnExitScope decrease_use_count([&]()->void{ next_data.first.is_data_locked -= 1; });

            if( next_data.first.element_id.load() != 0)
            {
                // Data is already set
                continue;
            }

            try
            {
                next_data.second = std::move(thread_data);
            }
            catch(...)
            {
                return false;
            }

            next_data.first.element_id = m_queue_elements_id_counter.fetch_add(1);

            return true;
        }

        is_queue_full = true;

		return false;
    }


    bool
    ReassignDataQueueIds()
    {
        try
        {
            if(     max_data_queue_size             == 0
                ||  std::size(m_threads_queue_data) <  max_data_queue_size)
            return false;

            static std::mutex scope_mutex;
            std::lock_guard<std::mutex> scope_lock(scope_mutex);

            if( m_queue_elements_id_counter <= POOL_QUEUE_ELEMENT_MAX_ID)
                return false;

            for(uint32_t    count	=	0;
           				    count	<	(max_data_queue_size - 1);
           				    count	+=	1)
            {
                // Lock all data queue elements

                auto& next_elem = m_threads_queue_data[count];

                for(	uint32_t	count2	=	0;;
                					count2	+=	1)
                {
                    if( next_elem.first.is_data_locked.fetch_add(1) == 0)
                        break;

                    next_elem.first.is_data_locked -= 1;

                    if( count2 > (2*1024*1024))
                        this_thread::sleep_for(1ms);

                    if(     m_is_need_stop      !=  0
                        ||  m_is_pool_started   ==  0)
                    {
                        for(uint32_t    count_unlock	=	0;
                                        count_unlock	<	count;
                                        count_unlock	+=	1)
                        {
                            auto& unlock_elem = m_threads_queue_data[count_unlock];

                            unlock_elem.first.is_data_locked -= 1;
                        }

                        return false;
                    }
                }
            }

            std::pair<uint32_t,uint32_t> elements_ids[max_data_queue_size-1];
            // first: element id, second: element idx

            uint32_t total_elements = 0;

            for(uint32_t    count3	=	0;
           				    count3	<	(max_data_queue_size - 1);
           				    count3	+=	1)
            {
                // Copy elements info to additional array

                auto& next_elem = m_threads_queue_data[count3];

                if( next_elem.first.element_id > 0)
                {
                    elements_ids[total_elements].first =    next_elem.first.element_id;
                    elements_ids[total_elements].second =   count3;

                    total_elements += 1;
                }
            }

            try
            {
                if( total_elements > 0)
                {
                    std::sort( std::begin(elements_ids), std::begin(elements_ids) + total_elements,
                            [](std::pair<uint32_t,uint32_t>& first,std::pair<uint32_t,uint32_t>& second )->bool
                            {
                                if( first.first < second.first)
                                    return true;

                                return false;
                            });


                    for(uint32_t    count_ids	=	0;
                				    count_ids	<	total_elements;
                				    count_ids	+=	1)
                    {
                        // Reassign elements ids in m_threads_queue_data array

                        auto elem_idx = elements_ids[count_ids].second;

                        m_threads_queue_data[elem_idx].first.element_id = count_ids + 1;
                    }
                }
            }
            catch(...)
            {}

            m_queue_elements_id_counter = total_elements + 1;

            if(m_queue_elements_id_counter == 0)
                m_queue_elements_id_counter = 1;

            for(uint32_t    count4	=	0;
           				    count4	<	(max_data_queue_size - 1);
           				    count4	+=	1)
            {
                // Unlock all data queue elements

                auto& next_elem = m_threads_queue_data[count4];

                next_elem.first.is_data_locked -= 1;
            }
        
            return true;
        }
        catch(...)
        {}

        return false;
    }

public:

	WorkThreadsPool()
	{
		static_assert( max_pool_threads > 0 && max_pool_threads <= 256,"max_pool_threads incorrect value");
        static_assert( max_data_queue_size <= (8*1024),"max_data_queue_size incorrect value");
        static_assert( (max_data_queue_size * sizeof(DataElementType)) < (128*1024*1024),  "threads pool queue data size is too large");
        static_assert( max_pool_threads >=  initial_start_threads, "max_pool_threads < initial_start_threads");

        m_is_pool_started =     0;
        m_is_need_stop =        0;
        m_set_data_counter =    0;
        m_queue_elements_id_counter =    1;

		for(auto& next_state: m_threads_data)
		{
			next_state.Clear();
		}

        for(auto& next_queue_elem: m_threads_queue_data)
        {
            next_queue_elem.first.is_data_locked =  0;
            next_queue_elem.first.element_id =      0;
        }
	}


    virtual
	~WorkThreadsPool()
	{
		Stop();
	}

    WorkThreadsPool(const WorkThreadsPool &) =              delete;
    WorkThreadsPool(WorkThreadsPool &&) =                   delete;
    WorkThreadsPool& operator=(const WorkThreadsPool &) =   delete;
    WorkThreadsPool& operator=(WorkThreadsPool &&) =        delete;
};

}

}
#endif