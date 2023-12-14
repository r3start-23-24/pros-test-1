#ifndef ROBOT_HPP

#include "api.h"
#include "pros/motors.hpp"
#include "lemlib/api.hpp"

extern pros::Controller mainController;

extern pros::Motor left_motor_1;
extern pros::Motor left_motor_2;
extern pros::Motor left_motor_3;
extern pros::Motor right_motor_1;
extern pros::Motor right_motor_2;
extern pros::Motor right_motor_3;
extern pros::Motor_Group left_drive_motors;
extern pros::Motor_Group right_drive_motors;

extern pros::Motor cata_motor;
extern pros::ADIAnalogIn cata_limit_switch;
extern pros::Rotation cata_rotation_sensor;

extern pros::Motor intake_motor;

extern pros::ADIDigitalOut left_wing;
extern pros::ADIDigitalOut right_wing;

extern pros::ADIDigitalOut blocker;

extern pros::Imu inertial_sensor;

extern lemlib::Chassis lemlib_chassis;

#define ROBOT_HPP
#endif