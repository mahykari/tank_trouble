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

void Map::set_width_and_height(int w, int h) 
{
    width = w;
    height = h;
}

Point Map::get_dimensions()
{
    Point p(width, height);
    return p;
}