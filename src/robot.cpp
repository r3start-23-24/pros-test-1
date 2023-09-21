#include "robot.hpp"

pros::Controller mainController (pros::E_CONTROLLER_MASTER);

pros::Motor left_motor_1 (2, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_2 (3, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_1 (8, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2 (9, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_drive_motors ({left_motor_1, left_motor_2});
pros::Motor_Group right_drive_motors ({right_motor_1, right_motor_2});

pros::Motor cata_motor_1 (6, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor cata_motor_2 (5, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group cata_motors ({cata_motor_1, cata_motor_2});
pros::ADIAnalogIn cata_limit_switch('A');

pros::Motor intake_motor_1 (4, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor intake_motor_2 (7, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group intake_motors ({intake_motor_1, intake_motor_2});