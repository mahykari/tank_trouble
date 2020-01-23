#ifndef __BULLET_HPP__
#define __BULLET_HPP__
#include "rsdl.hpp"
#include "map.hpp"
#include "const_values.hpp"
#include <vector>

struct Bullet
{
    Point position;
    int lifetime = BULLET_LIFETIME;
    int v_x, v_y;
    bool shield_collision;
    Bullet(Point _position, int _v, double _rotation);
};

struct Pickup
{
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
        std::vector<Bullet*> power_up_bullets;
        std::vector<Pickup*> pickups;
        Map* map;
        Window* game_window;
    public:
        All_Bullets(int tank_count, Map* _map, Window* _win);
        std::vector<std::vector<Bullet*> > get_tank_bullets();
        std::vector<Bullet*> get_powerup_bullets();
        std::vector<Pickup*> get_pickups();
        void add_to_tank_bullets(int tank_id, Point _position, double _rotation);
        void add_to_power_up_bullets(Point _position, double _rotation);
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