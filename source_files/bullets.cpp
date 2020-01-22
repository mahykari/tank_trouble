#include "rsdl.hpp"
#include "bullets.hpp"
#include "const_values.hpp"
#include <cmath>
#include <vector>
#include <iostream>

All_Bullets::All_Bullets(int tank_count) {
    tank_bullets = std::vector<std::vector<Bullet> >(tank_count);
}

Bullet::Bullet(Point _position, int _v, double _rotation)
{
    position = _position;
    v_x = (double)_v * cos(_rotation * PI / 180);
    v_y = (double)_v * sin(_rotation * PI / 180);
}

void All_Bullets::add_to_tank_bullets(int tank_id, Point _position, double _rotation) 
{
    if(tank_bullets[tank_id - 1].size() == 5) return;
    _position += Point((HALF_TANK_SIZE + 5) * cos(_rotation * PI / 180), (HALF_TANK_SIZE + 5) * sin(_rotation * PI / 180));
    Bullet b(_position, BULLET_VELOCITY, _rotation);
    tank_bullets[tank_id - 1].push_back(b);
}

std::vector<std::vector<Bullet> > All_Bullets::get_tank_bullets() 
{
    return tank_bullets;
}

void All_Bullets::move_and_check_bullet_wall_collision(Bullet& b,const Map& map)
{
    if(b.lifetime == 0) return;
    b.position += Point(b.v_x, b.v_y);
    b.lifetime -= 25;
    std::vector<Wall> walls = map.get_walls();
    for(int i = 0; i < walls.size(); i++) {
        Wall w = walls[i];
        if((abs(b.position.x - w.src.x) <= HALF_BULLET_SIZE) && b.position.y >= w.src.y - HALF_BULLET_SIZE && b.position.y <= w.dest.y + HALF_BULLET_SIZE) {
            b.position -= Point(b.v_x, b.v_y);
            b.v_x = -b.v_x;  
            return;
        }

        if((abs(b.position.y - w.src.y) <= HALF_BULLET_SIZE) && b.position.x >= w.src.x - HALF_BULLET_SIZE && b.position.x <= w.dest.x + HALF_BULLET_SIZE) {
            b.position -= Point(b.v_x, b.v_y);
            b.v_y = -b.v_y;
            return;
        }

        if((abs(b.position.x - w.src.x) <= BULLET_VELOCITY) && b.position.y >= w.src.y - b.v_y && b.position.y <= w.dest.y + b.v_y) {
            b.position -= Point(b.v_x, b.v_y);
            b.v_x = -b.v_x;
            return;
        }

        if((abs(b.position.y - w.src.y) <= BULLET_VELOCITY) && b.position.x >= w.src.x - b.v_x && b.position.x <= w.dest.x + b.v_x) {
            b.position -= Point(b.v_x, b.v_y);
            b.v_y = -b.v_y;
            return;
        }
    }
}

void All_Bullets::check_all_bullets(Map& map)
{
    for(int i = 0; i < tank_bullets.size(); i++) {
        for(int j = 0; j < tank_bullets[i].size(); j++)
        move_and_check_bullet_wall_collision(tank_bullets[i][j], map);
    }
}