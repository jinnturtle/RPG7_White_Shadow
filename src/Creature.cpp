#include "Creature.hpp"

Creature::Creature(Vec2 pos, bool human_control)
: pos {pos}
, human_control {human_control}
{}


auto Creature::get_pos() -> const Vec2*
{
    return &this->pos;
}

auto Creature::is_human_controlled() -> bool
{
    return this->human_control;
}

auto Creature::move(Creature_control_command cmd) -> void
{
    switch(cmd) {
        case CCC_move_up:
            this->pos.y--;
            break;
        case CCC_move_down:
            this->pos.y++;
            break;
        case CCC_move_left:
            this->pos.x--;
            break;
        case CCC_move_right:
            this->pos.x++;
            break;
        default:
            break;
    }
}

Creature_human::Creature_human(Vec2 pos, bool human_control)
: Creature(pos, human_control)
{}

auto Creature_human::render(App_environment* app) -> void
{
    // TODO get rid of magic numbers
    SDL_Rect rect {
        .x = this->get_pos()->x, .y = this->get_pos()->y, .w = 32, .h = 32};
    rect.x *= rect.w;
    rect.y *= rect.h;
    SDL_RenderCopy(app->ren, app->texs[TEX_IDX_human], nullptr, &rect);
}
