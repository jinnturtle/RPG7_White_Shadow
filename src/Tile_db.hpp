#ifndef SRC_TILE_DB_HPP_
#define SRC_TILE_DB_HPP_

#include <map>
#include <string>

struct Tile_db_entry {
    std::string tex_path;
    bool passable;
};

class Tile_db final {
public:
    Tile_db();
    ~Tile_db() = default;

    const Tile_db_entry* find(const std::string& key) const;

private:
    std::map<const std::string, const Tile_db_entry> data;
};

#endif //define SRC_TILE_DB_HPP_
