#include "Base64.h"


namespace ToolSetLib
{

namespace Encoding
{

using namespace std;


string
Base64::
Encode(const string& source_str)
{
    m_last_error.clear();

    string decoded_data;

    Encode( source_str.c_str(), source_str.size(), decoded_data);

    return move(decoded_data);
}


bool
Base64::
Encode(const    char*           p_data,
                uint64_t        data_size,
                std::string&    encoded_string)
{
    string& data = encoded_string;

    try
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
                m_last_error.append("Base64::Encode(): incorrect method param: (p_data == nullptr)");
                return false;
            }

            if( data_size > MAX_BASE64_DATA_SIZE)
            {
                m_last_error.append("Base64::Encode(): incorrect method param: data_size > ");
                m_last_error.append(to_string(MAX_BASE64_DATA_SIZE));
                return false;
            }
        }
        const unsigned char* p_base64_chars = (const unsigned char*)g_base64_chars.c_str();

        if( p_base64_chars == nullptr)
        {
            m_last_error.append("Base64::Encode(): internal error: p_base64_chars == nullptr");
            return false;
        }

        if( g_base64_chars.size() != 64)
        {
            m_last_error.append("Base64::Encode(): (g_base64_chars.size() != 64)");
            return false;
        }

        unsigned char* p_bytes = (unsigned char*)p_data;
        uint64_t len =              data_size;

        uint64_t        in_len =    len;
        int             i =         0;
        unsigned char   char_array_3[3];
        unsigned char   char_array_4[4];

        while(in_len--)
        {
            char_array_3[i] = *(p_bytes);

            p_bytes +=  1;
            i +=        1;

            if(i == 3)
            {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                data.push_back(p_base64_chars[char_array_4[0]]);
                data.push_back(p_base64_chars[char_array_4[1]]);
                data.push_back(p_base64_chars[char_array_4[2]]);
                data.push_back(p_base64_chars[char_array_4[3]]);

                i = 0;
            }
        }

        if( i > 0)
        {
            int j = 0;

            for(j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(j = 0; j < ( i + 1); j++)
                data += p_base64_chars[char_array_4[j]];

            while(i++ < 3)
                data += '=';
        }

        return true;
    }
    catch(...)
    {
        try
        {
            m_last_error.append("Base64::Encode(): unknown exception");
        }
        catch(...)
        {}
        
        data.clear();
    }

    return false;
}


bool
Base64::
Decode(    const    std::string&        encoded_string,
                std::vector<char>&    ret)
{
    m_last_error.clear();
    ret.clear();

    if( encoded_string.size() == 0)
        return true;
    
    if( EstimateDecodedBase64DataSize(encoded_string) > MAX_BASE64_DATA_SIZE)
    {
        try
        {
            m_last_error.append("Base64::Decode(): encoded string size is too large");
        }
        catch(...)
        {}

        return false;
    }

    int in_len =    encoded_string.size();
    int i =         0;
    int j =         0;
    int in_ =       0;
    unsigned char char_array_4[4], char_array_3[3];

    try
    {
        while(        in_len--
                && (encoded_string[in_] != '=') )
        {
            if(   encoded_string[in_]    == '\r'
            ||    encoded_string[in_]    == '\n')
            {
                in_++;
                continue;
            }

            if( is_base64(encoded_string[in_]) == false)
            {
                try
                {
                    m_last_error.append("Base64::Decode(): no base 64 symbol: '");
                    m_last_error.append(1, encoded_string[in_]);
                    m_last_error.append("'");
                }
                catch(...)
                {}
                
                ret.clear();

                return false;
            }

            char_array_4[i++] = encoded_string[in_]; in_++;

            if(i == 4)
            {
                for(i = 0; i <4; i++)
                {
                    auto symbol_idx = g_base64_chars.find(char_array_4[i]);

                    if( symbol_idx == std::string::npos)
                    {
                        try
                        {
                            m_last_error.append("Base64::Decode(): fail to find base 64 symbol: '");
                            m_last_error.append( 1, encoded_string[in_]);
                            m_last_error.append("'");
                        }
                        catch(...)
                        {}

                        ret.clear();

                        return false;
                    }

                    char_array_4[i] = symbol_idx;
                }

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for(i = 0; (i < 3); i++)
                    ret.push_back(char_array_3[i]);

                i = 0;
            }
        }

        if( i > 0)
        {
            for(j = i; j <4; j++)
                char_array_4[j] = 0;

            for(j = 0; j <i; j++)
            {
                auto symbol_idx = g_base64_chars.find(char_array_4[j]);

                if(symbol_idx == std::string::npos)
                {
                    try
                    {
                        m_last_error.append("Base64::Decode(): fail to find base 64 symbol: '");
                        m_last_error.append( 1, char_array_4[j]);
                        m_last_error.append("'");
                    }
                    catch(...)
                    {}

                    ret.clear();

                    return false;
                }

                char_array_4[j] = symbol_idx;
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for(j = 0; j < (i - 1); j++)
                ret.push_back(char_array_3[j]);
        }
    }
    catch(...)
    {
        try
        {
            m_last_error.append("Base64::Decode(): unknown exception");
        }
        catch(...)
        {}
        
        ret.clear();
        return false;
    }

    return true;
}


uint64_t
Base64::
EstimateDecodedBase64DataSize(const std::string& data)
{
    m_last_error.clear();

    if(        data.length()        <    4
        ||    (data.length() % 4) !=    0) // source base64 string must be multiply by 4
        return 0;

    size_t decoded_len = 0;

    decoded_len = ( (data.length() - 4) / 4 ) * 3;

    decoded_len += 1;

    if(data[data.length() - 1] != '=')
    {
        decoded_len += 2;
    }
    else if(data[data.length() - 2] != '=')
    {
        decoded_len += 1;
    }

    return decoded_len;
}

}

}