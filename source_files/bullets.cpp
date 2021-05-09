#include "rsdl.hpp"
#include "bullets.hpp"
#include "const_values.hpp"
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

All_Bullets::All_Bullets(int tank_count, Map* _map, Window* _win) 
{
    tank_bullets = vector<vector<Bullet*> > (tank_count);
    map = _map;
    game_window = _win;
}

Pickup::Pickup(Point _position, int t) 
{
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
    _position += Point((HALF_TANK_SIZE + 5) * cos(_rotation * PI / 180), (HALF_TANK_SIZE + 5) * sin(_rotation * PI / 180));
    Bullet* b = new Bullet(_position, BULLET_VELOCITY, _rotation);
    check_bullet_wall_collision(b);
    tank_bullets[tank_id - 1].push_back(b);
    game_window->play_sound_effect(Assets_Dir + "/" + Tank_Shot + Wav);
}

void All_Bullets::add_to_pickup_bullets(Point _position, double _rotation, string pickup) 
{

    if(pickup == "DoubleBarrel") {
        _position += Point((HALF_TANK_SIZE + 5) * cos(_rotation * PI / 180), (HALF_TANK_SIZE + 5) * sin(_rotation * PI / 180));
        double r_1 = _rotation + 90;
        double r_2 = _rotation - 90;
        Point p_1 = _position + Point((TANK_SIZE / 8) * cos(r_1 * PI / 180), TANK_SIZE / 8 * sin(r_1 * PI / 180));
        Point p_2 = _position + Point((TANK_SIZE / 8) * cos(r_2 * PI / 180), TANK_SIZE / 8 * sin(r_2 * PI / 180));
        Bullet* b_1 = new Bullet(p_1, BULLET_VELOCITY, _rotation);
        Bullet* b_2 = new Bullet(p_2, BULLET_VELOCITY, _rotation);
        check_bullet_wall_collision(b_1);
        check_bullet_wall_collision(b_2);
        pickup_bullets.push_back(b_1);
        pickup_bullets.push_back(b_2);
    }

    else if(pickup == "Shotgun") {
        vector<double> r(5);
        vector<Point> p(5);
        vector<Bullet*> b(5);
        _rotation -= 20.00;
        for(int i = 0; i < 5; i++) {
            r[i] += _rotation + i * 10.00;
            p[i] = _position + Point((HALF_TANK_SIZE + 10) * cos(r[i] * PI / 180), (HALF_TANK_SIZE) * sin(r[i] * PI / 180));
            b[i] = new Bullet(p[i], PICKUP_BULLET_VELOCITY, r[i]);
            check_bullet_wall_collision(b[i]);
            pickup_bullets.push_back(b[i]);
        }
    }
    game_window->play_sound_effect(Assets_Dir + "/" + Tank_Shot + Wav);
}

vector<vector<Bullet*> > All_Bullets::get_tank_bullets() 
{
    return tank_bullets;
}

vector<Bullet*> All_Bullets::get_pickup_bullets()
{
    return pickup_bullets;
}

void All_Bullets::check_bullet_wall_collision(Bullet* b) 
{
    int MAX = max(HALF_BULLET_SIZE, BULLET_VELOCITY);
    vector<Wall> walls = map->get_walls();
    for(int i = 0; i < walls.size(); i++) {
        Wall w = walls[i];
        if(abs(b->position.x - w.src.x) < MAX && b->v_x <= 0 && b->position.y >= w.src.y - HALF_BULLET_SIZE && b->position.y <= w.dest.y + HALF_BULLET_SIZE) {
            b->position.x = w.src.x + MAX;
            b->v_x = -b->v_x;
        }
        
        else if(abs(w.src.x - b->position.x) < MAX && b->v_x >= 0 && b->position.y >= w.src.y - HALF_BULLET_SIZE && b->position.y <= w.dest.y + HALF_BULLET_SIZE) {
            b->position.x = w.src.x - MAX;
            b->v_x = -b->v_x;
        }

        if(abs(b->position.y - w.src.y) < MAX && b->v_y <= 0 && b->position.x >= w.src.x - HALF_BULLET_SIZE && b->position.x <= w.dest.x + HALF_BULLET_SIZE) {
            b->position.y = w.src.y + MAX;
            b->v_y = -b->v_y;
        }

        else if(abs(w.src.y - b->position.y) < MAX && b->v_y >= 0 && b->position.x >= w.src.x - HALF_BULLET_SIZE && b->position.x <= w.dest.x + HALF_BULLET_SIZE) {
            b->position.y = w.src.y - MAX;
            b->v_y = -b->v_y;
        }
    }
}

void All_Bullets::move(Bullet* b) 
{
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

    for(int i = 0; i < pickup_bullets.size(); i++) {
        Bullet* b = pickup_bullets[i];
        if(b->lifetime <= 0 || b->shield_collision == true) {
            delete b;
            pickup_bullets.erase(pickup_bullets.begin() + i);
            i -= 1;
            continue;
        }

        this->move(pickup_bullets[i]);
        this->check_bullet_wall_collision(pickup_bullets[i]);
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

    for(int i = 0; i < pickup_bullets.size(); i++) {
        this->draw_bullet(pickup_bullets[i]);
    }
}

void All_Bullets::draw_pickups() 
{
    for(int i = 0; i < pickups.size(); i++) {
        Pickup* p = pickups[i];
        p->lifetime -= DELAY_PER_FRAME;
        game_window->draw_img(p->filename, Rectangle(p->position - Point(QUARTER_TANK_SIZE, QUARTER_TANK_SIZE), HALF_TANK_SIZE, HALF_TANK_SIZE));
    }
}

vector<Pickup*> All_Bullets::get_pickups() { return pickups; }