
//~ Driver control
void opcontrol(){
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
   
   imu_accel_s_t ddP = imu_get_accel(Chassis->IMU);
   f64 Rotation = -imu_get_rotation(Chassis->IMU);
   printf("A: (%f, %f, %f) | R: %f\n", ddP.x, ddP.y, ddP.z, Rotation);
  }
  
  //~ Lift controls
  
  // NOTE(Tyler): Fullway
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_R2)){
   switch(LiftData.State){
    case LiftState_Free:
    case LiftState_Raised:  LiftData.State = LiftState_Fullway; break;
    case LiftState_Partway: LiftData.State = LiftState_Fullway; break;
    case LiftState_Fullway: LiftData.State = LiftState_Raised; break;
   }
  }
  
  // NOTE(Tyler): Partway
  if(controller_get_digital_new_press(CONTROLLER_MASTER, DIGITAL_R1)){
   switch(LiftData.State){
    case LiftState_Free:
    case LiftState_Raised:  LiftData.State = LiftState_Partway; break;
    case LiftState_Fullway: LiftData.State = LiftState_Partway; break;
    case LiftState_Partway: LiftData.State = LiftState_Raised; break;
   }
  }
  
  // NOTE(Tyler): Emergency manual control
  if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L2)){
   LiftData.State = LiftState_Free;
   motor_move_velocity(LEFT_LIFT_MOTOR,  LIFT_VELOCITY);
   motor_move_velocity(RIGHT_LIFT_MOTOR, LIFT_VELOCITY);
  }else if(controller_get_digital(CONTROLLER_MASTER, DIGITAL_L1)){
   LiftData.State = LiftState_Free;
   motor_move_velocity(LEFT_LIFT_MOTOR,  -LIFT_VELOCITY);
   motor_move_velocity(RIGHT_LIFT_MOTOR, -LIFT_VELOCITY);
  }else if(LiftData.State == LiftState_Free){
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
  
  printf("Temp: %f %f\n", 
         motor_get_temperature(LEFT_LIFT_MOTOR), 
         motor_get_temperature(RIGHT_LIFT_MOTOR));
  
  task_delay_until(&PreviousTime, MILLISECONDS_PER_TICK);
 }
}