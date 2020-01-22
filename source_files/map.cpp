#include "map.hpp"
#include <vector>

std::vector<Wall> Map::get_walls() const
{
    return walls;
}

void Map::add_to_walls(Wall w)
{
    walls.push_back(w);
}

Map::Map() {}