#include "robot.hpp"

pros::Controller mainController (pros::E_CONTROLLER_MASTER);

pros::Motor left_motor_1 (11, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_2 (12, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_3 (13, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_1 (18, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2 (19, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_3 (20, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_drive_motors ({left_motor_1, left_motor_2, left_motor_3});
pros::Motor_Group right_drive_motors ({right_motor_1, right_motor_2, right_motor_3});

pros::Motor cata_motor (9, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::ADIAnalogIn cata_limit_switch('A');

pros::Motor intake_motor (2, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::ADIDigitalOut left_wing (3);
pros::ADIDigitalOut right_wing (2);

pros::ADIDigitalOut blocker (4);