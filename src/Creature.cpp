#include "Creature.hpp"

#include "logs.hpp"

Creature::Creature(const std::string& name,
                   const Creature_db_entry* data,
                   SDL_Texture* tex)
:name {name}
,pos {0,0}
,tex {tex}
,dmg {data->dmg}
,max_hp {data->max_hp}
,hp {data->max_hp}
,human_control {false}
{}

int Creature::get_hp()
{
    return this->hp;
}

bool Creature::get_human_control()
{
    return this->human_control;
}

int Creature::get_max_hp()
{
    return this->max_hp;
}

std::string Creature::get_name()
{
    return this->name;
}

const Vec2u* Creature::get_pos()
{
    return &this->pos;
}

void Creature::set_pos(const Vec2u* pos)
{
    this->pos = *pos;
}

void Creature::set_human_control(bool val)
{
    this->human_control = val;
}

void Creature::move(Creature_control_command cmd)
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

void Creature::render(SDL_Renderer* ren)
{
    if (this->tex == nullptr) {
        logs::err("null texture render request for creature ",
            this->name, " at " , this->pos.x, "x", this->pos.y);
        return;
    }

    SDL_Rect rect;
    SDL_QueryTexture(this->tex, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = static_cast<int>(this->get_pos()->x) * rect.w;
    rect.y = static_cast<int>(this->get_pos()->y) * rect.h;

    SDL_RenderCopy(ren, this->tex, nullptr, &rect);
}
