#include "FileBaseFunctions.h"

#include <fstream>
#include <filesystem>

namespace ToolSetLib
{

namespace Files
{

using namespace std;


bool
ReadFile(const string& file_path, vector<char>& file_data)
{
    file_data.clear();

    try
    {
        auto source_file_size = std::experimental::filesystem::file_size(file_path);

        if( source_file_size == -1)
            return false;

        ifstream source_file;

        source_file.open(file_path, ios::binary);

        if( source_file.is_open() == false)
            return false;

        file_data.resize( source_file_size);

        if( file_data.size() != source_file_size)
            return false;

        source_file.read( file_data.data(), file_data.size());

        if( source_file.fail() == true)
            return false;

        return true;
    }
    catch(...)
    {}

    file_data.clear();

    return false;
}


bool
WriteFile( const string& file_path, const string& file_data)
{
    try
    {
        ofstream source_file;

        source_file.open( file_path,ios::binary | ios::trunc);

        if( source_file.is_open() == false)
            return false;

        source_file.write( file_data.data(), file_data.size());

        if( source_file.fail() == true)
        {
            source_file.close();
            std::experimental::filesystem::remove(file_path);
            return false;
        }

        return true;
    }
    catch(...)
    {}

    return false;
}

}

}
