#include "funcs.hpp"
#include "const_values.hpp"

using namespace std;

void draw_walls(Window& game_window, Map* map)
{
    vector<Wall> walls = map->get_walls();
    for(int i = 0; i < walls.size(); i++) {
        game_window.draw_line(walls[i].src, walls[i].dest, WHITE);
    }
}

void draw_tank(Window& game_window, Tank& tank, Tank& second_tank, Map* map)
{
    int direction = tank.get_rotation_state();
    tank.set_rotation(direction * ANGULAR_VELOCITY);
    tank.move();
    if(check_tank_wall_collision(tank, map)) tank.move_back();
    else if(check_tank_tank_collision(tank, second_tank)) tank.move_back();
    Point p = tank.get_position();
    game_window.draw_img(tank.get_filename(), Rectangle(p - Point(HALF_TANK_SIZE, HALF_TANK_SIZE), TANK_SIZE, TANK_SIZE), NULL_RECT, tank.get_rotation());
}