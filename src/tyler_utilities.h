#if !defined(TYLER_UTILITIES_H)
#define TYLER_UTILITIES_H

//~ Primitive types

#include <stddef.h>
#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef s8  b8;
typedef s16 b16;
typedef s32 b32;
typedef s64 b64;

typedef uintptr_t rawptr;

#define U8_MAX  0xff
#define U16_MAX 0xffff
#define U32_MAX 0xffffffff
#define U64_MAX 0xffffffffffffffff

#define S8_MAX  0x7f
#define S16_MAX 0x7fff
#define S32_MAX 0x7fffffff
#define S64_MAX 0x7fffffffffffffff

#define S8_MIN  0x80
#define S16_MIN 0x8000
#define S32_MIN 0x80000000
#define S64_MIN 0x8000000000000000

typedef size_t memory_index;

typedef float  f32;
typedef double f64;

#define internal        static
#define global          static
#define global_constant static const
#define local_persist   static
#define local_constant  static const

//~ Platform interface
struct memory_arena;
struct file_info
{
 memory_index Size;
 void *Data;
};

file_info PlatformReadEntireFile(const char *Path);

void FreeFileInfo(memory_arena *Arena, file_info File);

//~ Helpers
#define ArrayCount(Array) (sizeof(Array)/sizeof(*Array))
#define Kilobytes(Size) (1024*(Size))
#define Megabytes(Size) (1024*Kilobytes(Size))
#define Gigabytes(Size) (1024L*(u64)Megabytes(Size))

#if defined(_MSC_VER)
#define Assert(Expr) do {if (!(Expr)) __debugbreak();} while(0)
#else
#include <assert.h>
#define Assert(Expr) assert(Expr)
#endif

//~ Functions
internal inline f64
Clamp(f64 Value, f64 Min, f64 Max){
 f64 Result = Value;
 if(Result < Min){
  Result = Min;
 }else if(Result > Max){
  Result = Max;
 }
 return(Result);
}


#endif //TYLER_UTILITIES_H
