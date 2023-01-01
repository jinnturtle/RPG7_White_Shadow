#include "App_environment.hpp"

#include <string>
#include <array>

#include "logs.hpp"

App_environment::App_environment(std::string title, int win_w, int win_h)
: texs{}
, fonts{}
, title{title}
, win_w{win_w}
, win_h{win_h}
, win{nullptr}
, ren{nullptr}
{
    for (auto& tex : this->texs) {
        tex = nullptr;
    }
    for (auto& font : this->fonts) {
        font = nullptr;
    }
}

App_environment::~App_environment()
{
    for (auto& tex : this->texs) {
        SDL_DestroyTexture(tex);
    }
    for (auto& font : this->fonts) {
        delete font;
    }
}


auto App_environment::load_textures() -> void
{
    if (this->ren == nullptr) {
        logs::err("can not load app textures: renderer is nil");
        return;
    }

    std::array<std::string, TEX_IDX_array_size> paths;
    paths[TEX_IDX_human] = "data/gfx/human.png";
    paths[TEX_IDX_floor] = "data/gfx/floor.png";
    paths[TEX_IDX_wall]  = "data/gfx/wall.png";

    for (size_t i {0}; i < this->texs.size(); ++i) {
        this->texs.at(i) = load_texture(paths.at(i), this->ren);
        if (this->texs.at(i) == nullptr) {
            this->set_err("could not load default texture " + paths[i]);
            logs::err(this->get_err_txt());
            return;
        }
    }
}

auto App_environment::load_fonts() -> void
{
    if (this->ren == nullptr) {
        logs::err( "can not load app fonts: renderer is nil");
        return;
    }

    constexpr int font_size {16};

    std::array<std::string, FONT_IDX_array_size> paths;
    paths[FONT_IDX_mono_blend] = "data/fonts/terminus/TerminusTTF-4.46.0.ttf";
    paths[FONT_IDX_mono_fast]  = "data/fonts/terminus/TerminusTTF-4.46.0.ttf";

    for (size_t i {0}; i < this->fonts.size(); ++i) {
        this->fonts.at(i) = new Font_atlas_mono(
            this->ren,
            paths.at(i),
            font_size,
            SDL_Color{0xff, 0xff, 0xff, 0xff},
            SDL_Color{0x00, 0x00, 0x00, 0x00},
            Raster_blended);
    }

//     this->fonts[FONT_IDX_mono_blend] = new Font_atlas_mono(
//         this->ren,
//         "data/fonts/terminus/TerminusTTF-4.46.0.ttf",
//         16,
//         SDL_Color{0xff, 0xff, 0xff, 0xff},
//         SDL_Color{0x00, 0x00, 0x00, 0x00},
//         Raster_blended);
//
//     this->fonts[FONT_IDX_mono_fast] = new Font_atlas_mono(
//         this->ren,
//         "data/fonts/terminus/TerminusTTF-4.46.0.ttf",
//         16,
//         SDL_Color{0xff, 0xff, 0xff, 0xff},
//         SDL_Color{0x00, 0x00, 0x00, 0x00},
//         Raster_shaded);
}

auto App_environment::load_default_assets() -> void
{
    this->load_textures();
    if (!this->err()) { this->load_fonts(); }
}

auto App_environment::err() -> bool
{
    return !this->err_txt.empty();
}


auto App_environment::get_err_txt() -> const char*
{
    return this-> err_txt.c_str();
}

// private ---------------------------------------------------------------------

auto App_environment::set_err(const std::string& txt) -> void
{
    this->err_txt = txt;
}
