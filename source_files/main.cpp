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

    map->set_window(game_window);

    All_Bullets* all_bullets = new All_Bullets(2, map, game_window);

    vector<Tank> tanks;

    tanks.push_back(Tank(1, Tank_Types[0], tank_1_position, rand() % 360, map, all_bullets, game_window));
    tanks.push_back(Tank(2, Tank_Types[0], tank_2_position, rand() % 360, map, all_bullets, game_window));
    
    game_window->clear();

    tanks[0].draw(tanks[1]);
    tanks[1].draw(tanks[0]);
    game_window->update_screen();
    while(true) {
        try {
            update_game_window(*game_window, tanks[0], tanks[1], *all_bullets);
            game_window->clear();
            map->draw();
            all_bullets->check_all_bullets();
            all_bullets->check_pickups();
            all_bullets->draw();
            tanks[0].draw(tanks[1]);
            tanks[1].draw(tanks[0]);
            for(int i = 0; i < tanks.size(); i++) {
                tanks[i].check_tank_bullet_collision();
                tanks[i].check_tank_pickup_collision();
                tanks[i].update_taken_pickups();
            }

            game_window->update_screen();
            delay(DELAY_PER_FRAME);
        }
        catch(quit_game& qg) { return 0; }
        catch(fatal_collision& fc) {
            RGB color(0, 0, 0);
            if(fc.tank_id == 1) color = GREEN;
            else if(fc.tank_id == 2) color = RED;
            string end_text = "Tank " + to_string(fc.tank_id) + " is eliminated!";
            Point text_place(game_window->get_width() / 12, game_window->get_height() / 2);
            game_window->show_text(end_text, text_place, color, "Assets/TankTrouble.ttf", 48); 
            game_window->play_sound_effect(Assets_Dir + "/" + Tank_Explosion + Wav);
            game_window->update_screen();
            delay(4000);
            return 0;
        }
    }
}