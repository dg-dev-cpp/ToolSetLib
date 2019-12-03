#include "FileBaseFunctions.h"

#include <fstream>


namespace ToolSetLib
{

namespace Files
{

using namespace std;

thread_local const char* np_last_error = "";


bool
ReadFile( const std::string&        a_file_path, 
                std::vector<char>&  a_file_data,
                uint64_t            a_max_file_size)
{
    static_assert( sizeof(char) == 1, "unsupported char size");

    np_last_error = "";

    a_file_data.clear();

    try
    {
        ifstream source_file;
        
        source_file.open(a_file_path, ios::binary);

        if( source_file.is_open() == false)
        {
            np_last_error = "fail to open file";
            return false;
        }

        source_file.seekg (0, source_file.end);
        auto source_file_size = source_file.tellg();
        
        if( source_file_size == ifstream::pos_type(- 1))
        {
            np_last_error = "fail to get file size";
            return false;
        }

        if((uint64_t)source_file_size  > a_max_file_size)
        {
            np_last_error = "source_file_size > max_file_size";
            return false;
        }

        a_file_data.resize( source_file_size);

        if( a_file_data.size() != source_file_size)
        {
            np_last_error = "fail to allocate file data buffer";
            a_file_data.clear();
            return false;
        }

        source_file.seekg (0,source_file.beg);

        source_file.read( a_file_data.data(), a_file_data.size());

        if( source_file.fail() == true)
        {
            np_last_error = "fail to read file data";
            a_file_data.clear();
            return false;
        }

        return true;
    }
    catch(...)
    {}

    np_last_error = "general exception";

    a_file_data.clear();

    return false;
}


bool
WriteFile( const string& a_file_path, const string& a_file_data)
{
    try
    {
        np_last_error = "";

        ofstream source_file;

        source_file.open( a_file_path,ios::binary | ios::trunc);

        if( source_file.is_open() == false)
        {
            np_last_error = "fail to open file";
            return false;
        }

        source_file.write( a_file_data.data(), a_file_data.size());

        if( source_file.fail() == true)
        {
            np_last_error = "fail to write file";
            source_file.close();
            return false;
        }

        return true;
    }
    catch(...)
    {}

    np_last_error = "general exception";

    return false;
}


std::string
GetLastError()
{
    try
    {
        if( np_last_error == nullptr)
            return "";

        return np_last_error;
    }
    catch(...)
    {}

    return "";
}

}

}
