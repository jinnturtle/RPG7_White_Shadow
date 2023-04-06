#include "Tile.hpp"

Tile::Tile(const Tile_db_entry* data, SDL_Texture* tex)
:tex {tex}
,passable {data->passable}
{}

bool Tile::get_passable() const { return this->passable; }
SDL_Texture* Tile::get_tex() const { return this->tex; }

void Tile::render(SDL_Renderer* ren, SDL_Rect* rect) const
{
    SDL_RenderCopy(ren, this->tex, nullptr, rect);
}
