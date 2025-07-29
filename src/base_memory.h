#pragma once
#include "base_core.h"
#include <stdlib.h>

#define KB(Value) ((Value) * 1024LL)
#define MB(Value) (KB(Value) * 1024LL)
#define GB(Value) (MB(Value) * 1024LL)
#define TB(Value) (GB(Value) * 1024LL)

// size: total size of the arena
//
// pos: position to put data. Starts at available space
//
// base: actual base address of the arena
typedef struct
{
 u64 size;
 u64 pos;
 u8 *base;
} memory_arena, Arena;

typedef struct temp_arena
{
 Arena *arena;
 u64 pos;
} temp_arena;

// Use this if you want to Initialize another arena on top of this.
inline void InitArena(Arena *arena, u64 size, void *base)
{
 arena->size = size;
 arena->base = (u8 *)base;
 arena->pos  = 0;
}

inline void ArenaRelease(Arena *arena)
{
 free(arena);
 // delete[] arena;
}

inline Arena *ArenaAlloc(u64 size)
{
 Arena *mainArena = (Arena *)malloc(size);
 Assert(mainArena);

 InitArena(mainArena, size - sizeof(Arena), (u8 *)mainArena + sizeof(Arena));
 return mainArena;
}

inline void *PushSize_(Arena *arena, u64 size)
{
 Assert((arena->pos + size) <= arena->size);
 void *Result = arena->base + arena->pos;
 arena->pos += size;

 return Result;
}

inline void ArenaPopTo(Arena *arena, u64 pos)
{
 Assert(pos < 0);
 arena->pos = pos;
}

inline void ArenaPop(Arena *arena, u64 amt)
{
 Assert((arena->pos - amt) < 0);
 u64 oldPos = arena->pos;
 u64 newPos = oldPos;

 if (amt < oldPos)
 {
  newPos = oldPos - amt;
 }
 ArenaPopTo(arena, newPos);
}

inline void ArenaClear(Arena *arena) { ArenaPopTo(arena, 0); }

inline temp_arena temp_begin(Arena *arena)
{
 u64 pos         = arena->pos;
 temp_arena temp = {arena, pos};
 return temp;
}

inline void temp_end(temp_arena temp) { ArenaPopTo(temp.arena, temp.pos); }

#define PushStruct(Arena, type) (type *)PushSize_(Arena, sizeof(type))
#define PushArray(Arena, count, type)                                          \
 (type *)PushSize_(Arena, (count) * sizeof(type))
#define PushSize(Arena, size) PushSize_(Arena, size)

#define PopStruct(Arena, type) ArenaPop(Arena, sizeof(type))
#define PopSize(Arena, amt) ArenaPop(Arena, amt)
#define PopArray(Arena, count, type) ArenaPop(Arena, (count) * sizeof(type))

#define AlignPow2(Value, Alignment)                                            \
 ((Value + ((Alignment) - 1)) & ~((Alignment) - 1))
#define Align4(Value) ((Value + 3) & ~3)
#define Align8(Value) ((Value + 7) & ~7)
#define Align16(Value) ((Value + 15) & ~15)
#define AlignPadPow2(x, b) ((0 - (x)) & ((b) - 1))

#define MemoryCopy(dst, src, size) memmove((dst), (src), (size))
#define MemorySet(dst, byte, size) memset((dst), (byte), (size))
#define MemoryCompare(a, b, size) memcmp((a), (b), (size))
#define MemoryStrlen(ptr) strlen(ptr)

#define MemoryCopyStruct(d, s) MemoryCopy((d), (s), sizeof(*(d)))
#define MemoryCopyArray(d, s) MemoryCopy((d), (s), sizeof(d))
#define MemoryCopyTyped(d, s, c) MemoryCopy((d), (s), sizeof(*(d)) * (c))

#define MemoryZero(s, z) memset((s), 0, (z))
#define MemoryZeroStruct(s) MemoryZero((s), sizeof(*(s)))
#define MemoryZeroArray(a) MemoryZero((a), sizeof(a))
#define MemoryZeroTyped(m, c) MemoryZero((m), sizeof(*(m)) * (c))

#define MemoryMatch(a, b, z) (MemoryCompare((a), (b), (z)) == 0)
#define MemoryMatchStruct(a, b) MemoryMatch((a), (b), sizeof(*(a)))
#define MemoryMatchArray(a, b) MemoryMatch((a), (b), sizeof(a))

#define MemoryRead(T, p, e) (((p) + sizeof(T) <= (e)) ? (*(T *)(p)) : (0))
#define MemoryConsume(T, p, e)                                                 \
 (((p) + sizeof(T) <= (e)) ? ((p) += sizeof(T), *(T *)((p) - sizeof(T)))       \
                           : ((p) = (e), 0))

#define ZeroPushStruct(Arena, type)                                            \
 (type *)MemoryZeroStruct(PushStruct(Arena, type))
