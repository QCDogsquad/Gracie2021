
//~ Driver control
void opcontrol(){
 ArenaClear(&TransientArena);
 
 PreloadBegin(&RobotManager, &RobotPreload);
 
 imu_reset(RobotChassis.IMU);
 
 //~
 b8 LiftOverheated = false;
 
 b8 ReverseToggled = false;
 b8 SlowModeToggled = false;
 
 u32 PreviousTime = millis();
 f64 dTime = (f64)MILLISECONDS_PER_TICK / 1000;
 while(true)
 {
  //~ Chassis control
  f64 Forward = (f64)controller_get_analog(CONTROLLER_MASTER, ANALOG_LEFT_Y)/127.0;
  f64 Turn = (f64)controller_get_analog(CONTROLLER_MASTER, ANALOG_RIGHT_X)/127.0;
  
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_DOWN)) ReverseToggled = !ReverseToggled;
  if(ReverseToggled){
   Forward *= -1.0;
  }
  
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_UP)) SlowModeToggled = !SlowModeToggled;
  if(SlowModeToggled){
   Forward *= 0.5;
   Turn    *= 0.5;
  }
  
  ChassisMoveArcade(&RobotChassis, Forward, Turn);
#if 0
  v2 P = ChassisIntegrateP(&RobotChassis, dTime);
  printf("P: (%f, %f)\n", P.X, P.Y);
#endif
  robot_chassis *Chassis = &RobotChassis;
  {
   const f64 Epsilon = 0.15;
   
   imu_accel_s_t ddP = imu_get_accel(Chassis->IMU);
   local_persist f64 Rotation = 0.0;
   imu_gyro_s_t RotationRate = imu_get_gyro_rate(Chassis->IMU);
   if(RotationRate.z != INFINITY){
    if(fabs(RotationRate.z) > Epsilon){
     Rotation += RotationRate.z*dTime;
    }
   }
#if 0
   printf("R: %.6f | RR: %.8f\n", 
          //ddP.x, ddP.y, ddP.z, 
          Rotation, RotationRate.z);
#endif
  }
  
  //~ Lift controls
  
  // NOTE(Tyler): Fullway
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_R2)){
   switch(RobotLift.State){
    case LiftState_Free:    RobotLift.State = LiftState_Fullway; break;
    case LiftState_Raised:  RobotLift.State = LiftState_Fullway; break;
    case LiftState_Partway: RobotLift.State = LiftState_Fullway; break;
    case LiftState_Fullway: RobotLift.State = LiftState_Raised;  break;
   }
  }
  
  // NOTE(Tyler): Partway
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_R1)){
   switch(RobotLift.State){
    case LiftState_Free: {
     RobotLift.NextState = LiftState_Raised;
     RobotLift.State = LiftState_Partway;  
    }break;
    case LiftState_Raised: {
     RobotLift.NextState = LiftState_Fullway;
     RobotLift.State = LiftState_Partway;   
    }break;
    case LiftState_Fullway: {
     RobotLift.NextState = LiftState_Raised;
     RobotLift.State = LiftState_Partway;   
    }break;
    case LiftState_Partway: RobotLift.State = RobotLift.NextState; break;
   }
  }
  
  // NOTE(Tyler): Emergency manual control
  if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L2)){
   RobotLift.State = LiftState_Free;
   motor_move_velocity(LEFT_LIFT_MOTOR,  LIFT_VELOCITY);
   motor_move_velocity(RIGHT_LIFT_MOTOR, LIFT_VELOCITY);
  }else if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)){
   RobotLift.State = LiftState_Free;
   motor_move_velocity(LEFT_LIFT_MOTOR,  -LIFT_VELOCITY);
   motor_move_velocity(RIGHT_LIFT_MOTOR, -LIFT_VELOCITY);
  }else if(RobotLift.State == LiftState_Free){
   motor_move_velocity(LEFT_LIFT_MOTOR,  0);
   motor_move_velocity(RIGHT_LIFT_MOTOR, 0);
  }
  
  //~ Safety and performance warning
  if((motor_get_temperature(LEFT_LIFT_MOTOR)  >= 55) ||
     (motor_get_temperature(RIGHT_LIFT_MOTOR) >= 55)){
   if(!LiftOverheated){
    controller_rumble(CONTROLLER_MASTER, "--------");
    LiftOverheated = true;
   }
  }else LiftOverheated = false;
  
#if 0  
  printf("Temperature: %f %f\n", 
         motor_get_temperature(LEFT_LIFT_MOTOR), 
         motor_get_temperature(RIGHT_LIFT_MOTOR));
#endif
  
  //~ Testing
#if 0
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_A)){
   DoPreload(&RobotManager, &RobotPreload, &RobotLift);
  }
#if 0
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_X)){
   ChassisFollowTrajectory(&RobotChassis, &Trajectory);
  }
#endif
#endif
  
  task_delay_until(&PreviousTime, MILLISECONDS_PER_TICK);
 }
}