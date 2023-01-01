#ifndef SRC_APP_ENVIRONMENT_HPP_
#define SRC_APP_ENVIRONMENT_HPP_

#include <string>
#include <array>

#include "Font_atlas.hpp"

enum Texture_index {
    TEX_IDX_human = 0,
    TEX_IDX_floor,
    TEX_IDX_wall,

    TEX_IDX_array_size
};

enum Font_index {
    FONT_IDX_mono_fast = 0,
    FONT_IDX_mono_blend,

    FONT_IDX_array_size
};

class App_environment final {
public:
    App_environment(std::string title, int win_w, int win_h);
    ~App_environment();

    auto load_default_assets() -> void;
    auto load_textures() -> void;
    auto load_fonts() -> void;

    auto err() -> bool; // returns true if there is an error
    auto get_err_txt() -> const char*;

    std::array<SDL_Texture*, TEX_IDX_array_size> texs; // default textures
    std::array<Font_atlas*, FONT_IDX_array_size> fonts; // default fonts
    std::string title; // title/name of the application
    int win_w; // main window width
    int win_h; // main window height
    SDL_Window* win; // main window
    SDL_Renderer* ren; // main renderer

private:
    auto set_err(const std::string& txt) -> void;
    std::string err_txt; // contains error text if an error occured
};

#endif // SRC_APP_ENVIRONMENT_HPP_
