#ifndef TOOL_SET_LIB_STRINGS_COMPEX_DEFINES
#define TOOL_SET_LIB_STRINGS_COMPEX_DEFINES

#include <vector>
#include <string>


namespace ToolSetLib
{

namespace Strings
{

    #define MAX_INTERIM_DATA_SIZE       ((uint64_t)16*1024*1024)
    #define MAX_COMPEX_FOUND_DATA_SIZE  ((uint64_t)12*1024*1024*1024)
    #define MAX_COMPEX_SAMPLES          64
    #define MAX_FORMAT_STR_LEN          1024

    enum CxSearchDirection
    {
	    front,
	    back
    };

    class CxSearchSample
    {
    public:

	    CxSearchDirection		    search_direction;
	    uint64_t			        max_search_positions;
	    std::vector<std::string>	sample_strings;
	    uint32_t			        max_found_samples;
	    std::vector<std::string>	stop_strings;
	    bool				        is_to_end_if_not_found;
    };

    class CxSearchSamplePos
    {
    public:

	    uint64_t begin_idx;
	    uint64_t len;

	    uint64_t interim_data_begin_idx;
	    uint64_t interim_data_len;

	    CxSearchSamplePos()
	    {
		    begin_idx =		0;
		    len =			0;

            interim_data_begin_idx =    0;
            interim_data_len =          0;
	    }
    };


    class CxFoundSamplePos
    {
    public:

	    uint64_t begin_idx;
	    uint64_t len;

	    uint64_t interim_data_begin_idx;
	    uint64_t interim_data_len;

	    CxFoundSamplePos()
	    {
		    begin_idx =		0;
		    len =			0;

            interim_data_begin_idx =    0;
            interim_data_len =          0;
	    }
    };

}

}

#endif