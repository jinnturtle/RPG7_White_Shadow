#include "Tile_db.hpp"

Tile_db::Tile_db()
:data {
    {"floor", Tile_db_entry{
        .tex_path = "data/gfx/tile_floor.png",
        .passable = true}},
    {"wall", Tile_db_entry{
        .tex_path = "data/gfx/tile_wall.png",
        .passable = false}}}
{}

const Tile_db_entry* Tile_db::find(const std::string& key) const
{
    auto result = this->data.find(key);
    if (result == this->data.end()) { return nullptr; }

    return &result->second;
}
