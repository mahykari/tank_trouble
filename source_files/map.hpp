#ifndef __MAP_HPP__
#define __MAP_HPP__
#include "rsdl.hpp"
#include <vector>
#include <string>

struct Wall
{
    Point src, dest;
    Wall(Point _src,Point _dest) {
        src = _src;
        dest = _dest;
    }
};

class Map
{
    private:
        int width;
        int height;
        std::vector<Wall> walls;
    public:
        Map();
        void set_width_and_height(int w, int h);
        Point get_dimensions();
        std::vector<Wall> get_walls() const;
        void add_to_walls(Wall w);
};

#endif