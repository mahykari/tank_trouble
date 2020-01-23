#include "funcs.hpp"
#include "const_values.hpp"
using namespace std;

bool check_tank_tank_collision(const Tank& t_1, const Tank& t_2) {
    Point t_p_1 = t_1.get_position();
    Point t_p_2 = t_2.get_position();
    double distance = get_distance(t_p_1, t_p_2);
    if(distance < TANK_SIZE) return true;
    else return false; 
}

bool check_tank_wall_collision(const Tank& tank, const Map* map) {
    vector<Wall> walls = map->get_walls();
    Point tank_position = tank.get_position();
    for(int i = 0; i < walls.size(); i++) {
        Wall w = walls[i];
        if(abs(tank_position.x - w.src.x) <= HALF_TANK_SIZE && tank_position.y >= w.src.y - HALF_TANK_SIZE && tank_position.y <= w.dest.y + HALF_TANK_SIZE) {
            return true;
        } 
        else if(abs(tank_position.y - w.src.y) <= HALF_TANK_SIZE && tank_position.x >= w.src.x - HALF_TANK_SIZE && tank_position.x <= w.dest.x + HALF_TANK_SIZE) {
            return true;
        }
    }
    return false;

}

void check_tank_bullet_collision(Tank& tank, All_Bullets& all_bullets) {
    const Point t_p = tank.get_position();
    const vector<vector<Bullet*> > b = all_bullets.get_tank_bullets();
    for(int i = 0; i < b.size(); i++) {
        for(int j = 0; j < b[i].size(); j++) {
            const Point b_p = b[i][j]->position;
            if(get_distance(t_p, b_p) < HALF_TANK_SIZE) {
                if(tank.get_status() != "Shield") throw fatal_collision(tank.get_id());
                else {
                    b[i][j]->shield_collision = true;
                }
            }
        }
    }
}