#include "logs.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <array>
#include <sstream>

auto logs::errt(Err_type err, const std::string& txt) -> void
{
    std::stringstream buf;

    buf << "ERROR " << timestamp_nano() << " " << txt;

    switch (err) {
        case ERR_sdl:
            buf << " SDL: " << SDL_GetError();
            break;
        case ERR_img:
            buf << " SDL_IMG: " << IMG_GetError();
            break;
        case ERR_ttf:
            buf << " SDL_TTF: " << TTF_GetError();
            break;
        default:
            buf << " (unknow error type): ";
            break;
    }

    buf << std::endl;

    (*logs::err_out) << buf.str();
}

