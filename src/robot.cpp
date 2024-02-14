#include "robot.hpp"
#include "pros/adi.hpp"
#include "pros/rotation.hpp"
#include "lemlib/api.hpp"

pros::Controller mainController (pros::E_CONTROLLER_MASTER);

pros::Motor left_motor_1 (11, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_2 (12, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor left_motor_3 (13, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_1 (18, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_2 (19, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_motor_3 (20, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor_Group left_drive_motors ({left_motor_1, left_motor_2, left_motor_3});
pros::Motor_Group right_drive_motors ({right_motor_1, right_motor_2, right_motor_3});

pros::Motor cata_motor (17, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Rotation cata_rotation_sensor (15);

pros::Motor intake_motor (14, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::ADIDigitalOut pto (4);
pros::ADIDigitalOut front_left_wing (1);
pros::ADIDigitalOut front_right_wing (2);
pros::ADIDigitalOut back_left_wing (3);
pros::ADIDigitalOut back_right_wing (5); // to be removed
// ratchet is 5

pros::Imu inertial_sensor(16);

// lemlib below

lemlib::Drivetrain_t lemlib_drive {
    &left_drive_motors,
    &right_drive_motors,
    11.25, // track width
    4, // wheel diameter
    257 // wheel rpm
};

lemlib::OdomSensors_t lemlib_sensors {
    nullptr, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    nullptr, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2
    &inertial_sensor // inertial sensor
};

// forward/backward PID
lemlib::ChassisController_t regular_pid {
    35, // kP 8
    35, // kD
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    5 // slew rate
};
lemlib::ChassisController_t turn_pid {
    9, // kP 8
    70, // kD 65
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    0 // slew rate
};

lemlib::Chassis lemlib_chassis(lemlib_drive, regular_pid, turn_pid, lemlib_sensors);
