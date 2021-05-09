#ifndef __FUNCS_HPP__
#define __FUNCS_HPP__

#include "tank.hpp"
#include "map.hpp"
#include "bullets.hpp"
#include "exceptions.hpp"
#include <vector>
#include <string>
#include <fstream>

double get_distance(const Point& p1, const Point& p2);

void update_game_window(Window& game_window, Tank& tank_1, Tank& tank_2, All_Bullets& all_bullets);

void make_map(Map* map,const std::vector<std::string>& wall_positions);
void read_map(std::string dir, int& win_width, int& win_height, Map* map, Point& t_1, Point& t_2);

#endif