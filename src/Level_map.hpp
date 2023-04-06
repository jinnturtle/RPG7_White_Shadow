#ifndef SRC_LEVEL_MAP_HPP_
#define SRC_LEVEL_MAP_HPP_

#include <vector>

#include <SDL2/SDL.h>

#include "Creature.hpp"
#include "Creature_spawner.hpp"
#include "Tile.hpp"
#include "Tile_palette.hpp"

class Level_map final {
public:
    Level_map(size_t w, size_t h);
    ~Level_map();

    friend class Level_master;

    Creature* get_creature(size_t x, size_t y);
    const Tile* get_tile(size_t x, size_t y);

    void set_creature_spawner(Creature_spawner* cs);
    void set_tile_palette(Tile_palette* tp);

    void add_creature(Creature* creature);
    void put_tile(size_t x, size_t y, const Tile* tile);
    //TODO decouple rendering from data ops via e.g. a viewport class
    void render();
    /* Initialises some rendering related data, should be called after at least
     * the tile at 0x0 is assigned, and before the first call to render().*/
    void render_init(SDL_Renderer* ren);

private:
    /*TODO db is no longer needed for creature and tile loaders, so shoudl prob
     * close */
    /*TODO think how to minimise future mistakes which may arise from forgetting
     * to call render_init() and to set tile and creature loaders.*/
    Tile_palette* tile_palette;
    Creature_spawner* creature_spawner;
    SDL_Renderer* ren;
    //TODO implement a simple Matrix?
    std::vector<std::vector<const Tile*>> tiles;
    std::vector<Creature*> creatures;
    int tile_w;
    int tile_h;

    // returns true if x/y is within bounds, false otherwise
    bool check_bounds(size_t x, size_t y);
};

#endif // define SRC_LEVEL_MAP_HPP_
