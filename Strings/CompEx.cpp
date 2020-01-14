#include "CompEx.h"

#include <cstring>


namespace ToolSetLib
{

namespace Strings
{

    using namespace std;

    bool
    CompEx::Find(   const   vector<CxSearchSample>&             search_samples,
                    const   char*                               p_data,
                            uint64_t                            data_size,
                            uint32_t                            max_found_samples,
                            vector<vector<CxFoundSamplePos>>&   found_samples_pos)
    {
        found_samples_pos.clear();
        m_last_error.clear();

        try
        {
            // Check method params values

            if( search_samples.size()   ==  0)
            {
                m_last_error = "incorrect method param: no search samples";
                return false;
            }

            if( search_samples.size()   >   MAX_COMPEX_SAMPLES)
            {
                m_last_error = "incorrect method param: too many search samples, max: " + to_string(MAX_COMPEX_SAMPLES);
                return false;
            }

            if( p_data == nullptr)
            {
                m_last_error = "incorrect method param: p_data is null";
                return false;
            }

            if( data_size ==  0)
            {
                m_last_error = "incorrect method param: data_size is 0";
                return false;
            }

            if( data_size  >=  MAX_COMPEX_FOUND_DATA_SIZE)
            {
                m_last_error = "incorrect method param: data_size too large, max: " + to_string(MAX_COMPEX_FOUND_DATA_SIZE) + " bytes";
                return false;
            }

            for(    uint32_t    count    =    0;
                                count    <    search_samples.size();
                                count    +=    1)
            {
                auto& next_elem = search_samples[count];

                if(     next_elem.search_direction != front
                    &&  next_elem.search_direction != back)
                {
                    m_last_error = "incorrect method param: unknown search_direction, sample idx: " + to_string(count);
                    return false;
                }

                if( next_elem.sample_strings.size() == 0)
                {
                    m_last_error = "incorrect method param: no sample_strings in search_sample, sample idx: " + to_string(count);
                    return false;
                }

                for( auto& next_sample: next_elem.sample_strings)
                {
                    if(next_sample.size() == 0)
                    {
                        m_last_error = "incorrect method param: empty sample string, search_sample idx: " + to_string(count);
                        return false;
                    }

                    if(     next_elem.max_search_positions  >   0
                        &&  next_sample.size()              >   next_elem.max_search_positions)
                    {
                        m_last_error = "incorrect method param: sample string length is greater than the max_search_positions" 
                                          ", search_sample idx: " + to_string(count)
                                        + ", sample: '" + next_sample + "'"
                                        + ", max_search_positions: " + to_string(next_elem.max_search_positions);
                        return false;
                    }
                }

                for( auto& next_stop: next_elem.stop_strings)
                {
                    if( next_stop.size() == 0)
                    {
                        m_last_error = "incorrect method param: empty stop string, search_sample idx: " + to_string(count);
                        return false;
                    }

                     if(    next_elem.max_search_positions  >   0
                        &&  next_stop.size()                >   next_elem.max_search_positions)
                    {
                        m_last_error =    "incorrect method param: stop string length is greater than the max_search_positions"
                                          ", search_sample idx: " + to_string(count)
                                        + ", stop string: '" + next_stop + "'"
                                        + ", max_search_positions: " + to_string(next_elem.max_search_positions);
                        return false;
                    }
                }
            }
        }
        catch(...)
        {
            return false;
        }

        vector<CxFoundSamplePos> current_found_sample;

        if( false == FindInternal(  search_samples,
                                    0,
                                    p_data,
                                    data_size,
                                    max_found_samples,
                                    current_found_sample,
                                    found_samples_pos))
        {
            found_samples_pos.clear();
            return false;
        }

        return true;
    }


