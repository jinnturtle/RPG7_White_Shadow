#ifndef SRC_LOGS_HPP_
#define SRC_LOGS_HPP_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

extern "C" {
#include "timestamp.h"
}

namespace logs {
    enum Err_type {
        ERR_sdl = 0, // SDL errors
        ERR_img, // SDL Img errors
        ERR_ttf, // SDL TTF errors
    };

    static std::ostream* std_out = &std::cout;
    static std::ostream* err_out = &std::cerr;

    // print specialised error message
    void errt(Err_type err, const std::string& txt);

    template<typename... Ts>
    void print(std::ostream* out, Ts... args)
    {
        std::stringstream buf;
        (buf << ... << args);
        (*out) << buf.str() << std::endl;
    }

    // TODO this will be called a lot, prob worth a look into inlining or smth
    template<typename... Ts>
    void dbg(Ts... args)
    {
        print(std_out, "DEBUG ", timestamp_nano(), " ", args...);
    }

    // log into error output (stderr, should be configurable in the future)
    template<typename... Ts>
    void err(Ts... args)
    {
        print(err_out, "ERROR ", timestamp_nano(), " ", args...);
    }

} // namespace logs

#ifndef SRC_POS
    #define SRC_POS "[", __FILE__, ":", __LINE__, "]"
#else
    #error SRC_POS already defined
#endif

#ifdef DEBUG
    #define DBG(verbocity, ...) if ((DEBUG) >= (verbocity) || (DEBUG) == -1) {\
    logs::dbg(SRC_POS, " ", __VA_ARGS__);\
}

#else
#   define DBG(verbocity, ...) // nothing to do if not a debug build
#endif // DEBUG

#endif // SRC_LOGS_HPP_
