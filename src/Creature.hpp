#ifndef SRC_CREATURE_HPP_
#define SRC_CREATURE_HPP_

#include <map>
#include <vector>
#include <string>

#include <SDL2/SDL.h>

#include "Creature_db.hpp"
#include "helpers.hpp"

enum Creature_control_command {
    CCC_noop = 0,
    CCC_move_up,
    CCC_move_down,
    CCC_move_left,
    CCC_move_right
};

class Creature final {
public:
    Creature(const std::string& name,
            const Creature_db_entry* data,
            SDL_Texture* tex);
    ~Creature() = default;

    int get_hp();
    bool get_human_control();
    int get_max_hp();
    std::string get_name();
    const Vec2u* get_pos();

    void set_pos(const Vec2u* pos);
    void set_human_control(bool val);

    void move(Creature_control_command cmd);
    void render(SDL_Renderer* ren);

private:
    std::string name;
    Vec2u pos;
    SDL_Texture* tex;
    int dmg;
    int max_hp, hp;
    bool human_control;
};

#endif // define SRC_CREATURE_HPP_
