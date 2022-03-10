struct trajectory_point {
 f64 Left;
 f64 Right;
 f64 Time;
};

struct trajectory {
 array<trajectory_point> Points;
};

struct b_spline_path {
 array<v2>  Points;
 array<f64> Knots;
};

//~ Path
b_spline_path MakeBSplinePath(memory_arena *Arena, v2 *Points, u32 Count){
 b_spline_path Result = {};
 Result.Points = MakeArray(Points, Count);
 u32 KnotCount = Count+4;
 Result.Knots = MakeArray<f64>(Arena, KnotCount);
 return Result;
}

b_spline_path MakeBSplinePath(memory_arena *Arena, u32 MaxCount){
 b_spline_path Result = {};
 Result.Points = MakeArray<v2>(Arena, MaxCount);
 u32 KnotCount = MaxCount+4;
 Result.Knots = MakeArray<f64>(Arena, KnotCount);
 return Result;
}

void PathAddPoint(b_spline_path *Path, v2 P){
 ArrayAdd(&Path->Points, P);
}

//~ B-Spline math

void CubicBSplineGenerateKnots(b_spline_path *Path){
 u32 KnotCount = Path->Points.Count+4;
 u32 Segments  = Path->Points.Count-3;
 
 ArrayAdd(&Path->Knots, 0.0);
 ArrayAdd(&Path->Knots, 0.0);
 ArrayAdd(&Path->Knots, 0.0);
 ArrayAdd(&Path->Knots, 0.0);
 
 for(u32 I=1; I<Segments; I++){
  ArrayAdd(&Path->Knots, (f64)I / (f64)Segments);
 }
 
 ArrayAdd(&Path->Knots, 1.0);
 ArrayAdd(&Path->Knots, 1.0);
 ArrayAdd(&Path->Knots, 1.0);
 ArrayAdd(&Path->Knots, 1.0);
}

v2 CubicBSplineInterpolate(memory_arena *Arena, b_spline_path *Path, f64 T, v2 *D){
 T = Clamp(T, 0.0, 1.0);
 
 array<f64> &Knots = Path->Knots;
 u32 Count = Path->Points.Count;
 Assert(Count >= 4);
 
 u32 K;
 for(K=3; K<Knots.Count-4; K++){
  if((Knots[K] <= T) && (T <= Knots[K+1])){
   break;
  }
 }
 
 for(u32 J=0; J < 4; J++){
  D[J] = Path->Points[J + K - 3];
 }
 
 for(u32 R=1; R < 4; R++){
  for(u32 J=3; J > R-1; J--){
   f64 Alpha = (T - Knots[J+K-3]) / (Knots[J+1+K-R] - Knots[J+K-3]);
   D[J] = (1.0 - Alpha)*D[J-1] + Alpha*D[J];
   //printf("Alpha: %f\n", Alpha);
  }
 }
 
 v2 Result = D[3];
 
 return Result;
}

//~ Trajectory

trajectory MakeTrajectory(memory_arena *Arena, u32 MaxCount){
 trajectory Result = {};
 Result.Points = MakeArray<trajectory_point>(Arena, MaxCount);
 return Result;
}

void PathCalculate(memory_arena *Arena, robot_chassis *Chassis, b_spline_path *Path, trajectory *Trajectory){
 CubicBSplineGenerateKnots(Path);
 
 v2 *D = PushArray(Arena, v2, 4);
 
 v2 Direction = V2(0, 1);
 v2 P = CubicBSplineInterpolate(Arena, Path, 0.0, D);
 ArrayAdd(&Trajectory->Points, {});
 
 f64 Step = 1.0 / (f64)Trajectory->Points.MaxCount;
 f64 T = Step;
 for(u32 I=1; I<Trajectory->Points.MaxCount; I++){
  v2 NewP = CubicBSplineInterpolate(Arena, Path, T, D);
  v2 dP = NewP-P;
  v2 NewDirection = dP;
  
  f64 Determinant = Direction.X*NewDirection.Y - Direction.Y*NewDirection.X;
  f64 Angle = -atan2(Determinant, V2Dot(Direction, NewDirection)); 
  Angle = Degrees(Angle);
  printf("(%f, %f), %f\n", NewP.X, NewP.Y, Angle);
  
  f64 Rotations = (Angle*Chassis->Track)/(360.0*Chassis->GearingFactor*Chassis->WheelDiameter);
  trajectory_point Previous = Trajectory->Points[I-1];
  trajectory_point Point = {};
  Point.Left = Previous.Left+Rotations;
  Point.Right = Previous.Right-Rotations;
  Point.Time = 10;
  ArrayAdd(&Trajectory->Points, Point);
  
  P = NewP;
  Direction = NewDirection;
  T += Step;
 }
 
 printf("Ended!\n");
}