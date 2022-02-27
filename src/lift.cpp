void LiftTask(void *Data){
 while(true){
  switch(LiftData.State){
   case LiftState_Raised: {
    motor_move_absolute(LEFT_LIFT_MOTOR,  0, LIFT_VELOCITY);
    motor_move_absolute(RIGHT_LIFT_MOTOR, 0, LIFT_VELOCITY);
   }break;
   case LiftState_Partway: {
    motor_move_absolute(LEFT_LIFT_MOTOR,  LiftData.Movement-LIFT_PARTWAY_OFFSET, LIFT_VELOCITY);
    motor_move_absolute(RIGHT_LIFT_MOTOR, LiftData.Movement-LIFT_PARTWAY_OFFSET, LIFT_VELOCITY);
   }break;
   case LiftState_Fullway: {
    if(!adi_digital_read(LIFT_LIMIT)){
     motor_move_velocity(LEFT_LIFT_MOTOR, LIFT_VELOCITY);
     motor_move_velocity(RIGHT_LIFT_MOTOR, LIFT_VELOCITY);
#if 0
     motor_move_absolute(LEFT_LIFT_MOTOR,  LiftData.Movement, LIFT_VELOCITY);
     motor_move_absolute(RIGHT_LIFT_MOTOR, LiftData.Movement, LIFT_VELOCITY);
#endif
    }
   }break;
   case LiftState_Free: break;
  }
  
  if(adi_digital_read(LIFT_LIMIT)){
   f64 Position = motor_get_position(LEFT_LIFT_MOTOR);
   if((motor_get_target_position(LEFT_LIFT_MOTOR) > Position) ||
      (motor_get_actual_velocity(LEFT_LIFT_MOTOR) > 0)){
    if(LiftData.Movement < Position) LiftData.Movement = Position;
    motor_move_velocity(LEFT_LIFT_MOTOR,  0);
    motor_move_velocity(RIGHT_LIFT_MOTOR, 0);
   }
  }
  
  // Doesn't need to be uber fast
  delay(20);
 }
}

//~ Fullway
void LowerLift(){
 LiftData.State = LiftState_Fullway;
}

void RaiseLift(){
 LiftData.State = LiftState_Raised;
}

void LowerLiftAndWait(){
 LiftData.State = LiftState_Fullway;
 
 u8 Left = LEFT_LIFT_MOTOR;
 u8 Right = RIGHT_LIFT_MOTOR;
 
 const f64 Epsilon = 1.0;
 
 while(true){
  f64 AverageP = 0.5*(motor_get_position(Left)+motor_get_position(Right));
  if(fabs(AverageP-LiftData.Movement) <= Epsilon) break;
  if(adi_digital_read(LIFT_LIMIT)) break;
  delay(1);
 }
}

void RaiseLiftAndWait(){
 LiftData.State = LiftState_Raised;
 
 u8 Left = LEFT_LIFT_MOTOR;
 u8 Right = RIGHT_LIFT_MOTOR;
 
 const f64 Epsilon = 1.0;
 
 while(true){
  f64 AverageP = 0.5*(motor_get_position(Left)+motor_get_position(Right));
  if(fabs(AverageP) <= Epsilon) break;
  delay(1);
 }
}

//~ Partway
void LowerLiftPartway(){
 LiftData.State = LiftState_Partway;
}

void LowerLiftPartwayAndWait(){
 LiftData.State = LiftState_Partway;
 
 f64 Target = LiftData.Movement-LIFT_PARTWAY_OFFSET;
 
 u8 Left = LEFT_LIFT_MOTOR;
 u8 Right = RIGHT_LIFT_MOTOR;
 
 const f64 Epsilon = 1.0;
 
 while(true){
  f64 AverageP = 0.5*(motor_get_position(Left)+motor_get_position(Right));
  if(fabs(AverageP-Target) <= Epsilon) break;
  delay(1);
 }
}

//~ Initialize
void InitLift(){
 task_create(LiftTask, 0, 
             TASK_PRIORITY_DEFAULT-2, TASK_STACK_DEPTH_DEFAULT, 
             "LiftTask");
 
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