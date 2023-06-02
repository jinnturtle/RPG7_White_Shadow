#ifndef SRC_CREATURE_SPAWNER_HPP_
#define SRC_CREATURE_SPAWNER_HPP_

#include <string>
#include <map>

#include <SDL2/SDL.h>

#include "Creature.hpp"
#include "Creature_db.hpp"
#include "helpers.hpp"

/* Creature_spawner handles loading and initializing creature objects from
 * database configuration entries and provides a way to instantiate copies of
 * the initialized objects.
 *
 * I.e. First load the creatures you want from db, then spawn as many as you
 * like.*/
class Creature_spawner final {
public:
    Creature_spawner();
    ~Creature_spawner();

    int load(const Creature_db* db, const std::string& name, SDL_Renderer* ren);
    Creature* spawn(const std::string& name, const Vec2u* pos);

private:
    std::map<std::string, Creature> loaded;
    std::vector<SDL_Texture*> texs; // used for resource deallocation
};


#endif //define SRC_CREATURE_SPAWNER_HPP_
