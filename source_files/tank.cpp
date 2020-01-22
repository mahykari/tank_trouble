#include "tank.hpp"
#include "rsdl.hpp"
#include "const_values.hpp"
#include <iostream>
#include <cmath>

Tank::Tank(std::string _filename, Point _position, double _rotation)
{
    filename = _filename;
    position = _position;
    rotation = _rotation;
}

void Tank::set_filename(std::string _filename)
{
    filename = _filename;
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

std::string Tank::get_filename()
{
    return filename;
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
    double pi = 3.1415926536;
    int v_x = (double)TANK_VELOCITY * cos(((double)rotation * pi) / 180.00);
    int v_y = (double)TANK_VELOCITY * sin(((double)rotation * pi) / 180.00);
    // std::cout << v_x << v_y << std::endl;
    if(i_should_move_forward) position =  position + Point(v_x, v_y);
    if(i_should_move_backward) position =  position - Point(v_x, v_y);
}

void Tank::move_back()
{
    double pi = 3.1415926536;
    int v_x = (double)TANK_VELOCITY * cos((rotation * pi) / 180.00);
    int v_y = (double)TANK_VELOCITY * sin((rotation * pi) / 180.00);
    // std::cout << v_x << v_y << std::endl;
    if(i_should_move_forward) position =  position - Point(v_x, v_y);
    if(i_should_move_backward) position =  position + Point(v_x, v_y);   
}
