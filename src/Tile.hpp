#ifndef SRC_TILE_HPP_
#define SRC_TILE_HPP_

#include <SDL2/SDL.h>

#include "Tile_db.hpp"

class Tile final {
public:
    Tile(const Tile_db_entry* data, SDL_Texture* tex);
    ~Tile() = default;

    bool get_passable() const;
    SDL_Texture* get_tex() const;

    void render(SDL_Renderer* ren, SDL_Rect* rect) const;

private:
    SDL_Texture* tex;
    bool passable; // can normally be walked through/on?
};

#endif // SRC_TILE_HPP_
