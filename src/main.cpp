#include "main.h"

global basic_chassis GlobalBasicChassis;
global pidf_chassis  GlobalPIDFChassis;

#include "chassis.cpp"
#include "motion_profile.cpp"

//~ Initialization
void initialize(){ 
 InitializeBasicChassis(&GlobalBasicChassis, GEARSET_6_MAX_VELOCITY, 
                        LEFT_DRIVE_MOTOR, RIGHT_DRIVE_MOTOR);
 InitializeBasicChassis(&GlobalPIDFChassis, GEARSET_6_MAX_VELOCITY, 
                        LEFT_DRIVE_MOTOR, RIGHT_DRIVE_MOTOR);
}

//~ Competition initialization
void competition_initialize(){
}

//~ Disabled
void disabled(){
}

//~ Autonomous
void 
autonomous(){
 u32 PreviousTime = millis();
 while(true)
 {
  
  task_delay_until(&PreviousTime, MILLISECONDS_PER_TICK);
 }
}

//~ Driver control
void
opcontrol(){
 u32 PreviousTime = millis();
 f64 dTime = 1 / MILLISECONDS_PER_TICK;
 while(true)
 {
  f64 Forward = (f64)controller_get_analog(E_CONTROLLER_MASTER, ANALOG_LEFT_Y)/127.0;
  f64 Turn    = (f64)controller_get_analog(E_CONTROLLER_MASTER, ANALOG_LEFT_X)/127.0;
  
  // NOTE(Tyler): The inputs can be squared(after they are normalized) in order to lessen 
  // sensitivity.
  UpdateBasicChassisSkid(&GlobalBasicChassis, Forward, Turn);
  //UpdatePIDFChassisSkid(&GlobalPIDFChassis, Forward, Turn, dTime);
  
  task_delay_until(&PreviousTime, MILLISECONDS_PER_TICK);
 }
}