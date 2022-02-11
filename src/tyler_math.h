/* date = August 16th 2021 7:21 pm */

#ifndef TYLER_MATH_H
#define TYLER_MATH_H
inline f64
Square(f64 A)
{
 f64 Result = A*A;
 return(Result);
}

inline f64
Cube(f64 A)
{
 f64 Result = A*A*A;
 return(Result);
}

inline f64
Tesseract(f64 A)
{
 f64 Result = A*A*A*A;
 return(Result);
}

inline f64
Clamp(f64 Value, f64 Min, f64 Max){
 f64 Result = Value;
 if(Result < Min){
  Result = Min;
 }else if(Result > Max){
  Result = Max;
 }
 return(Result);
}

inline f64
Round(f64 A)
{
 f64 Result;
 if(A < 0)
 {
  Result = floor((A - 0.5f));
 }
 else
 {
  Result = floor((A + 0.5f));
 }
 return(Result);
}

inline f64
SignOf(f64 A){
 f64 Result = (A < 0) ? -1.0f : 1.0f;
 return(Result);
}

inline f64
Lerp(f64 A, f64 B, f64 T){
 T = Clamp(T, 0.0f, 1.0f);
 f64 Result = T*A + (1.0f-T)*B;
 return(Result);
}

inline s32
NormalizeDegrees(s32 D){
 s32 Result;
 if(D < 0){
  Result = D;
  while(Result < 0) Result += 360; 
 }else{
  Result = D % 360;
 }
 return(Result);
}

//~ V2s

union v2
{
 struct
 {
  f64 X;
  f64 Y;
 };
 struct
 {
  f64 Width;
  f64 Height;
 };
};

inline v2
V2(f64 X, f64 Y){ 
 v2 Result = v2{X, Y}; 
 return(Result);
}

inline v2
V2(f64 XY){ 
 v2 Result = V2(XY, XY); 
 return(Result);
}

inline v2
operator+(v2 A, v2 B)
{
 v2 Result;
 Result.X = A.X + B.X;
 Result.Y = A.Y + B.Y;
 return(Result);
}

inline v2
operator-(v2 A, v2 B)
{
 v2 Result;
 Result.X = A.X - B.X;
 Result.Y = A.Y - B.Y;
 return(Result);
}

inline v2
operator-(v2 A)
{
 v2 Result;
 Result.X = -A.X;
 Result.Y = -A.Y;
 return(Result);
}

inline v2
operator*(v2 A, f64 B)
{
 v2 Result;
 Result.X = A.X * B;
 Result.Y = A.Y * B;
 return(Result);
}

inline v2
operator*(f64 B, v2 A)
{
 v2 Result;
 Result.X = A.X * B;
 Result.Y = A.Y * B;
 return(Result);
}

inline v2
operator/(v2 A, f64 B)
{
 v2 Result;
 Result.X = A.X / B;
 Result.Y = A.Y / B;
 return(Result);
}

inline v2
operator+=(v2 &A, v2 B)
{
 A = A + B;
 return(A);
}

inline v2
operator-=(v2 &A, v2 B)
{
 A = A - B;
 return(A);
}

inline v2
operator*=(v2 &A, f64 B)
{
 A = B * A;
 return(A);
}

inline v2
operator/=(v2 &A, f64 B)
{
 A = A / B;
 return(A);
}

inline f64
V2Dot(v2 A, v2 B) {
 f64 Result = (A.X*B.X)+(A.Y*B.Y);
 return(Result);
}

inline v2
V2Hadamard(v2 A, v2 B){
 v2 Result;
 Result.X = A.X*B.X;
 Result.Y = A.Y*B.Y;
 return(Result);
}

inline f64
V2LengthSquared(v2 V){
 f64 Result = V2Dot(V, V);
 return(Result);
}

inline f64
V2Length(v2 V){
 f64 Result = sqrt(V2LengthSquared(V));
 return(Result);
}

inline v2
V2Normalize(v2 V){
 f64 l = V2Length(V);
 v2 Result = {};
 if(l > 0.0f){
  Result = V/l;
 }
 return(Result);
}

// Perpendicular to A in the direction of B
inline v2 
V2TripleProduct(v2 A, v2 B){
 // A cross B cross A = (A cross B) cross A
 f64 Z = (A.X*B.Y)-(A.Y*B.X);
 v2 Result = V2(-Z*A.Y, Z*A.X);
 return(Result);
}

inline v2
V2Minimum(v2 A, v2 B){
 v2 Result;
 Result.X = Minimum(A.X, B.X);
 Result.Y = Minimum(A.Y, B.Y);
 
 return(Result);
}

inline v2
V2Maximum(v2 A, v2 B){
 v2 Result;
 Result.X = Maximum(A.X, B.X);
 Result.Y = Maximum(A.Y, B.Y);
 
 return(Result);
}

v2
V2Square(v2 V){
 v2 Result;
 Result.X = V.X*V.X;
 Result.Y = V.Y*V.Y;
 return(Result);
}

inline v2
V2Floor(v2 V){
 v2 Result;
 Result.X = floor(V.X);
 Result.Y = floor(V.Y);
 return(Result);
}

inline v2
V2Round(v2 V){
 v2 Result;
 Result.X = round(V.X);
 Result.Y = round(V.Y);
 return(Result);
}

inline v2
V2Ceil(v2 V){
 v2 Result;
 Result.X = ceil(V.X);
 Result.Y = ceil(V.Y);
 return(Result);
}

inline v2
V2Lerp(v2 A, v2 B, f64 T){
 T = Clamp(T, 0.0f, 1.0f);
 v2 Result = T*A + (1.0f-T)*B;
 return(Result);
}
#endif //TYLER_MATH_H
