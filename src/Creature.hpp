#ifndef SRC_CREATURE_HPP_
#define SRC_CREATURE_HPP_

#include "App_environment.hpp"
#include "helpers.hpp"

class Creature {
public:
    Creature(Vec2 pos) : pos {pos} {};
    virtual ~Creature() = default;

    virtual auto render(App_environment* app) -> void = 0;

protected:
    Vec2 pos;
};

class Creature_human: public Creature {
public:
    Creature_human(Vec2 pos) : Creature(pos) {};

    auto render(App_environment* app) -> void override;
};

#endif // define SRC_CREATURE_HPP_
