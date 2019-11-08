#include "HexString.h"


namespace ToolSetLib
{

namespace Encoding
{

using namespace std;


string
HexString::
Encode(const string& source_str, bool is_upper_case)
{
    m_last_error.clear();

    string encoded_string;

    Encode( source_str.data(), 
            source_str.size(), 
            encoded_string, 
            is_upper_case);

    return move(encoded_string);
}


bool
HexString::
Encode(const    char*           p_data,
                uint64_t        data_size,
                std::string&    encoded_string,
                bool            is_upper_case)
{
    m_last_error.clear();
    encoded_string.clear();

    {
        // Checking input values

        if( data_size == 0)
        {
            return true;
        }

        if( p_data == nullptr)
        {
            try
            {
                m_last_error.append("HexString::Encode(): incorrect method param: (p_data == nullptr)");
            }
            catch(...)
            {}

		    return false;
        }

        if( data_size > MAX_HEX_STRING_DATA_SIZE)
        {
            try
            {
                m_last_error.append("HexString::Encode(): data size is too large, max size: ");
                m_last_error.append(to_string(MAX_HEX_STRING_DATA_SIZE));
            }
            catch(...)
            {}
            
		    return false;
        }

        const std::pair<unsigned char, unsigned char>* p_data_array = nullptr;

        if( is_upper_case == true)
        {
            if( g_data_to_hex_uc.size() < 256)
            {
                try
                {
                    m_last_error.append("HexString::Encode(): internal error: ( g_data_to_hex_uc.size() < 256)");
                }
                catch(...)
                {}

                return false;
            }

            p_data_array = g_data_to_hex_uc.data();
        }
        else
        {
            p_data_array = g_data_to_hex_lc.data();

            if( g_data_to_hex_lc.size() < 256)
            {
                try
                {
                    m_last_error.append("HexString::Encode(): internal error: ( g_data_to_hex_lc.size() < 256)");
                }
                catch(...)
                {}

                return false;
            }
        }

        if( p_data_array == nullptr)
        {
            try
            {
                m_last_error.append("HexString::Encode(): internal error: ( p_data_array == nullptr)");
            }
            catch(...)
            {}

            return false;
        }

        for(	uint64_t	count	=	0;
        					count	<	data_size;
        					count	+=	1)
        {
            auto& data_elem = p_data_array[((unsigned char)p_data[count])];

            try
            {
                encoded_string.append( 1, data_elem.first);
                encoded_string.append( 1, data_elem.second);
            }
            catch(...)
            {
                try
                {
                    m_last_error.append("HexString::Encode(): internal error: fail to append encoded string");
                }
                catch(...)
                {}
                
                encoded_string.clear();

                return false;
            }
        }
    }

    return true;
}


bool
HexString::
Decode(	const	std::string&	    encoded_string,
				std::vector<char>&	decoded_data)
{
    m_last_error.clear();
	decoded_data.clear();

	if( encoded_string.size() == 0)
		return true;

    if( encoded_string.size() > (MAX_HEX_STRING_DATA_SIZE*2))
    {
        try
        {
            m_last_error.append("HexString::Decode(): encoded string size is too large, max: ");
            m_last_error.append(to_string(MAX_HEX_STRING_DATA_SIZE*2));
        }
        catch(...)
        {}

        return false;
    }

    if( encoded_string.size()%2 != 0)
    {
        try
        {
            m_last_error.append("HexString::Decode(): encoded string length is odd");
        }
        catch(...)
        {}

        return false;
    }

    try
    {
        decoded_data.resize( encoded_string.size()/2);
    }
    catch(...)
    {}

    auto* p_decoded_data = decoded_data.data();

    if(     decoded_data.size() != encoded_string.size()/2
        ||  p_decoded_data      == nullptr)
    {
        try
        {
            m_last_error.append("HexString::Decode(): fail to reserve decoded data memory");
        }
        catch(...)
        {}

        decoded_data.clear();

        return false;
    }
    

    auto* p_encoded_data =              encoded_string.data();
    pair<unsigned char, unsigned char>  decoded_hex;

    for(	uint32_t	count	=	0;
    					count	<	encoded_string.size();
    					count	+=	2)
    {
        if(     false == HexCharToNum(p_encoded_data[count], decoded_hex.first)
            ||  false == HexCharToNum(p_encoded_data[count+1], decoded_hex.second))
        {
            try
            {
                string  hex_byte_str;
                hex_byte_str.append( &encoded_string.data()[count], 2);
                m_last_error.append("HexString::Decode(): fail to decode string '");
                m_last_error.append(hex_byte_str);
                m_last_error.append("'");
            }
            catch(...)
            {}

            decoded_data.clear();
            return false;
        }

        p_decoded_data[count/2] = (decoded_hex.first * 16 + decoded_hex.second);
    }

	return true;
}


bool
HexString::
HexCharToNum( unsigned char char_text, unsigned char& hex_value)
{
	switch( char_text )
	{
		case '0':
			hex_value = 0; return true;

		case '1':
			hex_value = 1; return true;

		case '2':
			hex_value = 2; return true;

		case '3':
			hex_value = 3; return true;

		case '4':
			hex_value = 4; return true;

		case '5':
			hex_value = 5; return true;

		case '6':
			hex_value = 6; return true;

		case '7':
			hex_value = 7; return true;

		case '8':
			hex_value = 8; return true;

		case '9':
			hex_value = 9; return true;

		case 'A':
			hex_value = 10; return true;

		case 'B':
			hex_value = 11; return true;

		case 'C':
			hex_value = 12; return true;

		case 'D':
			hex_value = 13; return true;

		case 'E':
			hex_value = 14; return true;

		case 'F':
			hex_value = 15; return true;
	}

    switch( char_text )
	{
		case 'a':
			hex_value = 10; return true;

		case 'b':
			hex_value = 11; return true;

		case 'c':
			hex_value = 12; return true;

		case 'd':
			hex_value = 13; return true;

		case 'e':
			hex_value = 14; return true;

		case 'f':
			hex_value = 15; return true;
	}

	return false;
}


}

}