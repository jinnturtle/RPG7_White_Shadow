#include "Level_map.hpp"

Level_map::Level_map(size_t w, size_t h, size_t tile_w, size_t tile_h)
: tile_w {tile_w}
, tile_h {tile_h}
{
    DBG(7, "Allocating map size ", w, "x", h);

    this->tiles = new std::vector<std::vector<Tile*>*>(w, nullptr);
    for (size_t i_w {0}; i_w < w; ++i_w) {
        this->tiles->at(i_w) = new std::vector<Tile*>(h, nullptr);
    }
}

Level_map::~Level_map()
{
    DBG(7, "Deleting map tiles");
    for (size_t x {0}; x < this->tiles->size(); ++x) {
        for (size_t y {0}; y < this->tiles->at(x)->size(); --y) {
            DBG(8, "Deleting tile: ", x, "x", y);
            delete this->tiles->at(x)->at(y);
        }
        DBG(8, "Deleting tilemap column: ", x);
        delete this->tiles->at(x);
    }
    DBG(8, "Deleting tilemap");
    delete this->tiles;

    DBG(8, "Deleting creatures");
    for (auto& creature : this->creatures) {
        delete creature;
    }
}

auto Level_map::get_creature(size_t x, size_t y) -> Creature*
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

auto Level_map::get_tile(size_t x, size_t y) -> const Tile*
{
    return this->tiles->at(x)->at(y);
}

auto Level_map::add_creature(Creature* creature) -> void
{
    this->creatures.push_back(creature);
}

auto Level_map::put_tile(size_t x, size_t y, Tile* tile) -> void
{
    DBG(8, "Placing tile at ", x, "x", y);
    if (this->check_bounds(x, y) != true) {
        DBG(0, "Attempted to place tile out of bounds: ", x, "/", y);
        return;
    }

    if (this->tiles->at(x)->at(y) != nullptr) {
        DBG(0, "Tile not null for put at ", x, "x", y);
    } else {
        this->tiles->at(x)->at(y) = tile;
    }
}

auto Level_map::render(App_environment* app) -> void
{
    SDL_Rect render_rect {
        0, 0, static_cast<int>(this->tile_w), static_cast<int>(this->tile_h)};

    for (size_t x {0}; x < this->tiles->size(); ++x) {
        render_rect.x = x * this->tile_w;
        for (size_t y {0}; y < this->tiles->at(x)->size(); ++y) {
            render_rect.y = y * this->tile_h;
            this->tiles->at(x)->at(y)->render(app, &render_rect);
        }
    }

    for (auto& creature : this->creatures) { creature->render(app); }
}

// private ---------------------------------------------------------------------
auto Level_map::check_bounds(size_t x, size_t y) -> bool
{
    if (this->tiles->size() < x+1 || this->tiles->at(x)->size() < y+1) {
        return false;
    }

    return true;
}
