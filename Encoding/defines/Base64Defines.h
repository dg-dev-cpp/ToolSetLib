#ifndef TOOL_SET_LIB_ENCODING_BASE64_DEFINES
#define TOOL_SET_LIB_ENCODING_BASE64_DEFINES

#include <string>


namespace ToolSetLib
{

namespace Encoding
{

#define MAX_BASE64_DATA_SIZE ((uint64_t)8*1024*1024*1024)

static const std::string g_base64_chars =   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                            "abcdefghijklmnopqrstuvwxyz"
                                            "0123456789+/";
}

}

#endif