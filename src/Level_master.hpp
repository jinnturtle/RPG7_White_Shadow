#ifndef SRC_LEVEL_MASTER_HPP_
#define SRC_LEVEL_MASTER_HPP_

#include <vector>

#include "Level_map.hpp"
#include "Tile.hpp"
#include "Creature.hpp"

/* Level_master contains logic pertaining to the gameplay interactions between
 * the various objects in the Level.*/

class Level_master final {
public:
    Level_master(Level_map* map);
    ~Level_master() = default;

    auto update(std::vector<Creature_control_command>* commands) -> void;

private:
    auto move_creatures(
            std::vector<Creature_control_command>* commands) -> void;
    auto move_player(
            Creature* player,
            std::vector<Creature_control_command>* commands) -> void;

    Level_map* map;
};

#endif // define SRC_LEVEL_MASTER_HPP_