    bool
    CompEx::FindInternal(   const   vector<CxSearchSample>&             search_samples,
                                    uint64_t                            current_sample_idx,
                            const   char*                               p_data,
                                    uint64_t                            data_size,
                                    uint32_t                            max_found_samples,
                                    vector<CxFoundSamplePos>&           current_found_sample,
                                    vector<vector<CxFoundSamplePos>>&   search_samples_pos)
    {
        try
        {
            if(     max_found_samples           >   0
                &&  search_samples_pos.size()   >= max_found_samples)
                return true;

            if( current_sample_idx >= search_samples.size())
            {
                m_last_error = "internal error: incorrect method param: ( current_sample_idx  >=  search_samples.size())";
                return false;
            }

            if( data_size == 0)
            {
                m_last_error = "internal error: incorrect method param: ( data_size == 0)";
                return false;
            }

            if( p_data == nullptr)
            {
                m_last_error = "internal error: incorrect method param: ( p_data == nullptr)";
                return false;
            }

            auto& current_sample = search_samples[current_sample_idx];

            uint64_t    search_pos_idx =            0;
            bool        is_search_pos_range_out =   false;

            if( current_sample_idx == 0)
            {
                if(current_sample.search_direction == front)
                    search_pos_idx = 0;
                else
                {
                    if(current_sample.search_direction == back)
                        search_pos_idx = data_size - 1;
                    else
                    {
                        m_last_error = "internal error: unknown search direction";
                        return false;
                    }
                }
            }
            else
            {
                auto prev_sample_idx = current_sample_idx - 1;

                if( prev_sample_idx >= current_found_sample.size())
                {
                    m_last_error = "internal error: ( prev_sample_idx >= current_found_sample.size())";
                    return false;
                }

                auto& prev_sample_pos = current_found_sample[prev_sample_idx];

                if( current_sample.search_direction == front)
                {
                    search_pos_idx = prev_sample_pos.begin_idx + prev_sample_pos.len;

                    if(search_pos_idx >= data_size)
                        is_search_pos_range_out = true;
                }
                else
                {
                    if(current_sample.search_direction == back)
                    {
                        if( prev_sample_pos.begin_idx == 0)
                            is_search_pos_range_out = true;

                        search_pos_idx = prev_sample_pos.begin_idx - 1;

                        if( search_pos_idx >= data_size)
                            is_search_pos_range_out = true;
                    }
                    else
                    {
                        m_last_error = "internal error: unknown search direction-2";
                        return false;
                    }
                }
            }

            bool is_stop_found =    false;

            uint32_t total_found_samples = 0;

            if( is_search_pos_range_out == false )
            for(    uint64_t    count_pos   =   0;;
                                count_pos   +=  1)
            {
                if( current_sample.max_search_positions > 0)
                    if( count_pos >= current_sample.max_search_positions )
                        break;

                if( current_sample.max_found_samples > 0)
                    if( total_found_samples >= current_sample.max_found_samples)
                        break;

                uint64_t data_idx;
                
                if( current_sample.search_direction == front)
                {
                    data_idx = search_pos_idx + count_pos;

                    if( data_idx >= data_size)
                        break;
                }
                else
                {
                    if( current_sample.search_direction == back)
                    {
                        if(count_pos > search_pos_idx)
                            break;

                        data_idx = search_pos_idx - count_pos;
                    }
                    else
                    {
                        m_last_error = "internal error: unknown search direction, sample_idx: " + to_string(current_sample_idx);
                        return false;
                    }
                }


                for(auto& next_stop: current_sample.stop_strings)
                {
                    // Search current sample stop strings 
                    
                    if(     next_stop.size()                ==  0
                        ||  (next_stop.size() + data_idx)   >   data_size)
                        continue;

                    if(     current_sample.max_search_positions     >   0
                        &&  current_sample.search_direction         ==  front
                        &&  (count_pos + next_stop.size())          >   current_sample.max_search_positions)
                        continue;

                    if(     current_sample.max_search_positions     >   0
                        &&  current_sample.search_direction         ==  back
                        &&  (data_idx + next_stop.size())           >   (search_pos_idx + 1))
                        continue;

                    if(0 == memcmp(next_stop.data(), &p_data[data_idx], next_stop.size()))
                    {
                        is_stop_found = true;
                        break;
                    }
                }
                

                if( is_stop_found == true)
                    break;

                for(auto& next_search_data: current_sample.sample_strings)
                {
                    // Search samples strings

                    if(     next_search_data.size()                ==  0
                        ||  (data_idx + next_search_data.size() )   >   data_size)
                        continue;

                    if(     current_sample.max_search_positions     >   0
                        &&  current_sample.search_direction         ==  front
                        &&  (count_pos + next_search_data.size())   >   current_sample.max_search_positions)
                        continue;

                    if(     current_sample.max_search_positions     >   0
                        &&  current_sample.search_direction         ==  back
                        &&  (data_idx + next_search_data.size())    >   (search_pos_idx + 1))
                    {
                        continue;
                    }

                    if( 0 != memcmp(next_search_data.data(), &p_data[data_idx], next_search_data.size()))
                        continue;

                    if( current_sample.max_found_samples > 0)
                        if( total_found_samples >= current_sample.max_found_samples)
                            break;

                    total_found_samples += 1;

                    // Found data sample, process it

                    vector<CxFoundSamplePos> next_found_sample = current_found_sample;

                    if( next_found_sample.size() > 0)
                    {
                        // Fill interim data values in prev result

                        auto& last_sample = next_found_sample.back();

                        uint64_t last_data_end_idx = 0;

                        switch( current_sample.search_direction)
                        {
                            case front:

                                last_data_end_idx = last_sample.begin_idx + last_sample.len;

                                if( last_data_end_idx > data_idx)
                                {
                                    m_last_error = "internal error: ( last_data_end_idx > data_idx)";
                                    return false;
                                }

                                last_sample.interim_data_begin_idx =    last_data_end_idx;
                                last_sample.interim_data_len =          data_idx - last_data_end_idx;
                                
                            break;

                            case back:
                                auto data_end_idx = data_idx + next_search_data.size();

                                if( data_end_idx < last_sample.begin_idx)
                                {
                                    last_sample.interim_data_begin_idx =    data_end_idx;
                                    last_sample.interim_data_len =          last_sample.begin_idx - data_end_idx;
                                }
                                else
                                {
                                    last_sample.interim_data_begin_idx =    data_end_idx;
                                    last_sample.interim_data_len =          0;
                                }

                            break;
                        }
                    }

                    CxFoundSamplePos current_sample_pos;

                    current_sample_pos.begin_idx =              data_idx;
                    current_sample_pos.len =                    next_search_data.size();
                    current_sample_pos.interim_data_begin_idx = 0;
                    current_sample_pos.interim_data_len =       0;

                    next_found_sample.push_back(current_sample_pos);

                    if( (current_sample_idx + 1) >= search_samples.size())
                    {
                        // Find last data sample, save search results 

                        search_samples_pos.push_back( move(next_found_sample));

                        if(     max_found_samples           >   0
                            &&  search_samples_pos.size()   >=  max_found_samples)
                            return true;

                        if(     current_sample.max_found_samples    >   0
                            &&  total_found_samples                 >=  current_sample.max_found_samples)
                                break;
                    }
                    else
                    {
                        // Try to find next samples

                        if( false == FindInternal(  search_samples,
                                                    current_sample_idx + 1,
                                                    p_data,
                                                    data_size,
                                                    max_found_samples,
                                                    next_found_sample,
                                                    search_samples_pos))
                            return false;

                        if(     max_found_samples           >   0
                            &&  search_samples_pos.size()   >= max_found_samples)
                            return true;

                        if(     current_sample.max_found_samples    >   0
                            &&  total_found_samples                 >=  current_sample.max_found_samples)
                                break;
                    }
                }
            }

            if( is_stop_found == true)
                return true;

            if(     current_sample.is_to_end_if_not_found   == true    
                &&  total_found_samples                     == 0)
            {
                vector<CxFoundSamplePos> next_found_sample = current_found_sample;

                if( next_found_sample.size() > 0)
                {
                    // Fill interim data values

                    auto& last_sample = next_found_sample.back();

                    uint64_t last_data_end_idx = 0;

                    switch( current_sample.search_direction)
                    {
                        case front:

                            last_data_end_idx = last_sample.begin_idx + last_sample.len;

                            if( last_data_end_idx > data_size )
                            {
                                m_last_error = "internal error: ( last_data_end_idx > data_idx)-2";
                                return false;
                            }

                            last_sample.interim_data_begin_idx =    last_data_end_idx;
                            last_sample.interim_data_len =          data_size - last_data_end_idx;
                                
                        break;

                        case back:
                            last_sample.interim_data_begin_idx =    0;
                            last_sample.interim_data_len =          last_sample.begin_idx;
                        break;

                        default:
                            m_last_error = "internal error: unknown search direction-3, sample_idx: " + to_string(current_sample_idx);
                            return false;
                        break;
                    }
                }

                CxFoundSamplePos current_sample_pos;

                switch( current_sample.search_direction)
                {
                    case front:
                        current_sample_pos.begin_idx =              data_size;
                        current_sample_pos.len =                    0;
                    break;

                    case back:
                        current_sample_pos.begin_idx =              0;
                        current_sample_pos.len =                    0;
                    break;

                    default:
                        m_last_error = "internal error: unknown search direction-4, sample_idx: " + to_string(current_sample_idx);
                        return false;
                    break;
                }

                current_sample_pos.interim_data_begin_idx = 0;
                current_sample_pos.interim_data_len =       0;

                next_found_sample.push_back(current_sample_pos);

                if( (current_sample_idx + 1) >= search_samples.size())
                {
                    // Find last data sample, save search results 

                    search_samples_pos.push_back( move(next_found_sample));
                }
                else
                {
                    // Try to find next samples

                    if( false == FindInternal(  search_samples,
                                                current_sample_idx + 1,
                                                p_data,
                                                data_size,
                                                max_found_samples,
                                                next_found_sample,
                                                search_samples_pos))
                        return false;
                }
            }

            return true;
        }
        catch(...)
        {
            try
            {
                m_last_error = "internal error: unknown exception";
            }
            catch(...)
            {}
        }

        return false;
    }


