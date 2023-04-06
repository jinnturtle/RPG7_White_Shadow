#include "Level_master.hpp"

#include "logs.hpp"

Level_master::Level_master(Level_map* map)
: map {map}
{}

void Level_master::move_creatures(
        std::vector<Creature_control_command>* commands)
{
    for (auto& creature : this->map->creatures) {
        if (creature->get_human_control()) {
            this->move_player(creature, commands);
        }
    }
}

void Level_master::move_player(
        Creature* player,
        std::vector<Creature_control_command>* commands)
{
    if (commands->size() == 0) { return; }

    Vec2u tgt_pos = *player->get_pos();
    for (size_t i {0}; i < commands->size(); ++i) {
        switch(commands->at(i)) {
            case CCC_move_up:
                if (tgt_pos.y > 0) { --tgt_pos.y; }
                break;
            case CCC_move_down:
                ++tgt_pos.y;
                break;
            case CCC_move_left:
                if (tgt_pos.x > 0) { --tgt_pos.x; }
                break;
            case CCC_move_right:
                ++tgt_pos.x;
                break;
            default:
                return;
        }
    }

    if (this->map->get_tile(tgt_pos.x, tgt_pos.y)->get_passable() == false) {
        DBG(8, "won't move due to terrain at ", tgt_pos.x, "x", tgt_pos.y);
        return;
    }
    Creature* tgt_creature = this->map->get_creature(tgt_pos.x, tgt_pos.y);
    if (tgt_creature != nullptr) {
        DBG(8, "WIP: staring menacingly at the ",
                tgt_creature->get_name(),
                " in ",
                tgt_pos.x, "x", tgt_pos.y,
                " because combat is not implemented yet");
        return;
    }


    for (size_t i {0}; i < commands->size(); ++i) {
        player->move(commands->at(i));
    }
}

void Level_master::update(std::vector<Creature_control_command>* commands)
{
    this->move_creatures(commands);
}
