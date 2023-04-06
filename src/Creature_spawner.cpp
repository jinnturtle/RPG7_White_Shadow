#include "Creature_spawner.hpp"

#include "logs.hpp"

Creature_spawner::Creature_spawner(Creature_db* db)
:db {db}
{}

Creature_spawner::~Creature_spawner()
{
    for (auto& tex : this->texs) {
        SDL_DestroyTexture(tex);
        if (SDL_GetError()[0] != '\0') {
            logs::errt(logs::ERR_sdl, "during texture cleanup");
        }
    }
}

int Creature_spawner::load(const std::string& name, SDL_Renderer* ren)
{
    const Creature_db_entry* found = this->db->find(name);
    if (found == nullptr) {
        logs::err("creature '", name,  "' not found in DB");
        return 1;
    }

    SDL_Texture* tex = load_texture(found->tex_path, ren);
    if (tex == nullptr) {
        logs::err("failed to load texture for '", name, "'");
        return 1;
    }

    this->texs.push_back(tex); // will use the pointer to free memory later
    Creature creature(name, found, tex);
    this->loaded.emplace(name, creature);

    return 0;
}

Creature* Creature_spawner::spawn(const std::string& name, const Vec2u* pos)
{
    auto iter = this->loaded.find(name);
    if (iter == this->loaded.end()) {
        logs::err("spawn request for creature that's not loaded: '", name, "'");
        return nullptr;
    }

    Creature* creature = new Creature{iter->second};
    creature->set_pos(pos);

    return creature;
}
