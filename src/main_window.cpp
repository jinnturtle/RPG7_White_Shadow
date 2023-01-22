#include "main_window.hpp"

#include <array>
#include <vector>
#include <functional>
#include <string>

#include <SDL2/SDL.h>

#include "FPS_manager.hpp"
#include "Level_map.hpp"
#include "Level_master.hpp"
#include "Tile.hpp"
#include "Creature.hpp"
#include "logs.hpp"
#include "helpers.hpp"
#include "itoa.hpp"
#include "version.hpp"

auto run_main_window(App_environment* app) -> void
{
    DBG(3, "running main window");

    FPS_manager fps_man;

    constexpr size_t map_w {20};
    constexpr size_t map_h {20};
    constexpr size_t tile_w {32};
    constexpr size_t tile_h {32};

    const std::string app_version(app->title + " " + version_str());

    // generating a test map
    // TODO ugly test code, kill it with fire
    Level_map level(map_w, map_h, tile_w, tile_h);
    for (size_t x {0}; x < map_w; ++x) {
        for (size_t y {0}; y < map_h; ++y) {
            if (x == 7) {
                if (y == 5 || y == 6 || y == 7) {
                    level.put_tile(x, y, new Tile_wall);
                }
                else {
                    level.put_tile(x, y, new Tile_floor);
                }
            } else if (y == 7) {
                if (x == 8 || x == 9) {
                    level.put_tile(x, y, new Tile_wall);
                }
                else {
                    level.put_tile(x, y, new Tile_floor);
                }
            } else {
                level.put_tile(x, y, new Tile_floor);
            }
        }
    }

    level.add_creature(new Creature_human({.x = 5, .y = 3}, true));
    level.add_creature(new Creature_human({.x = 8, .y = 6}));

    Level_master level_master(&level);

    // main loop
    bool exit {false};
    bool show_fps {true};
    SDL_Event event {0};
    std::array<char, 8> fps_buf {0};
    Vec2 txt_pos {.x = 0, .y = 0}; // controls render position of text objects
    // will prob need a FIFO queue
    std::vector<Creature_control_command> player_input;
    player_input.reserve(5);
    while (!exit) {
        // input stage

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_h) {
                    // player movement
                    player_input.push_back(CCC_move_left);
                } else if (event.key.keysym.sym == SDLK_j) {
                    // player movement
                    player_input.push_back(CCC_move_down);
                } else if (event.key.keysym.sym == SDLK_k) {
                    // player movement
                    player_input.push_back(CCC_move_up);
                } else if (event.key.keysym.sym == SDLK_l) {
                    // player movement
                    player_input.push_back(CCC_move_right);
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

        level_master.update(&player_input);
        player_input.clear();

        // render stage

        SDL_SetRenderDrawColor(app->ren, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(app->ren);

        level.render(app);

        if (show_fps) {
            txt_pos.y = 0;
            /* size - 2 so we keep the null at the end to represent text as a
             * null-terminated string */
            app->fonts[FONT_IDX_mono_fast]->render(
                // TODO - implement and use utoa (fps is unsigned)
                itoa(fps_man.get_fps(), &fps_buf[fps_buf.size()-2]),
                &txt_pos,
                app->ren);
        }

        txt_pos.y = app->win_h - 24;
        app->fonts[FONT_IDX_mono_fast]->render(
            app_version.c_str(),
            &txt_pos,
            app->ren);

        SDL_RenderPresent(app->ren);

        fps_man.end_frame();
    }

    DBG(3, "end of main window");
}
