#include "Creature_db.hpp"

Creature_db::Creature_db()
:data {
    {"human", Creature_db_entry{
        .tex_path = "data/gfx/creature_human.png",
        .dmg = 1,
        .max_hp = 10}},
    {"gray_goo", Creature_db_entry{
        .tex_path = "data/gfx/creature_gray_goo.png",
        .dmg = 1,
        .max_hp = 3}}}
{}

const Creature_db_entry* Creature_db::find(const std::string& key) const
{
    auto result = this->data.find(key);
    if (result == this->data.end()) { return nullptr; }

    return &result->second;
}
