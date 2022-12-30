#include "version.hpp"

#include <sstream>
#include <array>

#define EXPAND(x) #x
// quotes are added automatically when a macro calls a macro
#define EXPAND_QUOTED(x) EXPAND(x)

auto version_str() -> std::string
{
    std::stringstream buf;
    buf
#ifdef DEBUG
    << "DBG-"
#endif // #ifdef DEBUG
    << EXPAND_QUOTED(PROGRAM_VERSION);

    return buf.str();
}

