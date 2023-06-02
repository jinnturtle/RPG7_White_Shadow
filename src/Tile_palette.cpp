#include "Tile_palette.hpp"

#include "helpers.hpp"
#include "logs.hpp"

Tile_palette::Tile_palette()
{}

Tile_palette::~Tile_palette()
{
    for (auto& tex : this->texs) {
        SDL_DestroyTexture(tex);
        if (SDL_GetError()[0] != '\0') {
            logs::errt(logs::ERR_sdl, "during texture cleanup");
        }
    }
}

int Tile_palette::load(
        const Tile_db* db,
        const std::string& name,
        SDL_Renderer* ren)
{
    if (db == nullptr) {
        logs::err("null tile db provided to load from");
        return 1;
    }

    const Tile_db_entry* found = db->find(name);
    if (found == nullptr) {
        logs::err("tile '", name,  "' not found in DB");
        return 1;
    }

    SDL_Texture* tex = load_texture(found->tex_path, ren);
    if (tex == nullptr) {
        logs::err("failed to load texture for '", name, "'");
        return 1;
    }

    this->texs.push_back(tex); // will use the pointer to free memory later
    Tile tile(found, tex);
    this->loaded.emplace(name, std::move(tile));

    return 0;
}

const Tile* Tile_palette::make_ref(const std::string& name)
{
    auto iter = this->loaded.find(name);
    if (iter == this->loaded.end()) {
        logs::err("request for tile that's not loaded: '", name, "'");
        return nullptr;
    }

    return &iter->second;
}
