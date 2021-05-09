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
        Window* game_window;
    public:
        Map(Window* win);
        Map();
        Point get_dimensions();
        std::vector<Wall> get_walls() const;
        void set_width_and_height(int w, int h);
        void add_to_walls(Wall w);
        void unify_walls();
        void set_window(Window* win);
        void draw();
};

#endif