    bool
    CompEx::
    GetInternalString(  const   std::vector<CxFoundSamplePos>&  sample_pos,
                                uint32_t                        pos_idx,
                        const   char*                           p_data,
                                uint64_t                        data_size,
                                std::string&                    internal_string)
    {
        internal_string.clear();
        m_last_error.clear();

        try
        {
            if( p_data == nullptr)
            {
                m_last_error = "incorrect method param: p_data == nullptr";
                return false;
            }

            if( sample_pos.size() == 0)
            {
                m_last_error = "incorrect method param: sample_pos.size() == 0";
                return false;
            }

            if( pos_idx >= sample_pos.size())
            {
                m_last_error = "incorrect method param: pos_idx >= sample_pos.size() ( " 
                                + to_string(pos_idx ) + " >= " + to_string(sample_pos.size()) + " )";
                return false;
            }

            auto& sample_pos_info = sample_pos[pos_idx];

            if( sample_pos_info.interim_data_len == 0)
                return true;

            if( sample_pos_info.interim_data_begin_idx >= data_size)
            {
                m_last_error = "incorrect data: interim_data_begin_idx >= data_size"
                                ", interim_data_begin_idx: " + to_string(sample_pos_info.interim_data_begin_idx)
                                + ", interim_data_len: " + to_string(sample_pos_info.interim_data_len)
                                + ", data_size: " + to_string(data_size);
                return false;
            }

            if( (sample_pos_info.interim_data_begin_idx + sample_pos_info.interim_data_len) > data_size)
            {
                m_last_error = "incorrect data: (interim_data_begin_idx + interim_data_len) > data_size"
                                ", interim_data_begin_idx: " + to_string(sample_pos_info.interim_data_begin_idx)
                                + ", interim_data_len: " + to_string(sample_pos_info.interim_data_len)
                                + ", data_size: " + to_string(data_size);
                return false;
            }

            if( sample_pos_info.interim_data_len > MAX_INTERIM_DATA_SIZE)
            {
                m_last_error = "incorrect data: interim_data_len > "+ to_string(MAX_INTERIM_DATA_SIZE);

                return false;
            }

            internal_string.assign( &p_data[sample_pos_info.interim_data_begin_idx], sample_pos_info.interim_data_len);

            return true;
        }
        catch(...)
        {
            try
            {
                m_last_error = "unknown exception";
            }
            catch(...)
            {}
        }

        internal_string.clear();

        return false;
    }
}

}
