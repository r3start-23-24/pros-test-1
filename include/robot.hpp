#ifndef ROBOT_HPP

#include "api.h"
#include "pros/motors.hpp"

extern pros::Controller mainController;

extern pros::Motor left_motor_1;
extern pros::Motor left_motor_2;
extern pros::Motor right_motor_1;
extern pros::Motor right_motor_2;
extern pros::Motor_Group left_drive_motors;
extern pros::Motor_Group right_drive_motors;

extern pros::Motor cata_motor;
extern pros::ADIAnalogIn cata_limit_switch;

extern pros::Motor intake_motor_1;
extern pros::Motor intake_motor_2;
extern pros::Motor_Group intake_motors;

#define ROBOT_HPP
#endif