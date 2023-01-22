#ifndef SRC_CREATURE_HPP_
#define SRC_CREATURE_HPP_

#include "App_environment.hpp"
#include "helpers.hpp"

enum Creature_control_command {
    CCC_noop = 0,
    CCC_move_up,
    CCC_move_down,
    CCC_move_left,
    CCC_move_right
};

class Creature {
public:
    Creature(Vec2u pos, bool human_control);
    virtual ~Creature() = default;

    auto get_pos() -> const Vec2u*;
    auto is_human_controlled() -> bool;

    auto move(Creature_control_command cmd) -> void;
    virtual auto render(App_environment* app) -> void = 0;

private:
    Vec2u pos;
    bool human_control;
};

class Creature_human: public Creature {
public:
    Creature_human(Vec2u pos, bool human_control = false);

    auto render(App_environment* app) -> void override;
};

#endif // define SRC_CREATURE_HPP_
