#ifndef SRC_LEVEL_MAP_HPP_
#define SRC_LEVEL_MAP_HPP_

#include <vector>

#include "App_environment.hpp"
#include "Tile.hpp"
#include "Creature.hpp"
#include "logs.hpp"

class Level_map final {
public:
    Level_map(size_t w, size_t h, size_t tile_w, size_t tile_h);
    ~Level_map();

    friend class Level_master;

    auto get_creature(size_t x, size_t y) -> Creature*;
    auto get_tile(size_t x, size_t y) -> const Tile*;

    auto add_creature(Creature* creature) -> void;
    auto put_tile(size_t x, size_t y, Tile* tile) -> void;
    auto render(App_environment* app) -> void;
    //auto replace_obj(size_t x, size_t y, size_t l, Level_object* obj) -> void;

private:
    //TODO implement a simple Matrix?
    std::vector<std::vector<Tile*>*>* tiles;
    std::vector<Creature*> creatures;
    size_t tile_w;
    size_t tile_h;

    // returns true if x/y is within bounds, false otherwise
    auto check_bounds(size_t x, size_t y) -> bool;
};

#endif // define SRC_LEVEL_MAP_HPP_
