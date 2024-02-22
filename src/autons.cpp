#include "autons.hpp"
#include "pros/rtos.h"
#include "robot.hpp"

const float one_lemlib_tile = 24;
float lemlib_x;
float lemlib_y;

void move_lemlib(float tiles, char direction, int velocity = 90) {
    direction == 'x' ? lemlib_x += one_lemlib_tile*tiles : lemlib_y += one_lemlib_tile*tiles;
    lemlib_chassis.moveTo(lemlib_x, lemlib_y, 3000, velocity);
}

void turn_lemlib(int alpha, int velocity = 50) {
    int theta = lemlib_chassis.getPose().theta;
    theta = lemlib::degToRad(theta);
    alpha = lemlib::degToRad(alpha);
    lemlib_chassis.turnTo(lemlib_x - sin(theta + alpha), lemlib_y + cos(theta + alpha), 1000);
}

void skills_auton() {
    lemlib_chassis.moveTo(0.2*one_lemlib_tile, -0.7*one_lemlib_tile, 2500, 80);
    lemlib_chassis.turnTo(one_lemlib_tile, 1.5*one_lemlib_tile, 1500);
	ratchet_piston.set_value(true);
    cata_motor.move(127);
    pros::c::delay(25000); // 25 seconds
    cata_motor.move(0);
    lemlib_chassis.moveTo(-2*one_lemlib_tile, -6.5*one_lemlib_tile, 2500, 80);
    lemlib_chassis.turnTo(0, 10000, 1500);
    intake_motor.move(-127);
    pros::c::delay(15000); // 15 seconds
    lemlib_chassis.moveTo(0, 3.3*one_lemlib_tile, 3000, 100);
    lemlib_chassis.moveTo(3.8*one_lemlib_tile, 0.5*one_lemlib_tile, 2000, 90);
    lemlib_chassis.moveTo(0, 3.3*one_lemlib_tile, 3000, 100);
    lemlib_chassis.turnTo(10000, 3.3*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(3*one_lemlib_tile, 3.3*one_lemlib_tile, 3000);
    front_left_wing.set_value(true);
    front_right_wing.set_value(true);
    move_lemlib(1.2, 'y');
}

void points_auton() {
    intake_motor.move(127);
    move_lemlib(-1.2, 'y', 100);
    back_left_wing.set_value(true);
    lemlib_chassis.turnTo(-1.2*one_lemlib_tile, 0, 1500, 70); // x must be the same as the value on line above, pos/neg may need to be changed. y must be 0
    lemlib_chassis.moveTo(1*one_lemlib_tile, -3*one_lemlib_tile, 1500);
    back_left_wing.set_value(false);
    lemlib_chassis.turnTo(-1000, -2.5*one_lemlib_tile, 1500, 70);
    move_lemlib(1, 'x');
    move_lemlib(-0.5, 'x');
    lemlib_chassis.turnTo(1000, -2.5*one_lemlib_tile, 1500, 70);
    intake_motor.move(-127);
    move_lemlib(0.5, 'x');
    move_lemlib(-0.5, 'x');
    intake_motor.move(127);
    lemlib_chassis.moveTo(1.5*one_lemlib_tile, -0.1*one_lemlib_tile, 2000, 90);
    lemlib_chassis.turnTo(2*one_lemlib_tile, -1*one_lemlib_tile, 2000, 70);
    intake_motor.move(-127);
    pros::c::delay(500); // wait for ball to be outtaked
    intake_motor.move(127);
    lemlib_chassis.moveTo(2.5*one_lemlib_tile, -0.1*one_lemlib_tile, 1500, 60);
    lemlib_chassis.turnTo(2.5*one_lemlib_tile, 1000, 1500, 60);
    back_left_wing.set_value(true);
    move_lemlib(-2, 'y');
    move_lemlib(0.5, 'y');
    back_left_wing.set_value(false);
    lemlib_chassis.turnTo(2.5*one_lemlib_tile, -2*one_lemlib_tile, 1500, 70);
    intake_motor.move(-127);
    move_lemlib(-0.6, 'y');
}

void awp_auton() {
	intake_motor.move(127);
	move_lemlib(2.3, 'y', 90);
    lemlib_chassis.turnTo(100, 2.3*one_lemlib_tile, 1000);
	front_left_wing.set_value(true);
    lemlib_chassis.turnTo(-1000, 2.3*one_lemlib_tile, 1000, 60);
    lemlib_chassis.moveTo(1.2*one_lemlib_tile, 2.3*one_lemlib_tile, 3000, 100);
    // pushed over
	front_left_wing.set_value(false);
    lemlib_chassis.turnTo(-1000, 2.3*one_lemlib_tile, 1500);
    intake_motor.move(-127);
    lemlib_chassis.moveTo(-10, 2.3*one_lemlib_tile, 1500);
    pros::c::delay(500);
    lemlib_chassis.moveTo(0, 2.3*one_lemlib_tile, 1500);
    lemlib_chassis.moveTo(0, 1.5*one_lemlib_tile, 1500);
    /*
	lemlib_chassis.moveTo(-12, 0, 3000, 60);
	lemlib_chassis.turnTo(-24, 6, 1000, false, 127);
    */
    lemlib_chassis.moveTo(-26, 18, 3000, 100);
    back_left_wing.set_value(true);
    pros::c::delay(200);
    lemlib_chassis.moveTo(0, 0, 3000, 127);
    back_left_wing.set_value(false);
    pros::c::delay(200);
    lemlib_chassis.turnTo(-1000, 0, 1500, true);
    lemlib_chassis.moveTo(0.7*one_lemlib_tile, 0, 3000, 100);
}
