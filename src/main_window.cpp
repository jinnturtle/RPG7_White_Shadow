#include "main_window.hpp"

#include <array>
#include <vector>
#include <functional>
#include <string>

#include <SDL2/SDL.h>

#include "Creature_spawner.hpp"
#include "FPS_manager.hpp"
#include "Level_map.hpp"
#include "Level_master.hpp"
#include "Test_map.hpp"
#include "Tile.hpp"
#include "Tile_db.hpp"
#include "Tile_palette.hpp"
#include "helpers.hpp"
#include "itoa.hpp"
#include "logs.hpp"
#include "version.hpp"

void run_main_window(App_environment* app)
{
    DBG(3, "running main window");

    const std::string app_version(app->title + " " + version_str());

    FPS_manager fps_man;

    Level_map* level = test_map::generate(app);
    Level_master level_master(level);

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

        level->render();

        if (show_fps) {
            txt_pos.y = 0;
            /* size -2 so we keep the null at the end to represent text as a
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
