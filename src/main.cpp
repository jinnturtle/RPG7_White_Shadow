// 3rd party libs
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// cpp std lib
#include <iostream>
#include <string>

// local
#include "App_environment.hpp"
#include "version.hpp"
#include "logs.hpp"
#include "main_window.hpp"

auto make_sdl_window(App_environment* app) -> int;
auto make_sdl_renderer(App_environment* app) -> int;
auto init_sdl() -> int;
auto deinit_sdl(App_environment* app) -> void;

auto main() -> int
{
    App_environment app(std::move("RPG7 White Shadow"), 800, 640);

    DBG(1, "starting ", app.title, " version ", version_str());

    if (init_sdl() != 0) {
        logs::err("fatal, could not initialise SDL");
        exit(-1);
    }

    DBG(1, "window resolution: ", app.win_w, "x", app.win_h);

    std::cout << app.title << " " << version_str() << std::endl;

    if (make_sdl_window(&app) != 0) {
        logs::err("fatal, failed to create main window");
        deinit_sdl(&app);
        exit(-1);
    }

    if (make_sdl_renderer(&app) != 0) {
        logs::err("fatal, failed to create sdl renderer");
        deinit_sdl(&app);
        exit(-1);
    }

    app.load_default_assets();
    if (app.err()) {
        logs::err( "could not initalise application");
    } else {
        run_main_window(&app);
    }

    deinit_sdl(&app);
    return 0;
}

auto init_sdl() -> int
{
    DBG(1, "initialising SDL");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logs::errt(logs::ERR_sdl, "FATAL - could not initialise SDL!");
        return -1;
    }

    int sdl_image_flags = IMG_INIT_PNG;
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    if ((IMG_Init(sdl_image_flags) & sdl_image_flags) != sdl_image_flags) {
        logs::errt(logs::ERR_img, "FATAL - could not initialise SDL_image!");
        return -1;
    }

    if (TTF_Init() == -1) {
        logs::errt(logs::ERR_ttf, "FATAL - could not initialise SDL_ttf!");
        return -1;
    }

    return 0;
}

auto make_sdl_window(App_environment* app) -> int
{
    if (app == nullptr) {
        logs::err( "could not create window, app = nil");
    }

    app->win = SDL_CreateWindow(
        app->title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // NOLINT
        app->win_w, app->win_h,
        SDL_WINDOW_SHOWN);
    if (app->win == nullptr) {
        logs::errt(logs::ERR_sdl, "FATAL - SDL window could not be created!");
        return -1;
    }

    return 0;
}

auto make_sdl_renderer(App_environment* app) -> int
{
    if (app == nullptr) {
        logs::err( "could not create renderer, app = nil");
    }

    app->ren = SDL_CreateRenderer(app->win, -1, SDL_RENDERER_ACCELERATED);

    if (app->ren == nullptr) {
        logs::errt(logs::ERR_sdl, "Could not create renderer!");
        return -1;
    }

    // initialise renderer color
    SDL_SetRenderDrawColor(app->ren, 0xFF, 0xFF, 0xFF, 0xFF); // NOLINT

    return 0;
}

auto deinit_sdl(App_environment* app) -> void
{
    DBG(1, "deinitialising SDL");

    if (app->win != nullptr) {SDL_DestroyWindow(app->win);}
    app->win = nullptr;

    if (app->ren != nullptr) {SDL_DestroyRenderer(app->ren);}
    app->ren = nullptr;

    SDL_Quit();
}
