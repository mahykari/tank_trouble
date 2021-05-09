#include "map.hpp"
#include <vector>
using namespace std;

vector<Wall> Map::get_walls() const
{
    return walls;
}

void Map::add_to_walls(Wall w)
{
    walls.push_back(w);
}

Map::Map(Window* win)
{
    game_window = win;
}

Map::Map() {}

void Map::set_window(Window* win)
{
    game_window = win;
}

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

void Map::unify_walls() {
    vector<Wall> horizontal, vertical;
    for(int i = 0; i < walls.size(); i++) {
        Wall w = walls[i];
        if(w.src.x == w.dest.x)
            vertical.push_back(w);
        else if(w.src.y == w.dest.y)
            horizontal.push_back(w);
    }
    walls.clear();

    for(int i = 0; i < horizontal.size() - 1; i++) {
        for(int j = i + 1; j < horizontal.size(); j++) {
            if(horizontal[i].dest.x == horizontal[j].src.x && horizontal[i].dest.y == horizontal[j].src.y) {
                horizontal[i].dest.x = horizontal[j].dest.x;
                horizontal.erase(horizontal.begin() + j);
                j -= 1;
                i -= 1;
            }
        }
    }

    for(int i = 0; i < vertical.size() - 1; i++) {
        for(int j = i + 1; j < vertical.size(); j++) {
            if(vertical[i].dest.x == vertical[j].src.x && vertical[i].dest.y == vertical[j].src.y) {
                vertical[i].dest.y = vertical[j].dest.y;
                vertical.erase(vertical.begin() + j);
                j -= 1;
                i -= 1;
            }
        }
    }

    for(int i = 0; i < max(horizontal.size(), vertical.size()); i++) {
        if(i < horizontal.size()) walls.push_back(horizontal[i]);
        if(i < vertical.size()) walls.push_back(vertical[i]);
    }

    for(int i = 0; i < walls.size(); i++) {
        cout << "Wall " << i + 1 << ": "  << walls[i].src << walls[i].dest << endl;
    }
}

void Map::draw()
{
    for(int i = 0; i < walls.size(); i++) {
        game_window->draw_line(walls[i].src, walls[i].dest, WHITE);
    }
}