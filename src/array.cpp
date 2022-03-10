
//~ Array
template<typename T>
struct array {
 T *Items;
 u32 Count;
 u32 MaxCount;
 
 inline T &operator[](s64 Index){
  Assert(Index < Count);
  return(Items[Index]);
 }
 
 inline operator b8(){  return(Items != 0); }
 inline operator b16(){ return(Items != 0); }
 inline operator b32(){ return(Items != 0); }
 inline operator b64(){ return(Items != 0); }
};

template<typename T> inline array<T>
MakeArray(memory_arena *Arena, u32 MaxCount){
 array<T> Result = {};
 Result.Items = PushArray(Arena, T, MaxCount);
 Result.MaxCount = MaxCount;
 return(Result);
}

template<typename T> inline array<T> 
MakeArray(T *Items, u32 Count){
 array<T> Result = {};
 Result.Items = Items;
 Result.Count = Count;
 Result.MaxCount = Count;
 return Result;
}

template<typename T> inline array<T>
MakeFullArray(memory_arena *Arena, u32 Count, umw Alignment=4){
 array<T> Result = {};
 Result.Items = PushSpecialArray(Arena, T, Count, ZeroAndAlign(Alignment));
 Result.Count = Count;
 Result.MaxCount = Count;
 return(Result);
}

template<typename T>  inline T
ArrayGet(array<T> *Array, s64 Index){
 Assert(Index < Array->Count);
 return(Array->Items[Index]);
}

template<typename T>  inline void
ArrayClear(array<T> *Array){
 Array->Count = 0;
}

template<typename T>  inline void
ArrayAdd(array<T> *Array, T Item){
 if(Array->Count+1 <= Array->MaxCount){
  Array->Items[Array->Count++] = Item;
 }else{
  Assert(0);
 }
}

template<typename T>  inline T *
ArrayAlloc(array<T> *Array, u32 N=1){
 T *Result = 0;
 if(Array->Count+N <= Array->MaxCount){
  Result = &Array->Items[Array->Count];
  Array->Count += N;
 }else{
  Assert(0);
 }
 *Result = {};
 return(Result);
}

// A better insert might be better,
// following the same logic as ordered and unordered remove 
template<typename T> void
ArrayInsert(array<T> *Array, u32 Index, T Item){
 Assert(Index <= Array->Count);
 MoveMemory(&Array->Items[Index+1], 
            &Array->Items[Index], 
            (Array->Count-Index)*sizeof(T));
 Array->Items[Index] = Item;
 Array->Count++;
}

template<typename T>  inline T *
ArrayInsertAlloc(array<T> *Array, u32 Index){
 MoveMemory(&Array->Items[Index+1], 
            &Array->Items[Index], 
            (Array->Count-Index)*sizeof(T));
 T *NewItem = &Array->Items[Index];
 Array->Count++;
 return(NewItem);
}

template<typename T>  inline void
ArrayOrderedRemove(array<T> *Array, u32 Index){
 MoveMemory(&Array->Items[Index], 
            &Array->Items[Index+1], 
            (Array->Count-Index)*sizeof(T));
 Array->Count--;
}

template<typename T>  inline void
ArrayUnorderedRemove(array<T> *Array, u32 Index){
 Array->Items[Index] = Array->Items[Array->Count-1];
 Array->Count--;
}

template<typename T>  inline void
ArraySwap(array<T> Array, u32 IndexA, u32 IndexB){
 T Temp = Array[IndexA];
 Array[IndexA] = Array[IndexB];
 Array[IndexB] = Temp;
}

