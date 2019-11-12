#ifndef TOOL_SET_LIB_STRINGS_COMPEX_HEADER
#define TOOL_SET_LIB_STRINGS_COMPEX_HEADER

#include "defines/CompExDefines.h"

// CompEx class is to search for composite string expressions

namespace ToolSetLib
{

namespace Strings
{

class CompEx
{
public:

    class VersionInfo
    {
    public:
        static const uint32_t api =         0;
        static const uint32_t features =    1;
        static const uint32_t revision =    4;
    };

    std::string m_last_error;

    bool
    Find(   const   std::vector<CxSearchSample>&                search_samples,
            const   char*                                       p_data,
                    uint64_t                                    data_size,
                    uint32_t                                    max_found_samples,
                    std::vector<std::vector<CxFoundSamplePos>>& found_samples_pos);

    bool
    GetInternalString(  const   std::vector<CxFoundSamplePos>&  sample_pos,
                                uint32_t                        pos_idx,
                        const   char*                           p_data,
                                uint64_t                        data_size,
                                std::string&                    internal_string);
private:

    bool
    FindInternal(   const    std::vector<CxSearchSample>&               search_samples,
                            uint64_t                                    current_sample_idx,
                    const   char*                                       p_data,
                            uint64_t                                    data_size,
                            uint32_t                                    max_found_samples,
                            std::vector<CxFoundSamplePos>&              current_found_sample,
                            std::vector<std::vector<CxFoundSamplePos>>& search_samples_pos);
};

}

}

#endif