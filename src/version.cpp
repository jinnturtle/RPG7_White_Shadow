#include "version.hpp"

#include <sstream>
#include <array>

auto version_str() -> std::string
{
    constexpr int major {0};
    constexpr int minor {0};
    constexpr int micro {0};
    constexpr const char* prefix {""};
    constexpr const char* postfix {"prototype-1"};

    std::stringstream buf;
    buf
#ifdef DEBUG
    << "DBG-"
#endif // #ifdef DEBUG
    << prefix << ((*prefix != 0)? "-" : "")
    << "v" << major << "."
    << minor << "."
    << micro
    << ((*postfix != 0)? "-" : "") << postfix;

    return buf.str();
}

