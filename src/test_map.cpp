#include "test_map.hpp"

#include "Creature.hpp"
#include "Creature_db.hpp"
#include "Creature_spawner.hpp"
#include "Tile.hpp"
#include "Tile_db.hpp"
#include "Tile_palette.hpp"
#include "logs.hpp"

#include <array>

Level_map* test_map::generate(App_environment* app)
{
    enum {
        Tile_id_floor = 0,
        Tile_id_wall};

    enum {
        Mob_id_human = 0,
        Mob_id_gray_goo};

    std::array<std::pair<std::string, const Tile*>, 2> tile_ref_table;
    tile_ref_table[Tile_id_floor] = {"floor", nullptr};
    tile_ref_table[Tile_id_wall]  = {"wall", nullptr};
    Tile_db tile_db;
    Tile_palette* tile_palette = new Tile_palette();
    for (std::pair<std::string, const Tile*>& ref : tile_ref_table) {
        if (tile_palette->load(&tile_db, ref.first, app->ren)) {
            logs::err("while loading tile data");
            return nullptr;
        }

        ref.second = tile_palette->make_ref(ref.first);
    }

    std::array<std::string, 2> mob_spawn_table;
    mob_spawn_table[Mob_id_human]    = "human";
    mob_spawn_table[Mob_id_gray_goo] = "gray_goo";
    Creature_db creature_db;
    Creature_spawner* creature_spawner = new Creature_spawner();
    for (const std::string& name : mob_spawn_table) {
        if (creature_spawner->load(&creature_db, name, app->ren)) {
            logs::err("while loading creature data");
            return nullptr;
        }
    }

    constexpr size_t map_w {40};
    constexpr size_t map_h {20};
    //  - floor
    //# - wall
    //@ - player
    //h - human creature
    //g - gray goo creature
    std::array<char, map_w * map_h> blueprint {
        "########################################"
        "#                                      #"
        "#                                      #"
        "#  #####                               #"
        "#  # h #  @    g                       #"
        "#  #   #                               #"
        "#  #   #                               #"
        "#  ## ###                              #"
        "#      #                               #"
        "#      #                               #"
        "#                                      #"
        "#                                      #"
        "#                                      #"
        "#                                      #"
        "#                                      #"
        "#                                      #"
        "#                                      #"
        "#                                      #"
        "#                                      #"
        "#######################################"};

    Level_map* level = new Level_map(map_w, map_h);
    level->set_tile_palette(tile_palette);
    level->set_creature_spawner(creature_spawner);
    for (unsigned i {0}; i < blueprint.size(); ++i) {
        unsigned x = i % map_w;
        unsigned y = i / map_w;
        switch(blueprint[i]) {
            case '\0':
                // because an initializer string is by default null-terminated
            case '#':
                level->put_tile(x, y, tile_ref_table[Tile_id_wall].second);
                break;

            case ' ':
                level->put_tile(x, y, tile_ref_table[Tile_id_floor].second);
                break;

            case 'h':
                level->put_tile(x, y, tile_ref_table[Tile_id_floor].second);
                {
                Vec2u spawn_pos{.x = x, .y = y};
                level->add_creature(
                    creature_spawner->spawn(mob_spawn_table[Mob_id_human],
                    &spawn_pos));
                }
                break;

            case 'g':
                level->put_tile(x, y, tile_ref_table[Tile_id_floor].second);
                {
                Vec2u spawn_pos{.x = x, .y = y};
                level->add_creature(
                    creature_spawner->spawn(mob_spawn_table[Mob_id_gray_goo],
                    &spawn_pos));
                }
                break;

            case '@':
                level->put_tile(x, y, tile_ref_table[Tile_id_floor].second);
                {
                Vec2u spawn_pos{.x = x, .y = y};
                Creature* player {
                    creature_spawner->spawn(mob_spawn_table[Mob_id_human],
                    &spawn_pos)};
                player->set_human_control(true);
                level->add_creature(std::move(player));
                }
                break;

            default:
                logs::err("unhandled blueprint tile at ", x, "x", y);
                break;
        }
    }
    level->render_init(app->ren);

    return level;
}
