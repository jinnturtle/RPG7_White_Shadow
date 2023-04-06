#include "helpers.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>

#include "logs.hpp"

SDL_Surface* load_surface(const std::string& path)
{
    DBG(10, "loading surface from '", path, "'");

    SDL_Surface* sfc {IMG_Load(path.c_str())};
    if (sfc == nullptr) {
        logs::errt(logs::ERR_img, "could not load surface from " + path);
        return nullptr;
    }

    return sfc;
}

SDL_Texture* load_texture(const std::string& path, SDL_Renderer* ren)
{
    DBG(2, "loading texture from '", path, "'");

    SDL_Surface* sfc {load_surface(path)};
    if (sfc == nullptr) { return nullptr; }

    SDL_Texture* tex {SDL_CreateTextureFromSurface(ren, sfc)};
    SDL_FreeSurface(sfc);
    sfc = nullptr;

    if (tex == nullptr) {
        logs::errt(
                logs::ERR_sdl, "could not create texture form surface " + path);

        return nullptr;
    }

    return tex;
}

SDL_Texture* rasterize_txt(
    const std::string& txt, int size,
    SDL_Color fg, SDL_Color bg,
    const std::string& fpath, SDL_Renderer* ren,
    Txt_raster_type ras_t)
{
    DBG(9, "rasterizing text: '", txt, "'");

    TTF_Font* font {TTF_OpenFont(fpath.c_str(), size)};
    if (font == nullptr) {
        logs::errt(logs::ERR_ttf, "could not open font " + fpath);
        return nullptr;
    }

    SDL_Surface* sfc {nullptr};
    switch (ras_t) {
        case Raster_solid:
            sfc = TTF_RenderText_Solid(font, txt.c_str(), fg);
            break;
        case Raster_shaded:
            sfc = TTF_RenderText_Shaded(font, txt.c_str(), fg, bg);
            break;
        case Raster_blended:
            sfc = TTF_RenderText_Blended(font, txt.c_str(), fg);
            break;
        default:
            logs::err("invlid text raster method selected");
            return nullptr;
    }

    TTF_CloseFont(font);
    font = nullptr;
    if (sfc == nullptr) {
        logs::errt(logs::ERR_ttf, "could not render text onto surface");

        return nullptr;
    }

    SDL_Texture* tex {SDL_CreateTextureFromSurface(ren, sfc)};
    SDL_FreeSurface(sfc);
    sfc = nullptr;
    if (tex == nullptr) {
        logs::errt(logs::ERR_sdl,
                  "(rasterize) could not create texture from surface");

        return nullptr;
    }

    return tex;
}
