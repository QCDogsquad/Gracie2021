#ifndef _PROS_MAIN_H_
#define _PROS_MAIN_H_

#define PROS_USE_SIMPLE_NAMES

#define PROS_USE_LITERALS

#define MILLISECONDS_PER_TICK 10

//~ Library includes
#include "api.h"
#include "pros/apix.h"
#include "assert.h"

#include "okapi/api.hpp"
using namespace pros;

using namespace pros::c;
using namespace pros::literals;
using namespace okapi::literals;

//~
#include "tyler_basics.h"
#include "robot_basics.h"
#include "memory_arena.cpp"
#include "array.cpp"

#include "integration.h"
#include "motion_profile.h"

#include "chassis.h"

#include "autonomous.h"

global_constant u8 IMU_PORT = 8;

global_constant	u8 LEFT_DRIVE_MOTOR = 1;
global_constant	u8 RIGHT_DRIVE_MOTOR = 10;

global_constant u8 LIFT_LIMIT = 'A';
global_constant u8 LEFT_LIFT_MOTOR  = 11;
global_constant u8 RIGHT_LIFT_MOTOR = 20;
global_constant f64 LIFT_VELOCITY =  0.5*GEARSET_36_MAX_VELOCITY;
global_constant f64 LIFT_PARTWAY_OFFSET = 50;

global_constant u8 PRELOAD_MOTOR = 17;
global_constant f64 PRELOAD_EMPTY_P = -170;
global_constant f64 PRELOAD_HOLD_P = -60;
global_constant f64 PRELOAD_VELOCITY = 40.0;

//~ Lift
enum lift_state {
 LiftState_Raised  = 0,
 LiftState_Partway = 1,
 LiftState_Fullway = 2,
 LiftState_Free    = 3,
};

struct lift_data {
 f64 Movement = 200;
 lift_state State;
 lift_state NextState; // Used for LiftState_Partway;
 b8 Done;
};

//~ Preload
enum preload_state {
 PreloadState_Set, 
 PreloadState_Empty, 
};

struct preload_data {
 preload_state State;
};

//~ Robot
enum robot_subsystem {
 Subsystem_None,
 Subsystem_Lift,
 Subsystem_Chassis,
 Subsystem_Preload,
 
 Subsystem_TOTAL
};

typedef u32 robot_subsystem_flags;
enum robot_subsystem_flags_ {
 SubsystemFlag_None    = (0 << 0),
 SubsystemFlag_Lift    = (1 << (Subsystem_Lift-1)),
 SubsystemFlag_Chassis = (1 << (Subsystem_Chassis-1)),
 SubsystemFlag_Preload = (1 << (Subsystem_Preload-1)),
 
 SubsystemFlag_TOTAL
};

typedef u8 subsystem_state_flags;
enum subsystem_state_flags_ {
 SubsystemState_None    = (0 << 0),
 SubsystemState_Done    = (1 << 0),
}; 

struct robot_manager;
typedef void (*subsystem_function)(robot_manager *, void *);

struct subsystem_command {
 subsystem_function Function;
 subsystem_command *Next;
};

struct subsystem {
 subsystem_state_flags State;
 subsystem_command *Next;
};

struct robot_manager {
 task_t Task;
 mutex_t Mutex;
 subsystem Subsystems[Subsystem_TOTAL];
 
};

void RobotSubsystemNotDone(robot_manager *Robot, robot_subsystem Subsystem);

//~ Declarations
void autonomous(void);
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);

#endif  // _PROS_MAIN_H_
