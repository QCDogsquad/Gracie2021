b8 ChassisBackgroundUpdate(robot_chassis *Chassis){
 b8 Result = true;
 
 if(competition_is_autonomous()){
  const f64 Epsilon = 0.01;
  
  motor_move_absolute(Chassis->Left, Chassis->LeftTarget, Chassis->AutonomousVelocity);
  {
   f64 P = motor_get_position(Chassis->Left);
   if(fabs(P-Chassis->LeftTarget) > Epsilon) Result = false;
  }
  
  motor_move_absolute(Chassis->Right, Chassis->RightTarget, Chassis->AutonomousVelocity);
  {
   f64 P = motor_get_position(Chassis->Right);
   if(fabs(P-Chassis->RightTarget) > Epsilon) Result = false;
  }
 }
 
 return Result;
}

//~ Configuration
void ChassisReverse(robot_chassis *Chassis, b8 Reversed){
 motor_set_reversed(Chassis->Left,   Reversed);
 motor_set_reversed(Chassis->Right, !Reversed);
}

void ChassisBrakeMode(robot_chassis *Chassis, motor_brake_mode_e BrakeMode){
 motor_set_brake_mode(Chassis->Left,  BrakeMode);
 motor_set_brake_mode(Chassis->Right, BrakeMode);
}

void InitChassis(robot_chassis *Chassis, 
                 u8 Left, u8 Right,
                 f64 MaxVelocity, f64 AutonomousVelocity, 
                 f64 Track, f64 WheelDiameter, 
                 f64 GearingFactor=1.0, motor_gearset_e Gearset=E_MOTOR_GEARSET_18){
 Chassis->Left  = Left;
 Chassis->Right = Right;
 Chassis->MaxVelocity = MaxVelocity;
 Chassis->AutonomousVelocity = AutonomousVelocity;
 Chassis->Track = Track;
 Chassis->WheelDiameter = WheelDiameter;
 Chassis->GearingFactor = GearingFactor;
 
 // Gearset
 // NOTE(Tyler): This is required for motor functions such as motor_move_* to work.
 // I'm not entirely sure why the motor functions don't account for this.
 switch(Gearset){
  case E_MOTOR_GEARSET_06: Chassis->GearingFactor /= 3.0; break;
  case E_MOTOR_GEARSET_18: Chassis->GearingFactor *= 1.0; break;
  case E_MOTOR_GEARSET_36: Chassis->GearingFactor *= 2.0; break;
 }
 motor_set_gearing(Chassis->Left,  Gearset);
 motor_set_gearing(Chassis->Right, Gearset);
 
 // Default to not reversed
 ChassisReverse(Chassis, false);
 
 // Encoder units
 motor_set_encoder_units(Chassis->Left,  E_MOTOR_ENCODER_ROTATIONS);
 motor_set_encoder_units(Chassis->Right, E_MOTOR_ENCODER_ROTATIONS);
}

//~ Opcontrol
// NOTE(Tyler): This function takes Forward and Turn as normalized values
// in the range [-1, 1]. 
void ChassisMoveArcade(robot_chassis *Chassis, f64 Forward, f64 Turn){
 Forward = Clamp(Forward, -1.0, 1.0);
 Turn    = Clamp(Turn, -1.0, 1.0);
 
 f64 MaxVelocity = Chassis->MaxVelocity;
 motor_move_velocity(Chassis->Left, (Forward + Turn) * MaxVelocity);
 motor_move_velocity(Chassis->Right,(Forward - Turn) * MaxVelocity);
 
#if 0 
 f64 LeftP  = motor_get_position(Chassis->Left);
 f64 RightP = motor_get_position(Chassis->Right);
 
 f64 WheelCircumference = PI*Chassis->WheelDiameter;
 f64 StraightP = 0.5*(LeftP+RightP);
 StraightP *= Chassis->GearingFactor*WheelCircumference;
 
 // TODO(Tyler): This negative here is a @HACK specifically for Gracie.
 f64 Rotation = -imu_get_rotation(Chassis->IMU);
 
 v2 FinalP = MakeV2FromPolar(StraightP, Radians(Rotation));
 
 //printf("R: %f P: %f | P: (%f, %f)\n", Rotation, StraightP, FinalP.X, FinalP.Y);
#endif
}

#if 0
// TODO(Tyler): I quite frankly don't know if this function will work properly or not
v2 ChassisIntegrateP(robot_chassis *Chassis, f64 dTime){
 imu_accel_s_t IMUddP = imu_get_accel(Chassis->IMU);
 f64 Rotation = -imu_get_rotation(Chassis->IMU);
 v2 ddP = V2(IMUddP.x, IMUddP.y);
 ddP = V2Rotate(ddP, Radians(Rotation));
 Chassis->P += (Chassis->dP*dTime + 
                0.5*ddP*Square(dTime));
 Chassis->dP += ddP*dTime;
 
 return Chassis->P;
}
#endif

//~ Autonomous functions

void ChassisBegin(robot_manager *Robot, robot_chassis *Chassis){
 // Reset
 motor_tare_position(Chassis->Left);
 motor_tare_position(Chassis->Right);
}

// NOTE(Tyler): Distance IS IN METERS!!!
void ChassisMove(robot_manager *Robot, robot_chassis *Chassis, f64 Distance){
 f64 WheelCircumference = PI*Chassis->WheelDiameter;
 f64 Rotations = Distance/(Chassis->GearingFactor*WheelCircumference);
 
 {
  f64 P = motor_get_position(Chassis->Left);
  Chassis->LeftTarget = P + Rotations;
 }
 {
  f64 P = motor_get_position(Chassis->Right);
  Chassis->RightTarget = P + Rotations;
 }
 
 RobotSubsystemNotDone(Robot, Subsystem_Chassis);
}

// NOTE(Tyler): Angle IS IN DEGREES!!!
void ChassisRotate(robot_manager *Robot, robot_chassis *Chassis, f64 Angle){
 f64 Rotations = (Angle*Chassis->Track)/(360.0*Chassis->GearingFactor*Chassis->WheelDiameter);
 
 {
  f64 P = motor_get_position(Chassis->Left);
  Chassis->LeftTarget = P + Rotations;
 }
 {
  f64 P = motor_get_position(Chassis->Right);
  Chassis->RightTarget = P - Rotations;
 }
 
 RobotSubsystemNotDone(Robot, Subsystem_Chassis);
}

void ChassisFollowTrajectory(robot_chassis *Chassis, trajectory *Trajectory){
 u32 PreviousTime = millis();
 for(u32 I=0; I<Trajectory->Points.Count; I++){
  trajectory_point *Point = &Trajectory->Points[I];
  printf("Here %u: %f %f %f!\n", I, Point->Left, Point->Right, Point->Time);
  
  motor_move_absolute(Chassis->Left,  Point->Left,  Chassis->AutonomousVelocity);
  motor_move_absolute(Chassis->Right, Point->Right, Chassis->AutonomousVelocity);
  
  task_delay(Point->Time);
  
  // TODO(Tyler): No idea, why this does not work:
  //task_delay_until(&PreviousTime, Point->Time);
 }
}