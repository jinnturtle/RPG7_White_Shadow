/* The purpose of this program is to implement a working A* path-finding
 * algorithm, isolated from other program code.
 * The path-finding problem in this case is on a simple, two dimentional, node
 * grid where nodes may be passable or impassable and are otherwise weighed
 * equally in regards to eachother. */

#include <array>
#include <cmath>
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <vector>

struct Pt2 {
    int x;
    int y;
};

enum Node_type {
    NT_nil = 0,
    NT_passable,
    NT_impassable
};

struct Node_map {
    std::vector<Node_type> nodes;
    size_t w;
    size_t h;
};

/* nodes - the map of nodes to navigate through
 * max_ops - max number of nodes to explore before giving up (to prevent
 *     excessively long attempts at a solution)
 */
std::vector<int> find_path(
        const Pt2* start,
        const Pt2* end,
        const Node_map* nodes,
        size_t max_ops);

int generate_nodes(Node_map* node_map);

int main (/*int argc, char** argv*/)
{
    Node_map node_map;
    if(generate_nodes(&node_map)) {
        std::cout << "ERROR: While generating nodes" << std::endl;
        return 1;
    }

    constexpr Pt2 start {.x = 11, .y = 15};
    constexpr Pt2 end   {.x = 58, .y = 3};
    constexpr int max_steps {1000};
    std::vector<int> route = find_path(&start, &end, &node_map, max_steps);
    if(route.size() == 0)
    {
        std::cout << "INFO: Could not find path" << std::endl;
        return 1;
    }

    std::string map_gfx_buf;
    {
        std::stringstream map_gfx_ss;
        for (size_t i {0}; i < node_map.nodes.size(); ++i) {
            switch(node_map.nodes[i]) {
                case NT_passable:
                    map_gfx_ss << ' ';
                    break;

                case NT_impassable:
                    map_gfx_ss << '#';
                    break;
            }
        }

        map_gfx_buf = std::move(map_gfx_ss.str());
    }

    for (auto& node_id : route) {
        try {
            map_gfx_buf.at(node_id) = 'x';
        } catch (std::out_of_range) {
            std::cout << "ERROR: route attempted to draw out of bounds"
            << std::endl;
        }
    }

    map_gfx_buf[start.x + start.y * node_map.w] = 'S';
    map_gfx_buf[end.x + end.y * node_map.w] = 'E';

    for (size_t i {0}; i < map_gfx_buf.size(); ++i) {
        std::cout << map_gfx_buf[i];
        if (i > 0 && (i+1) % node_map.w == 0) { std::cout << std::endl; }
    }
    std::cout << std::endl;

    return 0;
}

/* returns
 *     on success: a vector of nodemap node ids for the route
 *     on failure: an empty vector */
