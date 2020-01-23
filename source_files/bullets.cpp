#include "rsdl.hpp"
#include "bullets.hpp"
#include "const_values.hpp"
#include <cmath>
#include <vector>
#include <iostream>

All_Bullets::All_Bullets(int tank_count, Map* _map) {
    tank_bullets = std::vector<std::vector<Bullet> >(tank_count);
    map = _map;
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
    _position += Point((HALF_TANK_SIZE) * cos(_rotation * PI / 180), (HALF_TANK_SIZE) * sin(_rotation * PI / 180));
    Bullet b(_position, BULLET_VELOCITY, _rotation);
    check_bullet_wall_collision(b);
    tank_bullets[tank_id - 1].push_back(b);
}

std::vector<std::vector<Bullet> > All_Bullets::get_tank_bullets() 
{
    return tank_bullets;
}

void All_Bullets::check_bullet_wall_collision(Bullet& b) {
    int MAX = std::max(HALF_BULLET_SIZE, BULLET_VELOCITY);
    std::vector<Wall> walls = map->get_walls();
    for(int i = 0; i < walls.size(); i++) {
        Wall w = walls[i];
        if(b.position.x - w.src.x < MAX && b.position.x - w.src.x > 0 && b.position.y >= w.src.y - HALF_BULLET_SIZE && b.position.y <= w.dest.y + HALF_BULLET_SIZE) {
            b.position.x = w.src.x + MAX;
            b.v_x = -b.v_x;
        }
        
        if(w.src.x - b.position.x < MAX && w.src.x - b.position.x > 0 && b.position.y >= w.src.y - HALF_BULLET_SIZE && b.position.y <= w.dest.y + HALF_BULLET_SIZE) {
            b.position.x = w.src.x - MAX;
            b.v_x = -b.v_x;
        }

        if(b.position.y - w.src.y < MAX && b.position.y - w.src.y > 0 && b.position.x >= w.src.x - HALF_BULLET_SIZE && b.position.x <= w.dest.x + HALF_BULLET_SIZE) {
            b.position.y = w.src.y + MAX;
            b.v_y = -b.v_y;
        }

        if(w.src.y - b.position.y <= MAX && w.src.y - b.position.y > 0 && b.position.x >= w.src.x - HALF_BULLET_SIZE && b.position.x <= w.dest.x + HALF_BULLET_SIZE) {
            b.position.y = w.src.y - MAX;
            b.v_y = -b.v_y;
        }
    }
}

void All_Bullets::move(Bullet& b) {
    b.position += Point(b.v_x, b.v_y);
    b.lifetime -= 25;
}

void All_Bullets::check_all_bullets()
{
    for(int i = 0; i < tank_bullets.size(); i++) {
        for(int j = 0; j < tank_bullets[i].size(); j++) {
            Bullet& b = tank_bullets[i][j];
            if(b.lifetime <= 0) {
                tank_bullets[i].erase(tank_bullets[i].begin() + j);
                j -= 1;
                continue;
            }
            this->check_bullet_wall_collision(tank_bullets[i][j]);
            this->move(tank_bullets[i][j]);
        }
    }
}