#include "Font_atlas.hpp"

#include <string>

#include "helpers.hpp"
#include "logs.hpp"

// ASCII printable characters: first - 32, last - 126
enum : char {
    Ascii_first_printable = 32,
    Ascii_last_printable = 126
};

Font_atlas::Font_atlas(SDL_Renderer* ren,
                       const std::string& fpath,
                       int size,
                       SDL_Color fg,
                       SDL_Color bg,
                       Txt_raster_type ras_t)
: glyphs{nullptr}
{
    std::string glyphs_txt(
        Ascii_last_printable - Ascii_first_printable + 1, '?');
    for (char ch {Ascii_first_printable}; ch <= Ascii_last_printable; ++ch) {
        glyphs_txt[ch - Ascii_first_printable] = ch;
    }

    this->glyphs = rasterize_txt(glyphs_txt, size, fg, bg, fpath, ren, ras_t);

    if (this->glyphs == nullptr) {
        logs::err("font atlas - no glyphs texture!");
        return;
    }

    int w{0};
    int h{0};
    SDL_QueryTexture(this->glyphs, nullptr, nullptr, &w, &h);
    if (w < 1 || h < 1) {
        logs::err("font atlas - bad glyphs texture!");
        return;
    }
}

Font_atlas::~Font_atlas()
{
    DBG(9, "destroying font atlas");
    SDL_DestroyTexture(this->glyphs);
}

// ----------

Font_atlas_mono::Font_atlas_mono(
    SDL_Renderer* ren,
    const std::string& fpath,
    int size,
    SDL_Color fg,
    SDL_Color bg,
    Txt_raster_type ras_t)
: Font_atlas(ren, fpath, size, fg, bg, ras_t)
, glyph_rect{0, 0, 0, 0}
{
    SDL_Texture* glyph = rasterize_txt("X", size, fg, bg, fpath, ren, ras_t);
    SDL_QueryTexture(
        glyph,
        nullptr,
        nullptr,
        &this->glyph_rect.w,
        &this->glyph_rect.h);
}

auto Font_atlas_mono::render(
    const char* txt,
    size_t n,
    Vec2* pos,
    SDL_Renderer* ren) -> void
{
    SDL_Rect src_rect{this->glyph_rect};
    SDL_Rect dst_rect{
        .x = pos->x,
        .y = pos->y,
        .w = this->glyph_rect.w,
        .h = this->glyph_rect.h};

    for (size_t i {0}; i < n; ++i) {
        src_rect.x = this->glyph_rect.w * (*(txt+i) - Ascii_first_printable);

        SDL_RenderCopy(ren, this->glyphs, &src_rect, &dst_rect);
        dst_rect.x += this->glyph_rect.w;
    }
}

auto Font_atlas_mono::render(
    const char* txt,
    Vec2* pos,
    SDL_Renderer* ren) -> void
{
    SDL_Rect src_rect{this->glyph_rect};
    SDL_Rect dst_rect{
        .x = pos->x,
        .y = pos->y,
        .w = this->glyph_rect.w,
        .h = this->glyph_rect.h};

    while (*txt != 0) {
        src_rect.x = this->glyph_rect.w * (*(txt) - Ascii_first_printable);

        SDL_RenderCopy(ren, this->glyphs, &src_rect, &dst_rect);
        dst_rect.x += this->glyph_rect.w;
        ++txt;
    }
}

auto Font_atlas_mono::make_tex(
    const char* txt,
    size_t n,
    SDL_Renderer* ren) -> SDL_Texture*
{
    SDL_Texture* tex {SDL_CreateTexture(
        ren,
        SDL_PIXELFORMAT_RGBA8888,
        static_cast<int>(SDL_TEXTUREACCESS_TARGET),
        this->glyph_rect.w * n,
        this->glyph_rect.h)};
    if (tex == nullptr) {
        logs::errt(logs::ERR_sdl, "could not create texture for text rendering");
    }

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(ren, tex);
    Vec2 ren_pos {.x = 0, .y = 0};
    this->render(txt, n, &ren_pos, ren);
    SDL_SetRenderTarget(ren, nullptr);

    return tex;
}
