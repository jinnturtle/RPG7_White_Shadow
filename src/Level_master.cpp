#include "Level_master.hpp"

#include "logs.hpp"

Level_master::Level_master(Level_map* map)
: map {map}
{}

auto Level_master::move_creatures(
        std::vector<Creature_control_command>* commands) -> void
{
    for (auto& creature : this->map->creatures) {
        if (creature->is_human_controlled()) {
            this->move_player(creature, commands);
        }
    }
}

auto Level_master::move_player(
        Creature* player,
        std::vector<Creature_control_command>* commands) -> void
{
    Vec2 tgt_pos = *player->get_pos();
    for (size_t i {0}; i < commands->size(); ++i) {
        switch(commands->at(i)) {
            case CCC_move_up:
                --tgt_pos.y;
                break;
            case CCC_move_down:
                ++tgt_pos.y;
                break;
            case CCC_move_left:
                --tgt_pos.x;
                break;
            case CCC_move_right:
                ++tgt_pos.x;
                break;
            default:
                break;
        }
    }
    const Tile* tgt_tile = this->map->get_tile(tgt_pos.x, tgt_pos.y);
    if (tgt_tile->is_passable() == false) {
        DBG(8, "won't move due to terrain at ", tgt_pos.x, "x", tgt_pos.y);
        return; // can't move due to impassable terrain
    }

    for (size_t i {0}; i < commands->size(); ++i) {
        player->move(commands->at(i));
    }
}

auto Level_master::update(
        std::vector<Creature_control_command>* commands) -> void
{
    this->move_creatures(commands);
}
