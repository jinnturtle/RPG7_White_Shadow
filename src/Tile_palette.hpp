#ifndef SRC_TILE_PALETTE_HPP_
#define SRC_TILE_PALETTE_HPP_

#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "Tile.hpp"
#include "Tile_db.hpp"

/* Tile_palette handles loading and initializing tile objects from
 * database configuration entries and provides a way to instantiate copies of
 * the initialized objects.
 *
 * I.e. First load the tiles you want from db, then instantiate as many as you
 * like
 *
 * The name Tile_palette was chosen as an alternative to Tile_set as the
 * palette can conceptually contain more than one thematic tile set or just a
 * part of a tile set, even if it is intended to hold the set of tiles for the
 * level.*/

class Tile_palette final
{
public:
    Tile_palette(const Tile_db* db);
    ~Tile_palette();

    int load(const std::string& name, SDL_Renderer* ren);
    // provide a reference to a loaded tile
    const Tile* make_ref(const std::string& name);

private:
    std::map<std::string, Tile> loaded;
    std::vector<SDL_Texture*> texs; // used for resource deallocation
    const Tile_db* db;
};

#endif //define SRC_TILE_PALETTE_HPP_
