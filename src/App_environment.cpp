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

// Will later be usefull for loading of UI and other general textures that
// should not be loaded dynamically.
void App_environment::load_textures()
{
    if (this->ren == nullptr) {
        logs::err("can not load app textures: renderer is nil");
        return;
    }

    std::array<std::string, TEX_IDX_array_size> paths;
    /* tile textures are now loaded dynamically elsewhere, here we will load
     * common textures (e.g. UI stuff) in the future*/
    //paths[TEX_IDX_tile_floor]             = "data/gfx/floor.png";
    //paths[TEX_IDX_tile_wall]              = "data/gfx/wall.png";

    for (size_t i {0}; i < this->texs.size(); ++i) {
        this->texs.at(i) = load_texture(paths.at(i), this->ren);
        if (this->texs.at(i) == nullptr) {
            this->set_err("could not load default texture " + paths[i]);
            logs::err(this->get_err_txt());
            return;
        }
    }
}

void App_environment::load_fonts()
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

/* TODO figure this out:
 * Old Self you crazy ninja, why on earth is this commented out, why the
 * Font_index entries are not, why if I tweak this it seems to have no effect?
 * Plz at least leave a comment next time.
 *
 * I'm pretty sure shaded and blended rasterization used to behave differently
 * from each other, now they don't seem to, need to dig deeper to figure out it
 * seems. One was a good bit faster than the other, but didn't offer a
 * "transparent" background IIRC.*/

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

void App_environment::load_default_assets()
{
    this->load_textures();
    if (!this->err()) { this->load_fonts(); }
}

bool App_environment::err()
{
    return !this->err_txt.empty();
}


const char* App_environment::get_err_txt()
{
    return this-> err_txt.c_str();
}

// private ---------------------------------------------------------------------

void App_environment::set_err(const std::string& txt)
{
    this->err_txt = txt;
}
