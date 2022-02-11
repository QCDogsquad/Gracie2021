#include "main.h"

global autonomous_selector AutonomousSelector;
global basic_chassis   RobotChassis;

#include "chassis.cpp"
#include "gui.cpp"
#include "opcontrol.cpp"
#include "autonomous.cpp"

//~ Initialization
void initialize(){ 
 InitGUI();
 
 motor_set_gearing(LEFT_DRIVE_MOTOR,  E_MOTOR_GEARSET_36);
 motor_set_gearing(RIGHT_DRIVE_MOTOR, E_MOTOR_GEARSET_36);
 
 InitChassis(&RobotChassis, 
             LEFT_DRIVE_MOTOR, RIGHT_DRIVE_MOTOR, IMU_PORT,
             GEARSET_36_MAX_VELOCITY, GEARSET_36_MAX_VELOCITY, 
             Inch(15.5), Inch(4), 60.0/84.0);
 
 motor_set_gearing(LIFT_MOTOR, E_MOTOR_GEARSET_36);
 motor_set_encoder_units(LIFT_MOTOR,   E_MOTOR_ENCODER_DEGREES);
 motor_set_brake_mode(LIFT_MOTOR,  E_MOTOR_BRAKE_HOLD);
 motor_tare_position(LIFT_MOTOR);
}

//~ Competition initialization
void competition_initialize(){
}

//~ Disabled
void disabled(){
}
