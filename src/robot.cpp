
//~ Helpers
// TODO(Tyler): I do not like this, it seems like it is a problem with this system of 
// denoting subsystems.
b8 RobotIsDone(robot_manager *Robot, robot_subsystem_flags Flags){
 mutex_take(Robot->Mutex, TIMEOUT_MAX);
 
 for(u32 Index=1; Index < Subsystem_TOTAL; Index++){
  if((1<<(Index-1)) & Flags){
   if(!(Robot->Subsystems[Index].State & SubsystemState_Done)){
    mutex_give(Robot->Mutex);
    return false;
   }
  }
 }
 mutex_give(Robot->Mutex);
 
 return true;
}

void RobotWait(robot_manager *Robot, robot_subsystem_flags Flags){
 while(!RobotIsDone(Robot, Flags)){
  delay(10);
 }
}

void RobotSubsystemNotDone(robot_manager *Robot, robot_subsystem Subsystem){
 Robot->Subsystems[Subsystem].State &= ~SubsystemState_Done;
}

#define COMMAND(Robot, Flags, Func, ...) \
RobotWait(Robot, Flags); \
Func(Robot, __VA_ARGS__);


//~ Preload
void PreloadBegin(robot_manager *Robot, preload_data *Preload){
 motor_move_absolute(PRELOAD_MOTOR, PRELOAD_HOLD_P, PRELOAD_VELOCITY);
}

void EmptyPreload(robot_manager *Robot, preload_data *Preload){
 motor_move_absolute(PRELOAD_MOTOR, PRELOAD_EMPTY_P, PRELOAD_VELOCITY);
 Preload->State = PreloadState_Empty;
 RobotSubsystemNotDone(Robot, Subsystem_Preload);
}

void ResetPreload(robot_manager *Robot, preload_data *Preload){
 motor_move_absolute(PRELOAD_MOTOR, 0, PRELOAD_VELOCITY);
 Preload->State = PreloadState_Set;
 RobotSubsystemNotDone(Robot, Subsystem_Preload);
}

void DoPreload(robot_manager *Robot, preload_data *Preload, lift_data *Lift){
 COMMAND(Robot, SubsystemFlag_None,    LiftPartway, Lift);
 COMMAND(Robot, SubsystemFlag_Lift,    EmptyPreload,     Preload);
 delay(500); // Settle time
 COMMAND(Robot, SubsystemFlag_Preload, ResetPreload,     Preload);
}

// NOTE(Tyler): This is basic enough to go here for now
b8 PreloadBackgroundUpdate(preload_data *Preload){
 switch(Preload->State){
  case PreloadState_Set: {
   if(MotorIsAtPosition(PRELOAD_MOTOR, 0, 1)){
    return true;
   }
  }break;
  case PreloadState_Empty: {
   if(MotorIsAtPosition(PRELOAD_MOTOR, PRELOAD_EMPTY_P, 1)){
    return true;
   }
  }break;
 }
 
 return false;
}

//~ 
void RobotBackgroundUpdate(void *Data){
 robot_manager *Robot = (robot_manager *)Data;
 f64 dTime = 1.0 / MILLISECONDS_PER_TICK;
 
 while(true){
  mutex_take(Robot->Mutex, TIMEOUT_MAX);
#define BACKGROUND_UPDATE(Name, ...) do{ \
subsystem *Subsystem = &Robot->Subsystems[Subsystem_##Name]; \
Subsystem->State &= ~SubsystemState_Done; \
if(Name##BackgroundUpdate(__VA_ARGS__)){ \
Subsystem->State |= SubsystemState_Done; \
} \
}while(0)
  
  BACKGROUND_UPDATE(Chassis, &RobotChassis);
  BACKGROUND_UPDATE(Lift, &RobotLift);
  BACKGROUND_UPDATE(Preload, &RobotPreload);
  //if(ChassisBackgroundUpdate(&RobotChassis)) Robot->SystemFlags |= Subsystem_Chassis;
  //if(LiftBackgroundUpdate(&RobotLift)) Robot->SystemFlags |= Subsystem_Lift;
#undef BACKGROUND_UPDATE
  for(u32 I=1; I<Subsystem_TOTAL; I++){
   subsystem *Subsystem = &Robot->Subsystems[I];
   
  }
  mutex_give(Robot->Mutex);
  
  delay(MILLISECONDS_PER_TICK);
 }
}

void InitRobot(robot_manager *Robot){
 Robot->Mutex = mutex_create();
 Robot->Task = task_create(RobotBackgroundUpdate, &RobotManager, 
                           TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,
                           "Background update");
}
