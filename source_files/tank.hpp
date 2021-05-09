#ifndef __TANK_HPP__
#define __TANK_HPP__
#include <iostream>
#include <vector>
#include "rsdl.hpp"
#include "map.hpp"
#include "bullets.hpp"

class Taken_Pickup
{
public:
    std::string type;
    int shoot_count;
    int lifetime;
    Taken_Pickup(std::string _type, int _count, int _time) : type(_type), shoot_count(_count), lifetime(_time) {}
};

class Tank
{
private:
    int id;
    std::string filename;
    std::string status;
    Point position;
    bool i_should_rotate_clockwise = false;
    bool i_should_rotate_counterclockwise = false;
    bool i_should_move_forward = false;
    bool i_should_move_backward = false;
    double rotation = 0;
    
    Map* map;
    All_Bullets* all_bullets;
    Window* game_window;

    std::vector<Taken_Pickup> taken_pickups;

public:
    
    Tank(int _id, std::string status, Point _position, double _rotation, Map* _map, All_Bullets* b, Window* win);
    
    std::string get_filename();
    int get_id();
    int get_rotation_state();
    double get_rotation();
    int get_moving_state();

    void set_filename();
    void set_rotation(double _rotation);
    void set_status();
    void set_position(Point p);
    
    void switch_rotation(int direction);
    void switch_clockwise_rotation(bool state);
    void switch_counterclockwise_rotation(bool state);
    void switch_moving_forward_state(bool state);
    void switch_moving_backward_state(bool state);
    void update_taken_pickups();
    
    void shoot();
    void move();
    void move_back();
    void draw(Tank& second_tank);
    
    std::string get_status();
    Point get_position() const;

    bool check_tank_wall_collision();
    bool check_tank_tank_collision(Tank& second_tank);
    void check_tank_bullet_collision();
    void check_tank_pickup_collision();
};

#endif