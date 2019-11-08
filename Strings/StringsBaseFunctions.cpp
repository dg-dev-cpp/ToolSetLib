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
	    string		        digStr;
	    uint32_t	        pos3Count = 1;

	    if(numStr.length() != 0)
	    for (int32_t	count	=	numStr.length() - 1;
				        count	>=	0;
				        count   -=	1, pos3Count +=1)
	    {
		    digStr.insert(digStr.begin(), numStr[count]);

		    if( pos3Count >= 3 && count > 0)
		    {
			    digStr.insert(digStr.begin(), ' ');
			    pos3Count = 0;
		    }
	    }

	    if(		minStrLen	    ==  0
		    ||	digStr.length() >=  minStrLen
		    ||	minStrLen	    >   MAX_FORMAT_STR_LEN)
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
	    wstring		        digStr;
	    uint32_t	        pos3Count = 1;

	    if(numStr.length() != 0)
	    for(int32_t count	=	numStr.length() - 1;
				    count	>=	0;
				    count	-=	1, pos3Count +=1)
	    {
		    digStr.insert(digStr.begin(), numStr[count]);

		    if( pos3Count >= 3 && count > 0)
		    {
			    digStr.insert(digStr.begin(),' ');
			    pos3Count = 0;
		    }
	    }

	    if(		minStrLen	    ==  0
		    ||	digStr.length() >=  minStrLen
		    ||	minStrLen	    >   MAX_FORMAT_STR_LEN)
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

		string	rest_size_str = to_string( ( reduce_str.size() - max_string_size) + digits_count + end_string.size()) + end_string;

		if(		rest_size_str.size() < max_string_size
			&&	(max_string_size - rest_size_str.size()) > rest_size_str.size())
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
                for(	uint32_t	count	=	0;
                					count	<	end_string.size();
                					count	+=	1)
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
Split( const string& source_string, const std::string& delimiter)
{
	vector<string> split_result;

	if( source_string.size() == 0)
		return split_result;

	if( source_string.size() <= delimiter.size())
	{
		return split_result;
	}

	if( delimiter.size() == 0)
	{
		return split_result;
	}

	if(		source_string.size()	>=	INT32_MAX
		||	delimiter.size()		>=	INT32_MAX)
		return split_result;

	try
	{
		int source_size =		(int)source_string.size();
		int delimiter_size =	(int)delimiter.size();

		const char* p_source_str =		source_string.c_str();
		const char* p_delimiter_str =	delimiter.c_str();

		vector<int> delimeter_idxs;

		for(int	count	=	0;
				count	<	source_size;
				count	+=	1)
		{
			bool is_equal = true;

			for(uint32_t	count_delim	=	0;
							count_delim	<	delimiter_size;
							count_delim	+=	1)
			{
				if(p_source_str[count + count_delim] != p_delimiter_str[count_delim])
				{
					is_equal = false;
					break;
				}
			}

			if( is_equal == false)
				continue;

			delimeter_idxs.push_back(count);

			count += (delimiter_size - 1);
		}

		if( delimeter_idxs.size() == 0)
		{
			return split_result;
		}

		string split_string;

		split_string.assign(&p_source_str[0],delimeter_idxs[0]);

		split_result.push_back(split_string);

		for(int	count	=	0;
				count	<	delimeter_idxs.size();
				count	+=	1)
		{
			auto current_delim_idx = delimeter_idxs[count];

			if((count + 1) >= delimeter_idxs.size())
			{
				if((current_delim_idx + delimiter_size) <= source_size)
				{
					split_string.clear();

					if( (source_size - delimiter_size - current_delim_idx) > 0)
						split_string.assign(&p_source_str[current_delim_idx + delimiter_size],source_size - delimiter_size - current_delim_idx);

					split_result.push_back(split_string);
				}

				break;
			}

			auto next_delim_idx = delimeter_idxs[count + 1];

			split_string.clear();

			if( ((next_delim_idx)- (current_delim_idx + delimiter_size)) > 0)
				split_string.assign(&p_source_str[current_delim_idx + delimiter_size],(next_delim_idx)- (current_delim_idx + delimiter_size));

			split_result.push_back(split_string);
		}

        return move(split_result);
	}
	catch(...)
	{}

    split_result.clear();
    return move(split_result);
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

	    if(		trimePos != 0
		    &&	trimePos != string::npos)
		    trim_str.erase(0,trimePos);    

        return true;
    }
    catch(...)
    {}

    return false;
}

}

}
