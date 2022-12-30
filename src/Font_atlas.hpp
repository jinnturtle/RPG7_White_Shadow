#ifndef SRC_FONT_ATLAS_HPP_
#define SRC_FONT_ATLAS_HPP_

#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "helpers.hpp"

class Font_atlas {
 public:
    Font_atlas(
        SDL_Renderer* ren,
        const std::string& fpath,
        int size,
        SDL_Color fg,
        SDL_Color bg,
        Txt_raster_type ras_t);
    virtual ~Font_atlas();

    // pass text via pointer to char buffer with length
    virtual auto render(
        const char* txt,
        size_t n,
        Vec2* pos,
        SDL_Renderer* ren) -> void = 0;

    // pass text via null-terminated char string
    virtual auto render(
        const char* txt,
        Vec2* pos,
        SDL_Renderer* ren) -> void = 0;

    virtual auto make_tex(
        const char* start,
        size_t n,
        SDL_Renderer* ren) -> SDL_Texture* = 0;

 protected:
     SDL_Texture* glyphs;
};

// font atlas for monospaced fonts
class Font_atlas_mono final : public Font_atlas {
 public:
    Font_atlas_mono(
        SDL_Renderer* ren,
        const std::string& fpath,
        int size,
        SDL_Color fg,
        SDL_Color bg,
        Txt_raster_type ras_t);

    auto render(
        const char* txt,
        size_t n,
        Vec2* pos,
        SDL_Renderer* ren) -> void override;

    auto render(
        const char* txt,
        Vec2* pos,
        SDL_Renderer* ren) -> void override;

    auto make_tex(
        const char* txt,
        size_t n,
        SDL_Renderer* ren) -> SDL_Texture* override;

 private:
     SDL_Rect glyph_rect; // one rect is enough for monospaced fonts
};

#endif // SRC_FONT_ATLAS_HPP_
