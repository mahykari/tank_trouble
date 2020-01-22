#pragma once
#include "tank.hpp"
#include "map.hpp"
#include "bullets.hpp"
#include <vector>
#include <string>
#include <fstream>

class quit_game {};
class collision {};

double get_distance(Point& p1, Point& p2);
void check_tank_bullet_collision(Tank& t, All_Bullets& all_bullets);
bool check_tank_wall_collision(const Tank& tank, const Map& map);
bool check_tank_tank_collision(const Tank& t_1, const Tank& t_2);

void update_game_window(Window& game_window, Tank& tank_1, Tank& tank_2, All_Bullets& all_bullets);

void make_map(Map& map,const std::vector<std::string>& wall_positions);
void read_map(std::string dir, int& win_width, int& win_height, Map& map, Point& t_1, Point& t_2);

void draw_walls(Window& game_window, Map& map);
void draw_tank(Window& game_window, Tank& tank, Tank& second_tank, Map& map);
void draw_bullet(Window& game_window,Bullet& b);
void draw_all_bullets(Window& game_window, All_Bullets& all_bullets);