std::vector<int> find_path(
        const Pt2* start,
        const Pt2* end,
        const Node_map* nodemap,
        size_t max_ops)
{
    struct Route_node {
        std::list<Route_node> neighbours;
        Pt2 pos;
        int dist;
        int id;
    };

    std::vector<int> result;

    // open start node
    // evaluate all unevaluated neighbours
    // open cheapest neighbour
    // if all neighbours closed or invalid, close this node
    std::list<Route_node> route;
    std::set<int> closed;

    // open first node
    route.push_back(Route_node{.pos = *start});
    route.back().id = route.back().pos.x + (route.back().pos.y * nodemap->w);
    // close node if not passable
    if (
        nodemap->nodes[route.back().id]
        != NT_passable)
    {
        closed.insert(route.back().id);
        route.pop_back();
        return std::move(result);
    }

    // calculate distance to target from our first open node
    route.back().dist =
        std::abs(std::abs(end->x) - std::abs(route.back().pos.x)) +
        std::abs(std::abs(end->y) - std::abs(route.back().pos.y));

    Route_node* current_node {nullptr};
    size_t ops {1};
    for ( ; ops < max_ops && !route.empty(); ++ops)
    {
        current_node = &route.back();

        // are we there yet?
        if (current_node->pos.x == end->x && current_node->pos.y == end->y) {
            break;
        }

        // pick next node from open neighbours if any are open
        if (current_node->neighbours.empty() == false) {
            route.push_back(current_node->neighbours.front());
            continue;
        } else {
            // find neighbours and sort by distance to goal (less is better)
            std::array<int, 4> neighbour_ids {
                // neighbour up
                current_node->pos.x + ((current_node->pos.y - 1) * nodemap->w),
                // neighbour down
                current_node->pos.x + ((current_node->pos.y + 1) * nodemap->w),
                // neighbour left
                current_node->pos.x - 1 + (current_node->pos.y * nodemap->w),
                // neighbour right
                current_node->pos.x + 1 + (current_node->pos.y * nodemap->w),
            };

            std::cout << "looking for neighbours of "
            << current_node->pos.x << 'x' << current_node->pos.y << std::endl;
            for (auto& neighbour_id : neighbour_ids) {
                // check if this neighbour has not been closed already
                if (closed.find(neighbour_id) != closed.end()) {
                    std::cout << "found already closed neighbour: "
                    << neighbour_id << std::endl;
                    std::cout << "closed set size: " << closed.size()
                    << std::endl;
                    continue;
                }

                // check if this neighbour is not on the route already
                {
                    /* TODO actually find an efficient way to verify if we're
                     * not intersecting ourselves */
                    bool found {false};
                    for(auto& route_node : route) {
                        if (route_node.id == neighbour_id) {
                            found = true;
                        }
                    }
                    if (found) { continue; }
                }

                const Node_type* neighbour {nullptr};
                try {
                    neighbour = &nodemap->nodes.at(neighbour_id);
                } catch (std::out_of_range) {
                    // ok, no neighbour that way, moving along
                }
                if (neighbour != nullptr) {
                    if (*neighbour != NT_impassable) {
                        Route_node new_neighbour {
                            .pos = {.x = neighbour_id % 60, .y = neighbour_id / 60},
                            .id = neighbour_id
                        };

                        // calculate distance to target
                        new_neighbour.dist = 
                            std::abs(std::abs(end->x) - std::abs(new_neighbour.pos.x)) +
                            std::abs(std::abs(end->y) - std::abs(new_neighbour.pos.y));

                        current_node->neighbours.push_back(std::move(new_neighbour));

                        std::cout << "found neighbour at "
                        << current_node->neighbours.back().pos.x << 'x'
                        << current_node->neighbours.back().pos.y
                        << std::endl;
                    }
                }
            }

            current_node->neighbours.sort(
                [](const Route_node& a, const Route_node& b) -> bool {
                    return a.dist < b.dist;
                });
        }

        // traverse up the route until we can find an open neighbour
        while (current_node->neighbours.empty()) {
            auto ins_res = closed.insert(current_node->id);
            if (ins_res.second == false) {
                std::cout << "DEBUG: tried to close already closed node "
                << current_node->id << std::endl;
            }

            route.pop_back();
            if (route.empty() == false) {
                current_node = &route.back();
                continue;
            } else {
                break;
            }
        }

        // TODO optimisation: don't check previous node when checking neighbours
    }

    // wrapping up

    result.reserve(route.size());
    for (auto& route_node : route) {
        result.push_back(route_node.id);
    }

    if (ops == max_ops) {
        // TODO should handle this via return parameter or maybe throw exception
        std::cout << "INFO: ops limit reached" << std::endl;
    }

    return std::move(result);
}

int generate_nodes(Node_map* node_map) {
    constexpr size_t w {60};
    constexpr size_t h {20};
    node_map->w = w;
    node_map->h = h;
    node_map->nodes = std::vector<Node_type>(node_map->w * node_map->h, NT_nil);

    std::array<char, 1 + w * h> node_data {
        "############################################################"
        "#                                                          #"
        "#       #                                     #            #"
        "#       #                                     #            #"
        "#       #                                     #            #"
        "#       #                                     ####         #"
        "#       #                                                  #"
        "#       #                                                  #"
        "#       #     #################################            #"
        "#       #                                     #            #"
        "#       #                                     #            #"
        "#       #                                     #            #"
        "#       #                                     #            #"
        "#       ##### ###                             #            #"
        "#       #       #                             #            #"
        "#       #       #                             #            #"
        "#       #       #                             #            #"
        "#       #       #                             #            #"
        "#       #########                             #            #"
        "############################################################"};
    // -1 as we don't need the null terminator
    for (size_t i {0}; i < node_data.size() -1; ++i) {
        switch (node_data[i]) {
            case ' ':
                node_map->nodes[i] = NT_passable;
                break;

            case '#':
                node_map->nodes[i] = NT_impassable;
                break;

            default:
                std::cout << "ERROR: invalid node data at pos " << i
                << std::endl;
                return 1;
                break;
        }
    }

    return 0;
}
