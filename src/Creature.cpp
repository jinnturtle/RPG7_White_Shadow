#include "Creature.hpp"

auto Creature_human::render(App_environment* app) -> void
{
    // TODO get rid of magic numbers
    SDL_Rect rect {.x = this->pos.x, .y = this->pos.y, .w = 32, .h = 32};
    rect.x *= rect.w;
    rect.y *= rect.h;
    SDL_RenderCopy(app->ren, app->texs[TEX_IDX_human], nullptr, &rect);
}
