#include "Creature.hpp"

Creature::Creature(Vec2u pos, bool human_control)
: pos {pos}
, human_control {human_control}
{}


auto Creature::get_pos() -> const Vec2u*
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
            if (this->pos.y > 0) { this->pos.y--; }
            break;
        case CCC_move_down:
            this->pos.y++;
            break;
        case CCC_move_left:
            if (this->pos.x > 0) { this->pos.x--; }
            break;
        case CCC_move_right:
            this->pos.x++;
            break;
        default:
            break;
    }
}

Creature_human::Creature_human(Vec2u pos, bool human_control)
: Creature(pos, human_control)
{}

auto Creature_human::render(App_environment* app) -> void
{
    // TODO get rid of magic numbers
    SDL_Texture* tex = app->texs[TEX_IDX_human];
    SDL_Rect rect;
    SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = static_cast<int>(this->get_pos()->x) * rect.w;
    rect.y = static_cast<int>(this->get_pos()->y) * rect.h;

    SDL_RenderCopy(app->ren, tex, nullptr, &rect);
}

Creature_gray_goo::Creature_gray_goo(Vec2u pos, bool human_control)
: Creature(pos, human_control)
{
    //TODO
}
