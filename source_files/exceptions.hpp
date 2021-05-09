#pragma once

class quit_game {};
class fatal_collision {
public:
    int tank_id;
    fatal_collision(int _id) { tank_id = _id; }
};