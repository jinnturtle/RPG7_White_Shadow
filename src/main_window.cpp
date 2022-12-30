#include "main_window.hpp"

#include <array>

#include <SDL2/SDL.h>

#include "FPS_manager.hpp"
#include "logs.hpp"
#include "helpers.hpp"
#include "itoa.hpp"

struct Tile final {
public:
    Default_texture tx;
    bool passable;
};

class Creature {
public:
    Creature(Default_texture tx, size_t x, size_t y);
    virtual ~Creature();

//private:
    Default_texture tx;
    size_t x;
    size_t y;
};

Creature::Creature(Default_texture tx, size_t x, size_t y)
: tx{tx}
, x{x}
, y{y}
{}

Creature::~Creature(){}

auto run_main_window(App_environment* app) -> void
{
    DBG(3, "running main window");

    FPS_manager fps_man;

    Tile floor {.tx = DEF_TEX_floor, .passable = true};
    Tile wall {.tx = DEF_TEX_wall, .passable = false};

    constexpr size_t map_w {20};
    constexpr size_t map_h {20};
    std::array<Tile*, map_w * map_h> tiles;

    for (auto& tile : tiles) {
        tile = &floor;
    } 
    tiles[7 + 5*map_w] = &wall;
    tiles[7 + 6*map_w] = &wall;
    tiles[7 + 7*map_w] = &wall;
    tiles[8 + 7*map_w] = &wall;
    tiles[9 + 7*map_w] = &wall;

    int tile_w, tile_h;
    if (SDL_QueryTexture(
                app->texs[wall.tx], nullptr, nullptr, &tile_w, &tile_h))
    {
        logs::errt(logs::ERR_sdl, "while querying texture");
    }

    Creature player(DEF_TEX_human, 2, 2);

    // main loop
    bool exit {false};
    bool show_fps {true};
    SDL_Event event {0};
    std::array<char, 8> fps_buf {0};
    Vec2 fps_pos {.x = 0, .y = 0};
    while (!exit) {
        // input stage

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_h) {
                    size_t new_x = player.x - 1;
                    if (tiles[new_x + player.y * map_w]->passable) {
                        --player.x;
                    }
                } else if (event.key.keysym.sym == SDLK_j) {
                    size_t new_y = player.y + 1;
                    if (tiles[player.x + new_y * map_w]->passable) {
                        ++player.y;
                    }
                } else if (event.key.keysym.sym == SDLK_k) {
                    size_t new_y = player.y - 1;
                    if (tiles[player.x + new_y * map_w]->passable) {
                        --player.y;
                    }
                } else if (event.key.keysym.sym == SDLK_l) {
                    size_t new_x = player.x + 1;
                    if (tiles[new_x + player.y * map_w]->passable) {
                        ++player.x;
                    }
                } else if (event.key.keysym.sym == SDLK_f) {
                    if (event.key.keysym.mod & KMOD_SHIFT) {
                        fps_man.toggle_cap();
                    } else {
                        show_fps = !show_fps;
                        if (show_fps) {
                            DBG(9, "show fps counter");
                        } else {
                            DBG(9, "hide fps counter");
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_q) {
                    exit = true;
                }
            } else if (event.type == SDL_QUIT) { exit = true; }
        }

        // update stage

        // render stage

        SDL_SetRenderDrawColor(app->ren, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(app->ren);

        {
        SDL_Rect tile_draw_rect {.x = 0, .y = 0, .w = tile_w, .h = tile_h};
        for (size_t y {0}; y < map_h; ++y) {
            tile_draw_rect.y = y * tile_h;
            for (size_t x {0}; x < map_w; ++x) {
                tile_draw_rect.x = x * tile_w;
                Default_texture tx_i = tiles[x + y*map_w]->tx;

                SDL_RenderCopy(
                        app->ren, app->texs[tx_i], nullptr, &tile_draw_rect);

                if (player.x == x && player.y == y) {
                    SDL_RenderCopy(
                            app->ren, app->texs[DEF_TEX_human],
                            nullptr, &tile_draw_rect);
                }
            }
        }
        }

        if (show_fps) {
            /* size - 2 so we keep the null at the end to represent text as a
             * null-terminated string */
            app->fonts[DEF_FONT_mono_fast]->render(
                // TODO - implement and use utoa (fps is unsigned)
                itoa(fps_man.get_fps(), &fps_buf[fps_buf.size()-2]),
                &fps_pos,
                app->ren);
        }

        SDL_RenderPresent(app->ren);

        fps_man.end_frame();
    }

    DBG(3, "end of main window");
}
