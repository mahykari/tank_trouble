#include "funcs.hpp"
#include "const_values.hpp"

using namespace std;

double get_distance(Point& p1, Point& p2) {
    double square_of_distance = 0;
    square_of_distance += pow((p1.x - p2.x), 2);
    square_of_distance += pow(p1.y - p2.y, 2);
    return sqrt(square_of_distance);
}

bool check_tank_tank_collision(Tank& t_1, Tank& t_2) {
    Point t_p_1 = t_1.get_position();
    Point t_p_2 = t_2.get_position();
    double distance = get_distance(t_p_1, t_p_2);
    if(distance < TANK_SIZE) return true;
    else return false; 
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

                if(event.get_pressed_key() == 'u') all_bullets.add_to_tank_bullets(1, tank_1.get_position(), tank_1.get_rotation()); 
                if(event.get_pressed_key() == 'q') all_bullets.add_to_tank_bullets(2, tank_2.get_position(), tank_2.get_rotation()); 
                break;
            }
        }    
    }
}

bool check_tank_wall_collision(const Tank& tank, const Map& map) {
    vector<Wall> walls = map.get_walls();
    Point tank_position = tank.get_position();
    for(int i = 0; i < walls.size(); i++) {
        Wall w = walls[i];
        if(abs(tank_position.x - w.src.x) <= HALF_TANK_SIZE && tank_position.y >= w.src.y - HALF_TANK_SIZE && tank_position.y <= w.dest.y + HALF_TANK_SIZE) {
            return true;
        } 
        else if(abs(tank_position.y - w.src.y) <= HALF_TANK_SIZE && tank_position.x >= w.src.x - HALF_TANK_SIZE && tank_position.x <= w.dest.x + HALF_TANK_SIZE) {
            return true;
        }
    }
    return false;

}

void make_map(Map& map,const vector<string>& wall_positions)
{
    for(int i = 0; i < wall_positions.size(); i++) {
        for(int j = 0; j < wall_positions[i].size(); j++) {
            switch(wall_positions[i][j]) {
                case '0' : break;
                case '1' : 
                {
                    Wall w_up( Point(j * GRID_SIZE, i * GRID_SIZE), Point((j + 1) * GRID_SIZE, i * GRID_SIZE) );
                    map.add_to_walls(w_up);
                    break;
                }

                case '2': 
                {
                    Wall w_left( Point(j * GRID_SIZE, i * GRID_SIZE), Point(j * GRID_SIZE, (i + 1) * GRID_SIZE) );
                    map.add_to_walls(w_left);
                    break;
                }

                case '3' :
                {
                    Wall w_up( Point(j * GRID_SIZE, i * GRID_SIZE), Point((j + 1) * GRID_SIZE, i * GRID_SIZE) );
                    Wall w_left( Point(j * GRID_SIZE, i * GRID_SIZE), Point(j * GRID_SIZE, (i + 1) * GRID_SIZE) );
                    map.add_to_walls(w_up); map.add_to_walls(w_left);
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
    map.add_to_walls(right_wall);
    map.add_to_walls(bottom_wall);
}

void read_map(string dir, int& win_width, int& win_height, Map& map, Point& t_1, Point& t_2) 
{
    ifstream map_file(dir.c_str());

    map_file >> win_height >> win_width;
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

void draw_walls(Window& game_window, Map& map)
{
    vector<Wall> walls = map.get_walls();
    for(int i = 0; i < walls.size(); i++) {
        game_window.draw_line(walls[i].src, walls[i].dest, WHITE);
    }
}

void draw_tank(Window& game_window, Tank& tank, Tank& second_tank, Map& map)
{
    int direction = tank.get_rotation_state();
    tank.set_rotation(direction * ANGULAR_VELOCITY);
    tank.move();
    if(check_tank_wall_collision(tank, map)) tank.move_back();
    else if(check_tank_tank_collision(tank, second_tank)) tank.move_back();
    Point p = tank.get_position();
    int half_tank_size = TANK_SIZE / 2;
    game_window.draw_img(tank.get_filename(), Rectangle(p - Point(half_tank_size, half_tank_size), TANK_SIZE, TANK_SIZE), NULL_RECT, tank.get_rotation());
}

void draw_bullet(Window& game_window,Bullet& b)
{
    game_window.fill_circle(b.position, HALF_BULLET_SIZE, WHITE);
}

void draw_all_bullets(Window& game_window, All_Bullets& all_bullets)
{
    vector<vector<Bullet> > b = all_bullets.get_tank_bullets();
    for(int i = 0; i < b.size(); i++) {
        for(int j= 0; j < b[i].size(); j++) {
            draw_bullet(game_window, b[i][j]);
        }
    }
}