#define ZeroPushArray(Arena, count, type)                                      \
 (type *)MemoryZeroTyped(PushArray(Arena, count, type), count)

#define DeferLoop(begin, end)                                                  \
 for (int _i_ = ((begin), 0); !_i_; _i_ += 1, (end))
#define DeferLoopChecked(begin, end)                                           \
 for (int _i_ = 2 * !(begin); (_i_ == 2 ? ((end), 0) : !_i_); _i_ += 1, (end))

//- rjf: linked list macro helpers
#define CheckNil(nil, p) ((p) == 0 || (p) == nil)
#define SetNil(nil, p) ((p) = nil)

#define DLLInsert_NPZ(nil, f, l, p, n, next, prev)                             \
 (CheckNil(nil, f)                                                             \
      ? ((f) = (l) = (n), SetNil(nil, (n)->next), SetNil(nil, (n)->prev))      \
  : CheckNil(nil, p)                                                           \
      ? ((n)->next = (f), (f)->prev = (n), (f) = (n), SetNil(nil, (n)->prev))  \
  : ((p) == (l))                                                               \
      ? ((l)->next = (n), (n)->prev = (l), (l) = (n), SetNil(nil, (n)->next))  \
      : (((!CheckNil(nil, p) && CheckNil(nil, (p)->next))                      \
              ? (0)                                                            \
              : ((p)->next->prev = (n))),                                      \
         ((n)->next = (p)->next),                                              \
         ((p)->next = (n)),                                                    \
         ((n)->prev = (p))))
#define DLLPushBack_NPZ(nil, f, l, n, next, prev)                              \
 DLLInsert_NPZ(nil, f, l, l, n, next, prev)
#define DLLPushFront_NPZ(nil, f, l, n, next, prev)                             \
 DLLInsert_NPZ(nil, l, f, f, n, prev, next)
#define DLLRemove_NPZ(nil, f, l, n, next, prev)                                \
 (((n) == (f) ? (f) = (n)->next : (0)),                                        \
  ((n) == (l) ? (l) = (l)->prev : (0)),                                        \
  (CheckNil(nil, (n)->prev) ? (0) : ((n)->prev->next = (n)->next)),            \
  (CheckNil(nil, (n)->next) ? (0) : ((n)->next->prev = (n)->prev)))

#define SLLQueuePush_NZ(nil, f, l, n, next)                                    \
 (CheckNil(nil, f) ? ((f) = (l) = (n), SetNil(nil, (n)->next))                 \
                   : ((l)->next = (n), (l) = (n), SetNil(nil, (n)->next)))
#define SLLQueuePushFront_NZ(nil, f, l, n, next)                               \
 (CheckNil(nil, f) ? ((f) = (l) = (n), SetNil(nil, (n)->next))                 \
                   : ((n)->next = (f), (f) = (n)))
#define SLLQueuePop_NZ(nil, f, l, next)                                        \
 ((f) == (l) ? (SetNil(nil, f), SetNil(nil, l)) : ((f) = (f)->next))

#define SLLStackPush_N(f, n, next) ((n)->next = (f), (f) = (n))
#define SLLStackPop_N(f, next) ((f) = (f)->next)

#define DLLInsert_NP(f, l, p, n, next, prev)                                   \
 DLLInsert_NPZ(0, f, l, p, n, next, prev)
#define DLLPushBack_NP(f, l, n, next, prev)                                    \
 DLLPushBack_NPZ(0, f, l, n, next, prev)
#define DLLPushFront_NP(f, l, n, next, prev)                                   \
 DLLPushFront_NPZ(0, f, l, n, next, prev)
#define DLLRemove_NP(f, l, n, next, prev) DLLRemove_NPZ(0, f, l, n, next, prev)
#define DLLInsert(f, l, p, n) DLLInsert_NPZ(0, f, l, p, n, next, prev)
#define DLLPushBack(f, l, n) DLLPushBack_NPZ(0, f, l, n, next, prev)
#define DLLPushFront(f, l, n) DLLPushFront_NPZ(0, f, l, n, next, prev)
#define DLLRemove(f, l, n) DLLRemove_NPZ(0, f, l, n, next, prev)

#define SLLQueuePush_N(f, l, n, next) SLLQueuePush_NZ(0, f, l, n, next)
#define SLLQueuePushFront_N(f, l, n, next)                                     \
 SLLQueuePushFront_NZ(0, f, l, n, next)
#define SLLQueuePop_N(f, l, next) SLLQueuePop_NZ(0, f, l, next)
#define SLLQueuePush(f, l, n) SLLQueuePush_NZ(0, f, l, n, next)
#define SLLQueuePushFront(f, l, n) SLLQueuePushFront_NZ(0, f, l, n, next)
#define SLLQueuePop(f, l) SLLQueuePop_NZ(0, f, l, next)

#define SLLStackPush(f, n) SLLStackPush_N(f, n, next)
#define SLLStackPop(f) SLLStackPop_N(f, next)
