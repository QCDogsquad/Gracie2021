b8 LiftBackgroundUpdate(lift_data *Lift){
 b8 Result = true;
 
 const f64 Epsilon = 1.0;
 u8 Left = LEFT_LIFT_MOTOR;
 u8 Right = RIGHT_LIFT_MOTOR;
 
 switch(Lift->State){
  case LiftState_Raised: {
   motor_move_absolute(LEFT_LIFT_MOTOR,  0, LIFT_VELOCITY);
   motor_move_absolute(RIGHT_LIFT_MOTOR, 0, LIFT_VELOCITY);
   
   Result = Motor2xIsAtPosition(Left, Right, 0, Epsilon);
  }break;
  case LiftState_Partway: {
   motor_move_absolute(LEFT_LIFT_MOTOR,  Lift->Movement-LIFT_PARTWAY_OFFSET, LIFT_VELOCITY);
   motor_move_absolute(RIGHT_LIFT_MOTOR, Lift->Movement-LIFT_PARTWAY_OFFSET, LIFT_VELOCITY);
   
   f64 Target = Lift->Movement-LIFT_PARTWAY_OFFSET;
   Result = Motor2xIsAtPosition(Left, Right, Target, Epsilon);
  }break;
  case LiftState_Fullway: {
   if(adi_digital_read(LIFT_LIMIT)){
    Result = true;
   }else{
    motor_move_velocity(LEFT_LIFT_MOTOR, LIFT_VELOCITY);
    motor_move_velocity(RIGHT_LIFT_MOTOR, LIFT_VELOCITY);
    Result = false;
   }
  }break;
  case LiftState_Free: break;
 }
 
 if(adi_digital_read(LIFT_LIMIT)){
  f64 Position = motor_get_position(LEFT_LIFT_MOTOR);
  if((motor_get_target_position(LEFT_LIFT_MOTOR) > Position) ||
     (motor_get_actual_velocity(LEFT_LIFT_MOTOR) > 0)){
   //if(Lift->Movement < Position) Lift->Movement = Position;
   motor_move_velocity(LEFT_LIFT_MOTOR,  0);
   motor_move_velocity(RIGHT_LIFT_MOTOR, 0);
   
   Result = true;
  }
 }
 
 Lift->Done = Result;
 return Result;
}

//~ Fullway
void LiftLower(robot_manager *Robot, lift_data *Lift){
 Lift->State = LiftState_Fullway;
 RobotSubsystemNotDone(Robot, Subsystem_Lift);
}

void LiftRaise(robot_manager *Robot, lift_data *Lift){
 Lift->State = LiftState_Raised;
 RobotSubsystemNotDone(Robot, Subsystem_Lift);
}

void LiftPartway(robot_manager *Robot, lift_data *Lift){
 Lift->State = LiftState_Partway;
 RobotSubsystemNotDone(Robot, Subsystem_Lift);
}

//~ Initialize
void InitLift(lift_data *Lift){
 // Motor setup
 motor_set_gearing(LEFT_LIFT_MOTOR, E_MOTOR_GEARSET_36);
 motor_set_encoder_units(LEFT_LIFT_MOTOR,   E_MOTOR_ENCODER_DEGREES);
 motor_set_brake_mode(LEFT_LIFT_MOTOR,  E_MOTOR_BRAKE_HOLD);
 motor_tare_position(LEFT_LIFT_MOTOR);
 motor_set_reversed(LEFT_LIFT_MOTOR, false);
 
 motor_set_gearing(RIGHT_LIFT_MOTOR, E_MOTOR_GEARSET_36);
 motor_set_encoder_units(RIGHT_LIFT_MOTOR,   E_MOTOR_ENCODER_DEGREES);
 motor_set_brake_mode(RIGHT_LIFT_MOTOR,  E_MOTOR_BRAKE_HOLD);
 motor_tare_position(RIGHT_LIFT_MOTOR);
 motor_set_reversed(RIGHT_LIFT_MOTOR, true);
}