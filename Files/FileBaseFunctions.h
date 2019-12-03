#ifndef TOOL_SET_LIB_FILES_BASE_FUNCTIONS_HEADER
#define TOOL_SET_LIB_FILES_BASE_FUNCTIONS_HEADER

#include <string>
#include <vector>

namespace ToolSetLib
{

namespace Files
{

bool
ReadFile( const std::string&        a_file_path, 
                std::vector<char>&  a_file_data,
                uint64_t            a_max_file_size = UINT64_MAX);

bool
WriteFile( const std::string& a_file_path, const std::string& a_file_data);

std::string
GetLastError();

}

}
#endif