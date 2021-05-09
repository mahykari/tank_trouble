#include "funcs.hpp"
#include "const_values.hpp"

using namespace std;

double get_distance(const Point& p1,const Point& p2) {
    double square_of_distance = 0;
    square_of_distance += pow((p1.x - p2.x), 2);
    square_of_distance += pow(p1.y - p2.y, 2);
    return sqrt(square_of_distance);
}

void update_game_window(Window& game_window, Tank& tank_1, Tank& tank_2, All_Bullets& all_bullets) 
{
    while(game_window.has_pending_event()) 
    {
        Event event = game_window.poll_for_event();
        switch(event.get_type()) 
        {
            case Event::QUIT : throw quit_game();
            case Event::KEY_PRESS : 
            {
                if(event.get_pressed_key() == 'j') tank_1.switch_counterclockwise_rotation(true);
                if(event.get_pressed_key() == 'l') tank_1.switch_clockwise_rotation(true);
                if(event.get_pressed_key() == 'i') tank_1.switch_moving_forward_state(true);
                if(event.get_pressed_key() == 'k') tank_1.switch_moving_backward_state(true);

                if(event.get_pressed_key() == 'a') tank_2.switch_counterclockwise_rotation(true);
                if(event.get_pressed_key() == 'd') tank_2.switch_clockwise_rotation(true);
                if(event.get_pressed_key() == 'w') tank_2.switch_moving_forward_state(true);
                if(event.get_pressed_key() == 's') tank_2.switch_moving_backward_state(true);
                
                if(event.get_pressed_key() == 'u') tank_1.shoot(); 
                if(event.get_pressed_key() == 'q') tank_2.shoot();   
                break;
            }
            case Event::KEY_RELEASE :
            {
                if(event.get_pressed_key() == 'j') tank_1.switch_counterclockwise_rotation(false);
                if(event.get_pressed_key() == 'l') tank_1.switch_clockwise_rotation(false);
                if(event.get_pressed_key() == 'i') tank_1.switch_moving_forward_state(false);
                if(event.get_pressed_key() == 'k') tank_1.switch_moving_backward_state(false);

                if(event.get_pressed_key() == 'a') tank_2.switch_counterclockwise_rotation(false);
                if(event.get_pressed_key() == 'd') tank_2.switch_clockwise_rotation(false);
                if(event.get_pressed_key() == 'w') tank_2.switch_moving_forward_state(false);
                if(event.get_pressed_key() == 's') tank_2.switch_moving_backward_state(false);

                break;
            }
        }    
    }
}

void make_map(Map* map, const vector<string>& wall_positions)
{
    for(int i = 0; i < wall_positions.size(); i++) {
        for(int j = 0; j < wall_positions[i].size(); j++) {
            switch(wall_positions[i][j]) {
                case '0' : break;
                case '1' : 
                {
                    Wall w_up( Point(j * GRID_SIZE, i * GRID_SIZE), Point((j + 1) * GRID_SIZE, i * GRID_SIZE) );
                    map->add_to_walls(w_up);
                    break;
                }

                case '2': 
                {
                    Wall w_left( Point(j * GRID_SIZE, i * GRID_SIZE), Point(j * GRID_SIZE, (i + 1) * GRID_SIZE) );
                    map->add_to_walls(w_left);
                    break;
                }

                case '3' :
                {
                    Wall w_up( Point(j * GRID_SIZE, i * GRID_SIZE), Point((j + 1) * GRID_SIZE, i * GRID_SIZE) );
                    Wall w_left( Point(j * GRID_SIZE, i * GRID_SIZE), Point(j * GRID_SIZE, (i + 1) * GRID_SIZE) );
                    map->add_to_walls(w_up); map->add_to_walls(w_left);
                    break;
                }
            }
        }
    }

    int map_height = wall_positions.size();
    int map_width = wall_positions[0].size();

    Point top_right(map_width * GRID_SIZE, 0);
    Point down_right(map_width * GRID_SIZE, map_height * GRID_SIZE);
    Point down_left(0, map_height * GRID_SIZE);
    
    Wall right_wall(top_right, down_right);
    Wall bottom_wall(down_left, down_right);
    
    map->add_to_walls(right_wall);
    map->add_to_walls(bottom_wall);
    map->unify_walls();
}

void read_map(string dir, int& win_width, int& win_height, Map* map, Point& t_1, Point& t_2) 
{
    ifstream map_file(dir.c_str());

    map_file >> win_height >> win_width;
    map->set_width_and_height(win_width, win_height);
    vector<string> wall_positions;

    for(int i = 0; i < win_height; i++) {
        string map_line;
        map_file >> map_line;
        wall_positions.push_back(map_line);
    }

    int tank_1_x, tank_1_y, tank_2_x, tank_2_y;
    map_file >> tank_1_x >> tank_1_y;
    map_file >> tank_2_x >> tank_2_y;

    t_1 = Point((3 * tank_1_y + 1) * TANK_SIZE + TANK_SIZE / 2, (3 * tank_1_x + 1) * TANK_SIZE + TANK_SIZE / 2);
    t_2 = Point((3 * tank_2_y + 1) * TANK_SIZE + TANK_SIZE / 2, (3 * tank_2_x + 1) * TANK_SIZE + TANK_SIZE / 2);
    make_map(map, wall_positions);
}