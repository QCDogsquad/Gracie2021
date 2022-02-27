
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
  case E_MOTOR_GEARSET_06: Chassis->GearingFactor *= 3.0; break;
  case E_MOTOR_GEARSET_18: Chassis->GearingFactor *= 1.0; break;
  case E_MOTOR_GEARSET_36: Chassis->GearingFactor *= 0.5; break;
 }
 motor_set_gearing(Chassis->Left,  Gearset);
 motor_set_gearing(Chassis->Right, Gearset);
 
 // Default to not reversed
 ChassisReverse(Chassis, false);
 
 // Encoder units
 motor_set_encoder_units(Chassis->Left,  E_MOTOR_ENCODER_ROTATIONS);
 motor_set_encoder_units(Chassis->Right, E_MOTOR_ENCODER_ROTATIONS);
}

//~ 

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
// TODO(Tyler): These functions are  not suitable for use in OP control, 
// since they cannot be interrupted.

// NOTE(Tyler): Distance IS IN METERS!!!
void ChassisMoveAndWait(robot_chassis *Chassis, f64 Distance){
 f64 WheelCircumference = PI*Chassis->WheelDiameter;
 f64 Rotations = Distance/(Chassis->GearingFactor*WheelCircumference);
 
 f64 StartP = 0.5*(motor_get_position(Chassis->Left)+motor_get_position(Chassis->Right));
 
 motor_move_relative(Chassis->Left,  Rotations, Chassis->AutonomousVelocity);
 motor_move_relative(Chassis->Right, Rotations, Chassis->AutonomousVelocity);
 
 const f64 Epsilon = 0.01;
 
 while(true){
  f64 AverageP = 0.5*(motor_get_position(Chassis->Left)+motor_get_position(Chassis->Right));
  if(fabs(AverageP-StartP-Rotations) <= Epsilon) break;
  delay(1);
 }
}

// NOTE(Tyler): Angle IS IN DEGREES!!!
void ChassisRotateAndWait(robot_chassis *Chassis, f64 Angle){
 f64 Rotations = (Angle*Chassis->Track)/(360.0*Chassis->GearingFactor*Chassis->WheelDiameter);
 f64 StartP = 0.5*(motor_get_position(Chassis->Left)-motor_get_position(Chassis->Right));
 
 motor_move_relative(Chassis->Left,   Rotations, Chassis->AutonomousVelocity);
 motor_move_relative(Chassis->Right, -Rotations, Chassis->AutonomousVelocity);
 
 const f64 Epsilon = 0.01;
 
 while(true){
  f64 AverageP = 0.5*(motor_get_position(Chassis->Left)-motor_get_position(Chassis->Right));
  if(fabs(AverageP-StartP-Rotations) <= Epsilon) break;
  delay(1);
 }
}