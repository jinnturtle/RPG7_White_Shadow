#ifndef SRC_TILE_HPP_
#define SRC_TILE_HPP_

#include <SDL2/SDL.h>

#include "App_environment.hpp"

class Tile {
public:
    //TODO Maybe we can avoid using this type differentiation technique?
    //enum Type {
    //    TYPE_wall = 0,
    //    TYPE_floor
    //};
    //
    //virtual auto get_type() -> Type = 0;

    virtual auto is_passable() const -> bool = 0;
    virtual auto render(App_environment* app, SDL_Rect* rect) -> void = 0;

    virtual ~Tile() {};
};

class Tile_floor final: public Tile {
public:
    auto is_passable() const -> bool override;
    auto render(App_environment* app, SDL_Rect* rect) -> void override;

    virtual ~Tile_floor() override = default;
};

class Tile_wall final: public Tile {
public:
    auto is_passable() const -> bool override;
    auto render(App_environment* app, SDL_Rect* rect) -> void override;

    virtual ~Tile_wall() override = default;
};

#endif // SRC_TILE_HPP_
