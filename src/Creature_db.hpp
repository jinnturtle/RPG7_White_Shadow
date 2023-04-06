/* This is a mockup of a database that stores creature information entries,
 * should be replaced with a database access wrapper once we have the respective
 * database.*/

#ifndef SRC_CREATURE_DB_HPP_
#define SRC_CREATURE_DB_HPP_

#include <map>
#include <string>

struct Creature_db_entry {
    std::string tex_path;
    int dmg;
    int max_hp;
};

class Creature_db final {
public:
    Creature_db();

    // returns data entry under key, or nullptr if entry is not found
    const Creature_db_entry* find(std::string key);

private:
    //TODO prob should define const string, const entry to disambiguate intent
    std::map<std::string, Creature_db_entry> data;
};

#endif //define SRC_CREATURE_DB_HPP_
