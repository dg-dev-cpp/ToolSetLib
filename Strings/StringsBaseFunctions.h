#ifndef TOOL_SET_LIB_STRINGS_BASE_HEADER
#define TOOL_SET_LIB_STRINGS_BASE_HEADER

#include "defines/StringsBaseFunctionsDefines.h"

#include <string>
#include <vector>


namespace ToolSetLib
{

namespace Strings
{

std::string
IntToStrDig( int64_t num, uint32_t minStrLen = 0);

std::wstring
IntToStrDigW( int64_t num, uint32_t minStrLen = 0);

bool
ReduceStringLen( std::string& reduce_str, uint32_t max_string_size);

std::vector<std::string>
Split( const std::string& source_string, const std::string& delimiter);

bool
Trim( std::string& trim_str);

}

}
#endif