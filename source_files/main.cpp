#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include "rsdl.hpp"
#include "tank.hpp"
#include "map.hpp"
#include "bullets.hpp"
#include "funcs.hpp"
#include "const_values.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    int win_width, win_height;
    Point tank_1_position, tank_2_position;
    string map_input_dir = argv[1];
    
    Map* map = new Map();
    read_map(map_input_dir, win_width, win_height, map, tank_1_position, tank_2_position);

    Window* game_window = new Window(win_width * 3 * TANK_SIZE + 1, win_height * 3 * TANK_SIZE + 1, "Tank Trouble!");
    All_Bullets all_bullets(2, map, game_window);

    Tank tank_1(1, Tank_Types[0], tank_1_position, rand() % 360);
    Tank tank_2(2, Tank_Types[0], tank_2_position, rand() % 360);
    
    game_window->clear();
    draw_tank(*game_window, tank_1, tank_2, map);
    draw_tank(*game_window, tank_2, tank_1, map);
    game_window->update_screen();
    while(true) {
        try {
            update_game_window(*game_window, tank_1, tank_2, all_bullets);
            game_window->clear();
            draw_walls(*game_window, map);
            draw_tank(*game_window, tank_1, tank_2, map);
            draw_tank(*game_window, tank_2, tank_1, map);
            all_bullets.check_all_bullets();
            all_bullets.check_pickups();
            all_bullets.draw();
            game_window->update_screen();
            check_tank_bullet_collision(tank_1, all_bullets);
            check_tank_bullet_collision(tank_2, all_bullets);
            delay(DELAY_PER_FRAME);
        }
        catch(quit_game& qg) { return 0;}
        catch(fatal_collision& fc) {
            cout << "Tank " << to_string(fc.tank_id) << " is eliminated\n"; 
            delay(1000);
            return 0;
        }
    }
}