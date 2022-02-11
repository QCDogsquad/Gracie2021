
void InitChassis(basic_chassis *Chassis, 
                 u8 Left, u8 Right, u8 IMU,
                 f64 MaxVelocity, f64 AutonomousVelocity, 
                 f64 Track, f64 WheelDiameter, f64 GearingFactor=1.0){
 Chassis->Left  = Left;
 Chassis->Right = Right;
 Chassis->IMU   = IMU;
 Chassis->MaxVelocity = MaxVelocity;
 Chassis->AutonomousVelocity = AutonomousVelocity;
 Chassis->Track = Track;
 Chassis->WheelDiameter = WheelDiameter;
 Chassis->WheelDiameter = WheelDiameter;
 Chassis->GearingFactor = GearingFactor;
 
 // TODO(Tyler): I don't know how we want to deal with determining which to reverse
 motor_set_reversed(Chassis->Left, true);
 motor_set_reversed(Chassis->Right, false);
 
 motor_set_encoder_units(Chassis->Left,  E_MOTOR_ENCODER_ROTATIONS);
 motor_set_encoder_units(Chassis->Right, E_MOTOR_ENCODER_ROTATIONS);
 
 // TODO(Tyler): We probably don't want this, but it might be good for autonomous
 motor_set_brake_mode(Chassis->Left,  E_MOTOR_BRAKE_BRAKE);
 motor_set_brake_mode(Chassis->Right, E_MOTOR_BRAKE_BRAKE);
 
 imu_reset(Chassis->IMU);
}

// NOTE(Tyler): This function takes Forward and Turn as normalized values
// in the range [-1, 1]. 
void ChassisMoveArcade(basic_chassis *Chassis, f64 Forward, f64 Turn){
 Forward = Clamp(Forward, -1.0, 1.0);
 Turn    = Clamp(Turn, -1.0, 1.0);
 
 f64 MaxVelocity = Chassis->MaxVelocity;
 motor_move_velocity(Chassis->Left, (Forward + Turn) * MaxVelocity);
 motor_move_velocity(Chassis->Right,(Forward - Turn) * MaxVelocity);
 
 f64 LeftP  = motor_get_position(Chassis->Left);
 f64 RightP = motor_get_position(Chassis->Right);
 
 f64 WheelCircumference = PI*Chassis->WheelDiameter;
 f64 StraightP = 0.5*(LeftP+RightP);
 StraightP *= Chassis->GearingFactor*WheelCircumference;
 
 // TODO(Tyler): This negative here is a @HACK specifically for Gracie.
 f64 Rotation = -imu_get_rotation(Chassis->IMU);
 v2 FinalP = MakeV2FromPolar(StraightP, Radians(Rotation));
 
 printf("R: %f P: %f | P: (%f, %f)\n", Rotation, StraightP, FinalP.X, FinalP.Y);
}

// TODO(Tyler): I quite frankly don't know if this function will work properly or not
v2 ChassisIntegrateP(basic_chassis *Chassis, f64 dTime){
 imu_accel_s_t IMUddP = imu_get_accel(Chassis->IMU);
 f64 Rotation = -imu_get_rotation(Chassis->IMU);
 v2 ddP = V2(IMUddP.x, IMUddP.y);
 ddP = V2Rotate(ddP, Radians(Rotation));
 Chassis->P += (Chassis->dP*dTime + 
                0.5*ddP*Square(dTime));
 Chassis->dP += ddP*dTime;
 
 return Chassis->P;
}

//~ Autonomous functions
// NOTE(Tyler): These functions are currently not suitable for use in OP control

void ChassisMoveForwardAndWait(basic_chassis *Chassis, f64 Distance){
 f64 Rotations = Distance;
 f64 WheelCircumference = PI*Chassis->WheelDiameter;
 Rotations /= WheelCircumference;
 Rotations /= Chassis->GearingFactor;
 
 f64 StartP = 0.5*(motor_get_position(Chassis->Left)+motor_get_position(Chassis->Right));
 
 motor_move_relative(Chassis->Left,  Rotations, Chassis->AutonomousVelocity);
 motor_move_relative(Chassis->Right, Rotations, Chassis->AutonomousVelocity);
 
 // TODO(Tyler): I'm not sure if it is correct to multiply here, or if we would want to divide.
 // Or if we want to do any sort of operation with GearingFactor, or if we want to use 
 // WheelCircumference as well.
 const f64 Epsilon = 0.001;
 
 while(true){
  f64 AverageP = 0.5*(motor_get_position(Chassis->Left)+motor_get_position(Chassis->Right));
  if(fabs(AverageP-StartP-Rotations) <= Epsilon) break;
  delay(1);
 }
}

// NOTE(Tyler): Angle IS IN DEGREES!!!
void ChassisRotateAndWait(basic_chassis *Chassis, f64 Angle){
 f64 Rotations = (Angle*Chassis->Track)/(360.0*Chassis->GearingFactor*Chassis->WheelDiameter);
 f64 StartP = 0.5*(motor_get_position(Chassis->Left)-motor_get_position(Chassis->Right));
 
 motor_move_relative(Chassis->Left,   Rotations, Chassis->AutonomousVelocity);
 motor_move_relative(Chassis->Right, -Rotations, Chassis->AutonomousVelocity);
 
 const f64 Epsilon = 0.0001;
 
 // TODO(Tyler): I do not know if this is correct. This must be tested.
 while(true){
  f64 AverageP = 0.5*(motor_get_position(Chassis->Left)-motor_get_position(Chassis->Right));
  if(fabs(AverageP-StartP-Rotations) <= Epsilon) break;
  delay(1);
 }
}