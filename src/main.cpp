#include "main.h"

global auto_selector AutoSelector;

global robot_chassis RobotChassis;
global lift_data     RobotLift;
global preload_data  RobotPreload;

global robot_manager  RobotManager;

#include "basics.cpp"
#include "motion_profile.cpp"
#include "trajectory.cpp"

#include "lift.cpp"
#include "chassis.cpp"
#include "gui.cpp"

#include "robot.cpp"

#include "opcontrol.cpp"
#include "autonomous.cpp"

//~ Initialization

void initialize(){ 
 {
  u32 Size = Kilobytes(64);
  void *Memory = malloc(Size);
  InitializeArena(&PermanentArena, Memory, Size);
 }
 {
  u32 Size = Kilobytes(512);
  void *Memory = malloc(Size);
  InitializeArena(&TransientArena, Memory, Size);
 }
 
 InitGUI();
 InitLift(&RobotLift);
 
 //~ Preload
 motor_set_gearing(PRELOAD_MOTOR, E_MOTOR_GEARSET_18);
 motor_set_brake_mode(PRELOAD_MOTOR, E_MOTOR_BRAKE_HOLD);
 motor_set_encoder_units(PRELOAD_MOTOR, E_MOTOR_ENCODER_DEGREES);
 
 //~ Initialize chassis
 InitChassis(&RobotChassis, 
             LEFT_DRIVE_MOTOR, RIGHT_DRIVE_MOTOR,
             GEARSET_36_MAX_VELOCITY, GEARSET_36_MAX_VELOCITY,
             Inch(15.25), Inch(4), 
             60.0/84.0, E_MOTOR_GEARSET_36);
 ChassisBrakeMode(&RobotChassis, E_MOTOR_BRAKE_HOLD);
 ChassisReverse(&RobotChassis, true);
 RobotChassis.IMU = IMU_PORT;
 //imu_reset(RobotChassis.IMU);
 InitRobot(&RobotManager);
}

//~ Competition initialization
void competition_initialize(){
}

//~ Disabled
void disabled(){
}
