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
}

void points_auton() {
    intake_motor.move(127);
    move_lemlib(-1.2, 'y', 100);
    back_left_wing.set_value(true);
    lemlib_chassis.turnTo(-1.2*one_lemlib_tile, 0, 1500, 70); // x must be the same as the value on line above, pos/neg may need to be changed. y must be 0
    lemlib_chassis.moveTo(1*one_lemlib_tile, -3*one_lemlib_tile, 1500);
    back_left_wing.set_value(true);
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
    back_right_wing.set_value(true);
    move_lemlib(-2, 'y');
    move_lemlib(0.5, 'y');
    back_left_wing.set_value(true);
    back_right_wing.set_value(true);
    lemlib_chassis.turnTo(2.5*one_lemlib_tile, -2*one_lemlib_tile, 1500, 70);
    intake_motor.move(-127);
    move_lemlib(-0.6, 'y');
}

void awp_auton() {
	intake_motor.move(127);
	move_lemlib(2.3, 'y', 90);
	intake_motor.move(0);
	lemlib_chassis.turnTo(-100, 2.5*one_lemlib_tile, 1000, 60);
	front_left_wing.set_value(true);
	intake_motor.move(-127);
	//move_lemlib(1, 'x', 90);
    lemlib_chassis.moveTo(one_lemlib_tile, 2.25*one_lemlib_tile-2, 3000, 100);
	front_left_wing.set_value(false);
	//move_lemlib(-0.5, 'x', 90);
	lemlib_chassis.moveTo(0.4*one_lemlib_tile, 2.25*one_lemlib_tile, 2000, 80);
	lemlib_chassis.moveTo(-one_lemlib_tile+12, one_lemlib_tile/2+1, 3000, 60);
    
	lemlib_chassis.turnTo(-24, 15, 1000, false, 127);
    lemlib_chassis.moveTo(-25, 17, 3000, 100);
    back_left_wing.set_value(true);
    pros::c::delay(200);
    //lemlib_chassis.turnTo(-24, -12, 1000, true, 127);
    lemlib_chassis.moveTo(0, 0, 3000, 100);
    back_left_wing.set_value(false);
    move_lemlib(-2.3, 'y', 90);
}
