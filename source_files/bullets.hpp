#ifndef __BULLET_HPP__
#define __BULLET_HPP__
#include "rsdl.hpp"
#include "map.hpp"
#include <vector>

struct Bullet
{
    Point position;
    int lifetime = 10000;
    int v_x, v_y;
    Point get_point();
    Bullet(Point _position, int _v, double _rotation);
};

class All_Bullets
{
    private:
        std::vector<std::vector<Bullet> > tank_bullets;
        std::vector<Bullet> tank_1_bullets;
        std::vector<Bullet> tank_2_bullets;
        std::vector<Bullet> power_up_bullets;
        Map* map;
    public:
        All_Bullets(int tank_count, Map* _map);
        void add_to_tank_bullets(int tank_id, Point _position, double _rotation);
        void add_to_power_up_bullets(Point _position, double _rotation);
        void move(Bullet& b);
        void check_bullet_wall_collision(Bullet& b);
        void check_all_bullets();
        std::vector<std::vector<Bullet> > get_tank_bullets();
        std::vector<Bullet> get_powerup_bullets();
};

#endif