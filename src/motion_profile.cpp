
//~ 1D motion profiles
void 
CalculatePositionMotionProfile1D(motion_profile_1d *Profile, 
                                 f64 TargetPosition, f64 Jerk, 
                                 f64 MaxAcceleration_, f64 MaxVelocity_) {
 *Profile = {};
 Profile->Type = MotionProfile_Position;
 
 if(TargetPosition < 0) Profile->Jerk = -Jerk; 
 else                   Profile->Jerk =  Jerk;
 TargetPosition = fabs(TargetPosition);
 
 f64 tz = Minimum(MaxAcceleration_/Jerk, sqrt((MaxVelocity_)/Jerk));
 f64 ty = (MaxVelocity_ - Jerk*Square(tz)) / (Jerk*tz);
 
 f64 Initial_tr = cbrt(TargetPosition/(2*Jerk));
 if(Initial_tr > tz){
  Profile->tr = tz;
  
  f64 b = Jerk*Square(tz);
  f64 c = -TargetPosition;
  f64 Initial_th = (-b + sqrt(Square(b) - 4*Jerk*tz*c))/(2*Jerk*tz) - tz;
  
  if(Initial_th > ty){
   Profile->th = ty;
   Profile->tw = (TargetPosition - 1.5*Jerk*Cube(tz) - 1.5*Jerk*Square(tz)*ty) / (0.5*Jerk*Square(tz) + Jerk*tz*ty) - tz - ty;
  }else{
   Profile->th = Initial_th;
   Profile->tw = 0.0;
  }
 }else{
  Profile->tr = Initial_tr;
  Profile->th = 0.0;
  Profile->tw = 0.0;
 }
}

void 
CalculateVelocityMotionProfile1D(motion_profile_1d *Profile,
                                 f64 TargetVelocity, f64 Jerk, f64 MaxAcceleration_) {
 *Profile = {};
 Profile->Type = MotionProfile_Velocity;
 
 if(TargetVelocity < 0) Profile->Jerk = -Jerk; 
 else                   Profile->Jerk =  Jerk;
 TargetVelocity = fabs(TargetVelocity);
 
 f64 tz = MaxAcceleration_/Jerk;
 f64 Initial_tr = sqrt(TargetVelocity/Jerk);
 if(Initial_tr > tz){
  Profile->tr = tz;
  Profile->th = TargetVelocity/(Jerk*tz) - tz;
 }else{
  Profile->tr = Initial_tr;
  Profile->th = 0.0;
 }
}

//~ 2D motion profiles
void 
CalculatePositionMotionProfile2D(motion_profile_2d *Profile, v2 Target, f32 Jerk, 
                                 f64 MaxAcceleration_, f64 MaxVelocity_){
 *Profile = {};
 
 f64 smin;
 f64 smax;
 motion_profile_1d *MotionProfileMin;
 motion_profile_1d *MotionProfileMax;
 if(Target.X < Target.Y){
  smin = Target.X;
  smax = Target.Y;
  MotionProfileMin = &Profile->X;
  MotionProfileMax = &Profile->Y;
 }else if(Target.Y < Target.X){
  smin = Target.Y;
  smax = Target.X;
  MotionProfileMin = &Profile->Y;
  MotionProfileMax = &Profile->X;
 }else{
  smin = Target.X;
  smax = Target.Y;
  MotionProfileMin = &Profile->X;
  MotionProfileMax = &Profile->Y;
 }
 
 CalculatePositionMotionProfile1D(MotionProfileMin, smin, Jerk, MaxAcceleration_, MaxVelocity_);
 f64 JerkMaxSide = smax / (Square(MotionProfileMin->tr)*MotionProfileMin->tw + 
                           MotionProfileMin->tr*MotionProfileMin->th*MotionProfileMin->tw +
                           2*Cube(MotionProfileMin->tr) + 
                           3*Square(MotionProfileMin->tr)*MotionProfileMin->th +  
                           MotionProfileMin->tr*Square(MotionProfileMin->th));
 *MotionProfileMax = *MotionProfileMin;
 MotionProfileMax->Jerk = JerkMaxSide;
}