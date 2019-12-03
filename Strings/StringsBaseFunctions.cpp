#include "StringsBaseFunctions.h"

using namespace std;


namespace ToolSetLib
{

namespace Strings
{

string
IntToStrDig( int64_t num, uint32_t minStrLen)
{
    try
    {
        string numStr = to_string(num);

        std::vector<int>    insertPos;
        string              digStr;
        uint32_t            pos3Count = 1;

        if(numStr.length() != 0)
        for (int32_t    count   =   numStr.length() - 1;
                        count   >=  0;
                        count   -=  1, pos3Count +=1)
        {
            digStr.insert(digStr.begin(), numStr[count]);

            if( pos3Count >= 3 && count > 0)
            {
                digStr.insert(digStr.begin(), ' ');
                pos3Count = 0;
            }
        }

        if(     minStrLen       ==  0
            ||  digStr.length() >=  minStrLen
            ||  minStrLen       >   MAX_FORMAT_STR_LEN)
            return move(digStr);

        string insertStr;

        insertStr.resize(minStrLen - digStr.length(),' ');
        insertStr += digStr;

        return move(insertStr);
    }
    catch(...)
    {}

    return "";
}


wstring
IntToStrDigW( int64_t num, uint32_t minStrLen)
{
    try
    {
        wstring numStr = to_wstring(num);

        std::vector<int>    insertPos;
        wstring             digStr;
        uint32_t            pos3Count = 1;

        if(numStr.length() != 0)
        for(int32_t count   =  numStr.length() - 1;
                    count   >= 0;
                    count   -= 1, pos3Count +=1)
        {
            digStr.insert(digStr.begin(), numStr[count]);

            if( pos3Count >= 3 && count > 0)
            {
                digStr.insert(digStr.begin(),' ');
                pos3Count = 0;
            }
        }

        if(     minStrLen       ==  0
            ||  digStr.length() >=  minStrLen
            ||  minStrLen       >   MAX_FORMAT_STR_LEN)
            return move(digStr);

        wstring insertStr;

        insertStr.resize(minStrLen - digStr.length(),' ');
        insertStr += digStr;

        return move(insertStr);
    }
    catch(...)
    {}

    return L"";
}


bool
ReduceStringLen( string& reduce_str, uint32_t max_string_size)
{
    if( max_string_size == 0)
    {
        reduce_str.clear();
        return true;
    }

    if( reduce_str.size() <= max_string_size)
        return true;

    try
    {
        //const static string end_string = "...";
        const static string end_string = "~";

        uint64_t digits_count = 0;

        uint64_t initial_value = ( (uint64_t)reduce_str.size() - (uint64_t)max_string_size );

        for(;;)
        {
            digits_count += 1;
            initial_value = initial_value/ 10;

            if( initial_value == 0)
                break;
        }

        string rest_size_str = to_string( ( reduce_str.size() - max_string_size) + digits_count + end_string.size()) + end_string;

        if(     rest_size_str.size() < max_string_size
            &&  (max_string_size - rest_size_str.size()) > rest_size_str.size())
        {
            auto initial_string_size = reduce_str.size();

            reduce_str.resize( max_string_size - rest_size_str.size());
            rest_size_str = to_string(initial_string_size - reduce_str.size());

            reduce_str += end_string + rest_size_str;

            if( reduce_str.size() > max_string_size)
                reduce_str.resize( reduce_str.size() - rest_size_str.size());
        }
        else
        {
            reduce_str.resize(max_string_size);

            if( reduce_str.size() > end_string.size())
            {
                for(    uint32_t    count   =   0;
                                    count   <   end_string.size();
                                    count   +=  1)
                {
                    reduce_str[ (reduce_str.size() - end_string.size()) + count] = end_string[count];
                }
            }
                
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}


vector<string>
Split( const string& a_source_string, const std::string& a_delimiter)
{
    vector<string> split_result;

    try
    {
        function<bool (const char*, uint64_t)> process_records = 
                [&] (const char* p_part, uint64_t part_size) -> bool 
            {
                string next_string( p_part, part_size);

                split_result.push_back( move(next_string));

                return true;
            };

        if( false == ToolSetLib::Strings::SplitForEach( a_source_string.data(), 
                                                        a_source_string.size(), 
                                                        a_delimiter, 
                                                        process_records))
        {
            split_result.clear();
            return move(split_result);
        }

        return move(split_result);
    }
    catch(...)
    {}

    split_result.clear();
    return move(split_result);
}


bool
SplitForEach(   const   char*           ap_data,
                        uint64_t        a_data_size,
                const   std::string&    a_delimiter,
                        std::function<bool (const char* p_part_begin, uint64_t part_len)> a_callback_function)
{
    if(     ap_data             == nullptr
        ||  a_data_size         == 0
        ||  a_callback_function == nullptr)
        return false;

    if( a_data_size > ((uint64_t)32*1024*1024*1024))
        return false;

    if( a_data_size <= a_delimiter.size())
    {
        return false;
    }

    if( a_delimiter.size() == 0)
    {
        return false;
    }

    if( a_delimiter.size() >= (16*1024))
    {
        return false;
    }

    try
    {
        auto    delimiter_size =    a_delimiter.size();
        auto*   p_delimiter_str =   a_delimiter.data();

        uint64_t last_end_delimiter_idx = 0;

        for( uint64_t   count   =   0;
                        count   <   a_data_size;
                        count   +=  1)
        {
            bool is_equal = true;

            for(uint32_t    count_delim =   0;
                            count_delim <   delimiter_size;
                            count_delim +=  1)
            {
                if( ap_data[count + count_delim] != p_delimiter_str[count_delim])
                {
                    is_equal = false;
                    break;
                }
            }

            if( is_equal == false)
                continue;

            if( count >= last_end_delimiter_idx)
            {
                if( false == a_callback_function(&ap_data[last_end_delimiter_idx], (count - last_end_delimiter_idx)))
                    return false;
            }

            last_end_delimiter_idx = count + delimiter_size;

            count += (delimiter_size - 1);
        }

        if(     last_end_delimiter_idx  !=  0
            &&  last_end_delimiter_idx  <=  a_data_size)
        {
            if(false == a_callback_function(&ap_data[last_end_delimiter_idx],(a_data_size - last_end_delimiter_idx)))
                return false;
        }

        return true;
    }
    catch(...)
    {}

    return false;
}


bool
Trim( string& trim_str)
{
    try
    {
        const char* spaces = " \r\n";

        auto trimePos = trim_str.find_last_not_of(spaces);

        if( trimePos != string::npos)
        {
            uint32_t erase_total = trim_str.size() - (trimePos + 1);

            if(erase_total != 0)
                trim_str.erase((trimePos + 1),erase_total);
        }

        trimePos = trim_str.find_first_not_of(spaces);

        if(     trimePos != 0
            &&  trimePos != string::npos)
            trim_str.erase(0,trimePos);    

        return true;
    }
    catch(...)
    {}

    return false;
}

}

}
