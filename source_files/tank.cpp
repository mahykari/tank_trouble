#include "tank.hpp"
#include "rsdl.hpp"
#include "const_values.hpp"
#include "funcs.hpp"
#include <iostream>
#include <cmath>
#include "exceptions.hpp"

using namespace std;

Tank::Tank(int _id, string _status, Point _position, double _rotation, Map* _map, All_Bullets* b, Window* win)
{
    id = _id;
    status = _status; 
    position = _position;
    rotation = _rotation;
    map = _map;
    all_bullets = b;
    game_window = win;
    filename = Assets_Dir + "/" + Generic_Tank + to_string(id) + "-" + status + Extension;
}

void Tank::set_filename()
{
    filename = Assets_Dir + "/" + Generic_Tank + to_string(id) + "-" + status + Extension;
}

void Tank::set_position(Point p)
{
    position = p;
}

void Tank::set_rotation(double _rotation)
{
    rotation += _rotation;
}

Point Tank::get_position() const
{
    return position;
}

double Tank::get_rotation()
{
    return rotation;
}

string Tank::get_filename()
{
    return filename;
}

int Tank::get_id() 
{
    return id;
}

string Tank::get_status() 
{
    return status;
}

void Tank::switch_clockwise_rotation(bool state)
{
    i_should_rotate_clockwise = state;
}

void Tank::switch_counterclockwise_rotation(bool state)
{
    i_should_rotate_counterclockwise = state;
}

void Tank::switch_moving_forward_state(bool state)
{
    i_should_move_forward = state;
}

void Tank::switch_moving_backward_state(bool state)
{
    i_should_move_backward = state;
}

int Tank::get_rotation_state()
{
    if(i_should_rotate_clockwise) return 1;
    else if(i_should_rotate_counterclockwise) return -1;
    else return 0;
}

void Tank::move()
{
    int v_x = (double)TANK_VELOCITY * cos(((double)rotation * PI) / 180.00);
    int v_y = (double)TANK_VELOCITY * sin(((double)rotation * PI) / 180.00);
    if(i_should_move_forward) position =  position + Point(v_x, v_y);
    if(i_should_move_backward) position =  position - Point(v_x, v_y);
}

void Tank::move_back()
{
    int v_x = (double)TANK_VELOCITY * cos((rotation * PI) / 180.00);
    int v_y = (double)TANK_VELOCITY * sin((rotation * PI) / 180.00);
    if(i_should_move_forward) position =  position - Point(v_x, v_y);
    if(i_should_move_backward) position =  position + Point(v_x, v_y);   
}

void Tank::draw(Tank& second_tank)
{
    this->set_status();
    this->set_filename();
    int direction = this->get_rotation_state();
    this->set_rotation(direction * ANGULAR_VELOCITY);
    this->move();
    if(this->check_tank_wall_collision()) this->move_back();
    else if(this->check_tank_tank_collision(second_tank)) this->move_back();
    Point p = this->get_position();
    game_window->draw_img(this->get_filename(), Rectangle(p - Point(HALF_TANK_SIZE, HALF_TANK_SIZE), TANK_SIZE, TANK_SIZE), NULL_RECT, this->get_rotation());
}

bool Tank::check_tank_wall_collision() {
    vector<Wall> walls = map->get_walls();
    for(int i = 0; i < walls.size(); i++) {
        Wall w = walls[i];
        if(abs(this->position.x - w.src.x) <= HALF_TANK_SIZE && this->position.y >= w.src.y - HALF_TANK_SIZE && this->position.y <= w.dest.y + HALF_TANK_SIZE) {
            return true;
        } 
        else if(abs(this->position.y - w.src.y) <= HALF_TANK_SIZE && this->position.x >= w.src.x - HALF_TANK_SIZE && this->position.x <= w.dest.x + HALF_TANK_SIZE) {
            return true;
        }
    }
    return false;
}

void Tank::check_tank_bullet_collision() {
    const vector<vector<Bullet*> > b = all_bullets->get_tank_bullets();
    for(int i = 0; i < b.size(); i++) {
        for(int j = 0; j < b[i].size(); j++) {
            const Point b_p = b[i][j]->position;
            if(get_distance(this->position, b_p) < HALF_TANK_SIZE) {
                if(this->get_status() != "Shield") throw fatal_collision(this->id);
                else {
                    b[i][j]->shield_collision = true;
                }
            }
        }
    }

    const vector<Bullet*> p = all_bullets->get_pickup_bullets();
    for(int i = 0; i < p.size(); i++) {
        const Point b_p = p[i]->position;
        if(get_distance(this->position, b_p) < HALF_TANK_SIZE) {
            if(this->get_status() != "Shield") throw fatal_collision(this->id);
            else {
                p[i]->shield_collision = true;
            }
        }
    }
}

bool Tank::check_tank_tank_collision(Tank& second_tank) {
    double distance = get_distance(this->position, second_tank.position);
    if(distance < TANK_SIZE) return true;
    else return false;
}

void Tank::check_tank_pickup_collision()
{
    vector<Pickup*> p = all_bullets->get_pickups();
    if(p.size() == 0) return;
    for(int i = 0; i < p.size(); i++) {
        if(get_distance(this->position, p[i]->position) < 3 * TANK_SIZE / 4) {
            p[i]->tank_collision = true;
            string type = p[i]->type;
            
            if(type == Tank_Types[1]) {
                Taken_Pickup pickup(type, -1, SHIELD_LIFETIME);
                taken_pickups.push_back(pickup);
            }

            else if(type == Tank_Types[2]) {
                Taken_Pickup pickup(type, 3, -TIME_UNIT);
                taken_pickups.push_back(pickup);
            }

            else if(type == Tank_Types[3]) {
                Taken_Pickup pickup(type, 5, -TIME_UNIT);
                taken_pickups.push_back(pickup);
            }

            else if(type == Tank_Types[4]) {
                Taken_Pickup pickup(type, 1, LASER_LIFETIME);
                taken_pickups.push_back(pickup);
            }

            break;
        }
    }
}

void Tank::set_status()
{
    if(taken_pickups.size() == 0) {
        status = Tank_Types[0];
        return;
    }
    else status = taken_pickups[0].type;
}

void Tank::update_taken_pickups()
{
    if(taken_pickups.size() == 0) return;
    Taken_Pickup tp = taken_pickups[0];
    if(tp.type == "Shield") {
        if(tp.lifetime < 0) {
            taken_pickups.erase(taken_pickups.begin());
        }
        else taken_pickups[0].lifetime -= DELAY_PER_FRAME;
    }

    else if(tp.type == "DoubleBarrel" || tp.type == "Shotgun") {
        if(tp.shoot_count == 0) {
            taken_pickups.erase(taken_pickups.begin());
        }
    }
}

void Tank::shoot()
{
    string s = this->status;
    
    if(s == "Normal" || s == "Shield")
        all_bullets->add_to_tank_bullets(this->id, this->position, this->rotation);
    
    else if(s == "DoubleBarrel" || s == "Shotgun") {
        all_bullets->add_to_pickup_bullets(this->position, this->rotation, this->status);
        taken_pickups[0].shoot_count -= 1;
    }
}