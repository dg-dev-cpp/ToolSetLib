#ifndef TOOL_SET_LIB_FILES_BASE_FUNCTIONS_HEADER
#define TOOL_SET_LIB_FILES_BASE_FUNCTIONS_HEADER

#include <string>
#include <vector>

namespace ToolSetLib
{

namespace Files
{

bool
ReadFile( const std::string& file_path, std::vector<char>& file_data);

bool
WriteFile( const std::string& file_path, const std::string& file_data);

}

}
#endif