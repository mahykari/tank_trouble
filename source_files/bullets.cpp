#include "rsdl.hpp"
#include "bullets.hpp"
#include "const_values.hpp"
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

All_Bullets::All_Bullets(int tank_count, Map* _map, Window* _win) {
    tank_bullets = vector<vector<Bullet*> > (tank_count);
    map = _map;
    game_window = _win;
}

Pickup::Pickup(Point _position, int t) {
    position = Point(_position.x * TANK_SIZE + HALF_TANK_SIZE, _position.y * TANK_SIZE + HALF_TANK_SIZE);
    type = Tank_Types[t];
    tank_collision = false;
    filename = Assets_Dir + "/" + type + "-" + Generic_Pickup + Extension;
}

Bullet::Bullet(Point _position, int _v, double _rotation)
{
    position = _position;
    v_x = (double)_v * cos(_rotation * PI / 180);
    v_y = (double)_v * sin(_rotation * PI / 180);
    shield_collision = false;
}

void All_Bullets::add_to_tank_bullets(int tank_id, Point _position, double _rotation) 
{
    if(tank_bullets[tank_id - 1].size() == 5) return;
    _position += Point((HALF_TANK_SIZE) * cos(_rotation * PI / 180), (HALF_TANK_SIZE) * sin(_rotation * PI / 180));
    Bullet* b = new Bullet(_position, BULLET_VELOCITY, _rotation);
    check_bullet_wall_collision(b);
    tank_bullets[tank_id - 1].push_back(b);
}

vector<vector<Bullet*> > All_Bullets::get_tank_bullets() 
{
    return tank_bullets;
}

void All_Bullets::check_bullet_wall_collision(Bullet* b) {
    int MAX = max(HALF_BULLET_SIZE, BULLET_VELOCITY);
    vector<Wall> walls = map->get_walls();
    for(int i = 0; i < walls.size(); i++) {
        Wall w = walls[i];
        if(abs(b->position.x - w.src.x) < MAX && b->v_x < 0 && b->position.y >= w.src.y - HALF_BULLET_SIZE && b->position.y <= w.dest.y + HALF_BULLET_SIZE) {
            b->position.x = w.src.x + MAX;
            b->v_x = -b->v_x;
            return;
        }
        
        else if(abs(w.src.x - b->position.x) < MAX && b->v_x > 0 && b->position.y >= w.src.y - HALF_BULLET_SIZE && b->position.y <= w.dest.y + HALF_BULLET_SIZE) {
            b->position.x = w.src.x - MAX;
            b->v_x = -b->v_x;
            return;
        }

        if(abs(b->position.y - w.src.y) < MAX && b->v_y < 0 && b->position.x >= w.src.x - HALF_BULLET_SIZE && b->position.x <= w.dest.x + HALF_BULLET_SIZE) {
            b->position.y = w.src.y + MAX;
            b->v_y = -b->v_y;
            return;
        }

        if(abs(w.src.y - b->position.y) < MAX && b->v_y > 0 && b->position.x >= w.src.x - HALF_BULLET_SIZE && b->position.x <= w.dest.x + HALF_BULLET_SIZE) {
            b->position.y = w.src.y - MAX;
            b->v_y = -b->v_y;
            return;
        }
    }
}

void All_Bullets::move(Bullet* b) {
    b->position += Point(b->v_x, b->v_y);
    b->lifetime -= DELAY_PER_FRAME;
}

void All_Bullets::check_all_bullets()
{
    for(int i = 0; i < tank_bullets.size(); i++) {
        for(int j = 0; j < tank_bullets[i].size(); j++) {
            Bullet* b = tank_bullets[i][j];
            if(b->lifetime <= 0 || b->shield_collision == true) {
                delete b;
                tank_bullets[i].erase(tank_bullets[i].begin() + j);
                j -= 1;
                continue;
            }
            this->move(tank_bullets[i][j]);
            this->check_bullet_wall_collision(tank_bullets[i][j]);
        }
    }
}

void All_Bullets::check_pickups() 
{
    for(int i = 0; i < pickups.size(); i++) {
        Pickup* p = pickups[i];
        if(p->lifetime <= 0 || p->tank_collision == true) {
            delete p;
            pickups.erase(pickups.begin() + i);
            i -= 1;
            continue;
        }
    }

    if(pickups.size() == 3) return;
    if(rand() % TIME_UNIT / 10 == 0) {
        Point win = map->get_dimensions();
        Point pos(rand() % (3 * int(win.x)), rand() % (3 * int(win.y)));
        int random_type = rand() % 3;
        random_type += 1;
        Pickup* p = new Pickup(pos, random_type);
        pickups.push_back(p);
    }
}

void All_Bullets::draw() 
{
    this->draw_all_bullets();
    this->draw_pickups();
}

void All_Bullets::draw_bullet(Bullet* b)
{
    game_window->fill_circle(b->position, HALF_BULLET_SIZE, WHITE);
}

void All_Bullets::draw_all_bullets()
{
    for(int i = 0; i < tank_bullets.size(); i++) {
        for(int j= 0; j < tank_bullets[i].size(); j++) {
            this->draw_bullet(tank_bullets[i][j]);
        }
    }
}

void All_Bullets::draw_pickups() {
    for(int i = 0; i < pickups.size(); i++) {
        Pickup* p = pickups[i];
        p->lifetime -= DELAY_PER_FRAME;
        game_window->draw_img(p->filename, Rectangle(p->position - Point(QUARTER_TANK_SIZE, QUARTER_TANK_SIZE), HALF_TANK_SIZE, HALF_TANK_SIZE));
    }
}