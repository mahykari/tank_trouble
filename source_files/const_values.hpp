#ifndef __CONST__
#define __CONST__
const int TANK_SIZE = 60;
const int GRID_SIZE = 3 * TANK_SIZE;
const int HALF_TANK_SIZE = TANK_SIZE / 2;
const int BULLET_SIZE = TANK_SIZE / 5;
const int HALF_BULLET_SIZE = BULLET_SIZE / 2;
const int ANGULAR_VELOCITY = 2;
const int TANK_VELOCITY = 3;
const int BULLET_VELOCITY = 6;
const int DELAY_PER_FRAME = 33;
const int TIME_UNIT = 1000;
const int BULLET_LIFETIME = 10 * TIME_UNIT; 
const double PI = 3.1415926536;

const std::string Assets_Dir = "Assets";
const std::string Generic_Tank = "Tank";
const std::string Extension = ".png";

#endif