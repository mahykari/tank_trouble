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
    
    Map map;

    read_map(map_input_dir, win_width, win_height, map, tank_1_position, tank_2_position);

    Window game_window(win_width * 3 * TANK_SIZE + 1, win_height * 3 * TANK_SIZE + 1, "Tank Trouble!");
    All_Bullets all_bullets(2);

    Tank tank_1("Assets/Tank1-Normal.png", tank_1_position, rand() % 360);
    Tank tank_2("Assets/Tank2-Normal.png", tank_2_position, rand() % 360);
    
    game_window.clear();
    draw_tank(game_window, tank_1, tank_2, map);
    draw_tank(game_window, tank_2, tank_1, map);
    game_window.update_screen();
    while(true) {
        try {
            update_game_window(game_window, tank_1, tank_2, all_bullets);
            game_window.clear();
            draw_walls(game_window, map);
            draw_tank(game_window, tank_1, tank_2, map);
            draw_tank(game_window, tank_2, tank_1, map);
            all_bullets.check_all_bullets(map);
            draw_all_bullets(game_window, all_bullets);
            game_window.update_screen();
            delay(DELAY_PER_FRAME);
        }
        catch(quit_game& qg) { return 0;}
    }
}