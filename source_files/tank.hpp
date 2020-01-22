#ifndef __TANK_HPP__
#define __TANK_HPP__
#include <iostream>
#include <vector>
#include "rsdl.hpp"
#include "map.hpp"
#include "bullets.hpp"

class Tank
{
private:
    int id;
    std::string filename;
    std::string status;
    bool i_should_rotate_clockwise = false;
    bool i_should_rotate_counterclockwise = false;
    bool i_should_move_forward = false;
    bool i_should_move_backward = false;
    Point position;
    double rotation = 0;
public:
    Tank(int _id, std::string status, Point _position, double _rotation);
    std::string get_filename();
    int get_id();
    void set_filename(std::string _filename);
    void switch_rotation(int direction);
    void switch_clockwise_rotation(bool state);
    void switch_counterclockwise_rotation(bool state);
    void set_rotation(double _rotation);
    void switch_moving_forward_state(bool state);
    void switch_moving_backward_state(bool state);
    std::string get_status();
    void set_status(std::string _status);
    Point get_position() const;
    void set_position(Point p);
    void move();
    void move_back();
    int get_rotation_state();
    double get_rotation();
    int get_moving_state();
};

#endif