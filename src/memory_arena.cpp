
//~ Memory arena
#define Kilobytes(Size) (1024*(Size))
#define Megabytes(Size) (1024*Kilobytes(Size))
#define Gigabytes(Size) (1024L*(u64)Megabytes(Size))

inline umw
AlignValue(umw Value, umw Alignment){
 umw Result = ((Value+(Alignment-1)) & ~(Alignment-1));
 return(Result);
}

typedef u32 memory_flags;
enum memory_flags_ {
 MemoryFlag_None       = (0 << 0),
 MemoryFlag_ZeroMemory = (1 << 1),
};

struct memory_info {
 memory_flags Flags;
 umw Alignment;
};

inline memory_info
ZeroAndAlign(umw Alignment=4){
 memory_info Result;
 Result.Flags = MemoryFlag_ZeroMemory;
 Result.Alignment = Alignment;
 return(Result);
}

inline memory_info
NoneAndAlign(umw Alignment=4){
 memory_info Result;
 Result.Flags = MemoryFlag_None;
 Result.Alignment = Alignment;
 return(Result);
}

struct memory_arena {
 u8 *Memory;
 umw Used;
 umw Size;
};

void
InitializeArena(memory_arena *Arena, void *Memory, umw Size){
 *Arena = {};
 Arena->Memory = (u8 *)Memory;
 Arena->Size = Size;
}

memory_arena
MakeArena(memory_arena *Arena, umw Size){
 Assert((Arena->Used+Size) < Arena->Size);
 memory_arena Result;
 Result.Memory = Arena->Memory+Arena->Used;
 Arena->Used += Size;
 Result.Size = Size;
 Result.Used = 0;
 ZeroMemory(Result.Memory, Result.Size);
 return(Result);
}

#define PushStruct(Arena, Type) (Type *)ArenaPush(Arena, sizeof(Type))
#define PushArray(Arena, Type, Count) (Type *)ArenaPush(Arena, sizeof(Type)*(Count))

#define PushSpecialStruct(Arena, Type, Info) (Type *)ArenaPush(Arena, sizeof(Type), Info)
#define PushSpecialArray(Arena, Type, Count, Info) (Type *)ArenaPush(Arena, sizeof(Type)*(Count), Info)


void *
ArenaPush(memory_arena *Arena, umw Size, memory_info Info=ZeroAndAlign(4)){
 Size = AlignValue(Size, Info.Alignment);
 umw UnAligned = (umw)(Arena->Memory+Arena->Used);
 u8 *Result = (u8 *)AlignValue(UnAligned, Info.Alignment);
 umw Difference = (umw)Result - UnAligned;
 Arena->Used += Size+Difference;
 Assert(Arena->Used <= Arena->Size);
 
 if(Info.Flags & MemoryFlag_ZeroMemory){
  ZeroMemory(Result, Size);
 }
 
 
 return(Result);
}

void *
ArenaResizeMemory(memory_arena *Arena, void *OldMemory, umw OldSize, umw NewSize, 
                  memory_info Info=ZeroAndAlign(4)){
 // We just forget about the old allocation, this shouldn't probably shouldn't be
 // used in arenas that are never cleared
 void *Result = ArenaPush(Arena, NewSize, Info);
 CopyMemory(Result, OldMemory, OldSize);
 
 return(Result);
}

inline char *
ArenaPushCString(memory_arena *Arena, const char *String){
 u32 Size = CStringLength(String)+1;
 char *Result = PushArray(Arena, char, Size);
 CopyCString(Result, String, Size);
 return(Result);
}

void
ArenaClear(memory_arena *Arena){
 Arena->Used = 0;
}

//~ Memory arena markers
struct memory_arena_marker {
 umw Used;
};

inline memory_arena_marker 
ArenaBeginMarker(memory_arena *Arena){
 memory_arena_marker Result = {};
 Result.Used = Arena->Used;
 return(Result);
}

inline void
ArenaEndMarker(memory_arena *Arena, memory_arena_marker *Marker){
 Assert(Arena->Used >= Marker->Used);
 Arena->Used = Marker->Used;
}

//~ Variable definitions

global memory_arena PermanentArena;
global memory_arena TransientArena;