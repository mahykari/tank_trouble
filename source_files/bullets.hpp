#ifndef __BULLET_HPP__
#define __BULLET_HPP__
#include "rsdl.hpp"
#include "map.hpp"
#include "const_values.hpp"
#include <vector>

class Bullet
{
public:
    Point position;
    int lifetime = BULLET_LIFETIME;
    int v_x, v_y;
    bool shield_collision;
    Bullet(Point _position, int _v, double _rotation);
};

class Pickup
{
public:
    Point position;
    int lifetime = PICKUP_LIFETIME;
    bool tank_collision;
    std::string type;
    std::string filename;
    Pickup(Point _position, int t);
};

class All_Bullets
{
    private:
        std::vector<std::vector<Bullet*> > tank_bullets;
        std::vector<Bullet*> pickup_bullets;
        std::vector<Pickup*> pickups;
        Map* map;
        Window* game_window;
    public:
        All_Bullets(int tank_count, Map* _map, Window* _win);
        std::vector<std::vector<Bullet*> > get_tank_bullets();
        std::vector<Bullet*> get_pickup_bullets();
        std::vector<Pickup*> get_pickups();
        
        void add_to_tank_bullets(int tank_id, Point _position, double _rotation);
        void add_to_pickup_bullets(Point _position, double _rotation, std::string pickup);
        
        void move(Bullet* b);
        void check_bullet_wall_collision(Bullet* b);
        void check_all_bullets();
        void check_pickups();
        void draw();
        void draw_all_bullets();
        void draw_bullet(Bullet* b);
        void draw_pickups();
};

#endif