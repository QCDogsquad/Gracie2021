
//~ PID
pidf_gains
PIDF(f64 kP, f64 kI, f64 kD, f64 kF){
 pidf_gains Result = {};
 Result.kP = kP;
 Result.kI = kI;
 Result.kD = kD;
 Result.kF = kF;
 
 return(Result);
}

pidf_controller 
MakePIDFController(f64 kP, f64 kI, f64 kD, f64 kF){
 pidf_controller Result = {};
 Result.kP = kP;
 Result.kI = kI;
 Result.kD = kD;
 Result.kF = kF;
 
 return Result;
}

pidf_controller 
MakePIDFController(pidf_gains Gains){
 pidf_controller Result = {};
 Result.Gains = Gains;
 return Result;
}

f64
UpdatePIDFController(pidf_controller *Controller, f64 Error, f64 dTime){
 f64 dError = (Error - Controller->PreviousError) / dTime;
 
 f64 Result = (Controller->kP*Error +
               Controller->kI*Controller->ErrorSum + 
               Controller->kD*dError +
               Controller->kF);
 
 Controller->PreviousError = Error;
 Controller->ErrorSum += Error*dTime;
 
 Result = Clamp(Result, -1.0f, 1.0f);
 
 return Result;
}


//~ Chassis

// TODO(Tyler): The gearset can be retreived by the motors, so this should be used to check the 
// MaxVelocity. 
void
InitializeBasicChassis(basic_chassis *Chassis, f64 MaxVelocity, u8 Left, u8 Right){
 *Chassis = {};
 Chassis->Left  = Left;
 Chassis->Right = Right;
 Chassis->MaxVelocity = MaxVelocity;
 
 motor_set_reversed(Chassis->Left,  false);
 motor_set_reversed(Chassis->Right, true);
}

void
UpdateBasicChassisSkid(basic_chassis *Chassis, f64 Forward, f64 Turn){
 Clamp(Forward, -1.0, 1.0);
 Clamp(Turn,    -1.0, 1.0);
 
 u16 ForwardU16 = (u16)(Chassis->MaxVelocity*Forward);
 u16 TurnU16    = (u16)(Chassis->MaxVelocity*Turn);
 motor_move_velocity(Chassis->Left,  ForwardU16+TurnU16);
 motor_move_velocity(Chassis->Right, ForwardU16-TurnU16);
}

//~ PIDF Chassis

void
InitializePIDFChassis(pidf_chassis *Chassis, f64 MaxVelocity, u8 Left, u8 Right, 
                      pidf_gains DistanceGains=DEFAULT_DISTANCE_GAINS, 
                      pidf_gains TurnGains=DEFAULT_DISTANCE_GAINS){
 InitializeBasicChassis(Chassis, MaxVelocity, Left, Right);
 
 Chassis->DistancePIDF = MakePIDFController(DistanceGains);
 Chassis->TurnPIDF     = MakePIDFController(TurnGains);
}

void
UpdatePIDFChassisSkid(pidf_chassis *Chassis, f64 Forward, f64 Turn, f64 dTime){
 f64 LeftPosition  = motor_get_position(Chassis->Left);
 f64 RightPosition = motor_get_position(Chassis->Right);
 
 f64 DistanceError = Forward;
 f64 DistanceOutput = UpdatePIDFController(&Chassis->DistancePIDF, DistanceError, dTime);
 
 Forward += DistanceOutput;
 
 f64 TurnError = Turn;
 Turn += UpdatePIDFController(&Chassis->TurnPIDF, TurnError, dTime);
 
 UpdateBasicChassisSkid(Chassis, Forward, Turn);
}