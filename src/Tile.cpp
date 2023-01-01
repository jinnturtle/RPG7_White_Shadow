#include "Tile.hpp"

auto Tile_floor::render(App_environment* app, SDL_Rect* rect) -> void
{
    SDL_RenderCopy(app->ren, app->texs[TEX_IDX_floor], nullptr, rect);
}

auto Tile_wall::render(App_environment* app, SDL_Rect* rect) -> void
{
    SDL_RenderCopy(app->ren, app->texs[TEX_IDX_wall], nullptr, rect);
}
