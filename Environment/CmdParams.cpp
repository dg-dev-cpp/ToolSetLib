#include "CmdParams.h"

using namespace std;


namespace ToolSetLib
{

namespace Environment
{

bool
CmdParams::
GetCmdParams(   uint32_t					total_params,
        const   char*						p_params_strings[],
                vector<CmdFindKeyParam>&	find_params,
                string*						p_error_str)
{
    cmd_keys_found.clear();

    try
    {
	    if( p_error_str != 0)
		    *p_error_str = "";

	    if(	p_params_strings	==	0
	    ||	total_params		<	0)
	    {
		    if(p_error_str != 0)
			    *p_error_str = "incorrect method arguments values: p_params_strings or (and) total_params";

		    return false;
	    }

	    if( find_params.size() == 0)
		    return true;

	    string next_param;

	    for(uint32_t	count	=	0;
					    count	<	total_params;
					    count	+=	1)
	    {
		    const char* p_next_key = p_params_strings[count];

		    if( p_next_key == 0)
		    {
			    if(p_error_str != 0)
				    *p_error_str =		string("internal error: cmd string incorrect, ")
								    +	"string idx: " + to_string(count);
			    cmd_keys_found.clear();
			    return false;
		    }

		    for(
			    int	count_find	=	0;
				    count_find	<	find_params.size();
				    count_find	+=	1)
		    {
			    auto& next_find_key = find_params[count_find];

			    if( next_find_key.key.compare( p_next_key ) != 0)
				    continue;
		
			    if((count + 1 + next_find_key.values_types.size()) > total_params)
			    {
				    if(p_error_str != 0)
					    *p_error_str =		"too low cmd params for key '" + next_find_key.key + "'";

				    cmd_keys_found.clear();
				    return false;
			    }

			    bool is_key_params = true;

			    CmdKey cmd_key;

			    for(
				    int	count_key_param	=	0;
					    count_key_param	<	next_find_key.values_types.size();
					    count_key_param	+=	1)
			    {
				    const char* p_next_key_value = p_params_strings[(count_key_param  + count + 1)];

				    if( p_next_key_value == 0)
				    {
					    if(p_error_str != 0)
						    *p_error_str =		"internal error for key '" + next_find_key.key + "'"
										    +	", key value idx: " + to_string(count_key_param)
										    +	": incorrect cmd param string";
					    cmd_keys_found.clear();
					    return false;
				    }

				    switch( next_find_key.values_types[count_key_param])
				    {
					    case CmdKeyValueType::string:
					    {
						    CmdKeyValue key_value;
						    key_value.type =		CmdKeyValueType::string;
						    key_value.value_str =	p_next_key_value;
						
						    cmd_key.values.push_back( key_value);
					    };break;

					    case CmdKeyValueType::int32:
					    {
						    try
						    {
							    stol(p_next_key_value);
						    }
						    catch (...)
						    {
							    if( p_error_str != 0)
								    *p_error_str =		"convert string key value to int32 fail: key '" + next_find_key.key + "'"
												    +	", value idx: " + to_string(count_key_param)
												    +	", cmd param string: " + p_next_key_value;
							    cmd_keys_found.clear();
							    return false;
						    }

						    CmdKeyValue key_value;
						    key_value.type =		CmdKeyValueType::int32;
						    key_value.value_str =	p_next_key_value;
						    cmd_key.values.push_back(key_value);
					    };break;

					    default:
					    {
						    if(p_error_str != 0)
							    *p_error_str =		"internal error for key '" + next_find_key.key + "'"
											    +	", value idx: " + to_string(count_key_param)
											    +	": unknown param type";
						    cmd_keys_found.clear();
						    return false;
					    };break;
				    }
			    }

			    if( is_key_params == true)
			    {
				    cmd_key.key_type =	next_find_key.key_type;
				    cmd_key.key =		next_find_key.key;

				    cmd_keys_found.push_back(cmd_key);
				    count += next_find_key.values_types.size();
			    }
		    }
	    }


	    for(
		    int	count_find	=	0;
			    count_find	<	find_params.size();
			    count_find	+=	1)
	    {
		    auto& next_find_key = find_params[count_find];

		    if(		next_find_key.key_type == CmdKeyType::required
			    ||	next_find_key.key_type == CmdKeyType::unique_required)
		    {
			    if( false == IsKeyPresent(next_find_key.key))
			    {
				    if(p_error_str != 0)
					    *p_error_str =	"key '" + next_find_key.key + "'" +	" not setted";

				    cmd_keys_found.clear();
				    return false;
			    }
		    }

		    if(		next_find_key.key_type == CmdKeyType::unique
			    ||	next_find_key.key_type == CmdKeyType::unique_required)
		    {
			    //cout<<"next_find_key.key: "<<next_find_key.key<<", key count: "<< GetKeyCount(next_find_key.key)<<endl;

			    if( 2 <= GetKeyCount(next_find_key.key))
			    {
				    if(p_error_str != 0)
					    *p_error_str =	"key '" + next_find_key.key + "'" +	" repeated";

				    cmd_keys_found.clear();
				    return false;
			    }
		    }
	    }

	    return true;
    }
    catch(...)
    {}

    cmd_keys_found.clear();
    return false;
}



bool
CmdParams::
IsKeyPresent(const string& key_str)
{
	for(auto& next_saved_key: cmd_keys_found)
	{
		if( key_str.compare(next_saved_key.key) == 0)
			return true;
	}

	return false;
}


uint32_t
CmdParams::
GetKeyCount(const string& key_str)
{
	uint32_t keys_total = 0;

	for(auto& next_saved_key: cmd_keys_found)
	{
		if(key_str.compare(next_saved_key.key) == 0)
			keys_total += 1;
	}

	return keys_total;
}



pair<string,vector<string>>
CmdParams::
GetKeyValues(const string& key_str, uint32_t key_data_idx)
{
    try
    {
        uint32_t    key_found_idx = 0;

        for(	uint32_t	count	=	0;
        					count	<	cmd_keys_found.size();
        					count	+=	1)
	    {
            auto& next_saved_key = cmd_keys_found[count];

		    if( key_str.compare(next_saved_key.key) == 0)
		    {
                if( key_data_idx == key_found_idx)
                {
                    pair<string,vector<string>> key_values;

                    for(auto& next_value: next_saved_key.values)
                    {
                        key_values.second.push_back(next_value.value_str);
                    }

                    key_values.first = key_str;

                    return move(key_values);
                }

                key_found_idx += 1;
		    }
	    }

        return pair<string,vector<string>>();
    }
    catch(...)
    {}

	return pair<string,vector<string>>();
}


std::string
CmdParams::
GetFrontKeyValue(const std::string& a_key_str, uint32_t a_value_idx)
{
    for(auto& next_saved_key: cmd_keys_found)
    {
        if( a_key_str.compare(next_saved_key.key) == 0)
        {
            if( next_saved_key.values.size() > a_value_idx)
                return next_saved_key.values[a_value_idx].value_str;
            else
                return "";
        }
    }

    return "";
}


unsigned long long
CmdParams::
GetFrontKeyValueAsUllong(const  std::string&    a_key_str, 
                                uint32_t        a_value_idx,
                                uint32_t        a_return_default)
{
    try
    {
        for(auto& next_saved_key: cmd_keys_found)
        {
            if(a_key_str.compare(next_saved_key.key) == 0)
            {
                if( next_saved_key.values.size() > a_value_idx)
                    return stoull(next_saved_key.values[a_value_idx].value_str);
                else
                {
                    return a_return_default;
                }
            }
        }
    }
    catch(...)
    {}

    return a_return_default;
}


std::vector<std::string>
CmdParams::
GetCmdParamsDescription(const vector<CmdFindKeyParam>& a_cmd_params_spec)
{
    try
    {
        std::vector<std::string> description_lines;

        uint32_t max_param_len = 0;

        for(auto& next_param: a_cmd_params_spec)
        {
            if( next_param.key.size() > max_param_len)
                max_param_len = next_param.key.size();
        }

        string next_line;

        for(auto& next_param: a_cmd_params_spec)
        {
            next_line.clear();
            next_line.append(next_param.key);

            if(next_line.size() < max_param_len)
                next_line.resize(max_param_len);

            uint32_t values_count = 1;
            for(auto& next_type: next_param.values_types)
            {
                next_line.append(" value-");
                next_line.append(to_string(values_count));
                next_line.append("");

                values_count++;
            }

            if( next_param.description.size() > 0 )
                next_line.append(" : ");
                next_line.append(next_param.description);

            description_lines.push_back(move(next_line));
        }

        return move(description_lines);
    }
    catch(...)
    {}

    return std::vector<std::string>();
}

}

}