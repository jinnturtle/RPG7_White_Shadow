#include "Level_map.hpp"

#include "logs.hpp"

Level_map::Level_map(size_t w, size_t h)
: tile_palette {nullptr}
, creature_spawner {nullptr}
, ren {nullptr}
{
    DBG(7, "Allocating map size ", w, "x", h);
    this->tiles.resize(w);

    for (size_t i_w {0}; i_w < w; ++i_w) {
        this->tiles.at(i_w).resize(h);
    }
}

Level_map::~Level_map()
{
    DBG(8, "Deleting level creatures");
    for (auto& creature : this->creatures) {
        delete creature;
    }

    delete this->tile_palette;
    delete this->creature_spawner;
}

Creature* Level_map::get_creature(size_t x, size_t y)
{
    for (auto& creature : this->creatures) {
        if (static_cast<size_t>(creature->get_pos()->x) == x
            && static_cast<size_t>(creature->get_pos()->y) == y
        ) {
            return creature;
        }
    }

    return nullptr;
}

const Tile* Level_map::get_tile(size_t x, size_t y)
{
    return this->tiles.at(x).at(y);
}

void Level_map::set_creature_spawner(Creature_spawner* cs)
{
    this->creature_spawner = cs;
}

void Level_map::set_tile_palette(Tile_palette* tp)
{
    this->tile_palette = tp;
}

void Level_map::add_creature(Creature* creature)
{
    this->creatures.push_back(creature);
}

void Level_map::put_tile(size_t x, size_t y, const Tile* tile)
{
    DBG(8, "Placing tile at ", x, "x", y);
    if (this->check_bounds(x, y) != true) {
        DBG(0, "Attempted to place tile out of bounds: ", x, "/", y);
        return;
    }

    if (this->tiles.at(x).at(y) != nullptr) {
        DBG(0, "Tile not null for put at ", x, "x", y);
    } else {
        this->tiles.at(x).at(y) = tile;
    }
}

void Level_map::render()
{
    SDL_Rect render_rect {0, 0, this->tile_w, this->tile_h};

    for (size_t x {0}; x < this->tiles.size(); ++x) {
        render_rect.x = x * this->tile_w;
        for (size_t y {0}; y < this->tiles.at(x).size(); ++y) {
            render_rect.y = y * this->tile_h;
            this->tiles.at(x).at(y)->render(this->ren, &render_rect);
        }
    }

    for (auto& creature : this->creatures) { creature->render(ren); }
}

void Level_map::render_init(SDL_Renderer* ren)
{
    this->ren = ren;
    const Tile* tile {this->get_tile(0,0)};
    if (tile == nullptr) {
        logs::err("can not initialize rendering of level map ",
            "while the first tile is null");
    }

    SDL_QueryTexture(tile->get_tex(), nullptr, nullptr,
        &this->tile_w,
        &this->tile_h);
}

// private ---------------------------------------------------------------------
bool Level_map::check_bounds(size_t x, size_t y)
{
    if (this->tiles.size() < x+1 || this->tiles.at(x).size() < y+1) {
        return false;
    }

    return true;
}
