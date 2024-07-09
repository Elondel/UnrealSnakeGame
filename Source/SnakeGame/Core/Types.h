// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"

namespace SnakeGame
{

using Position = FUint32Point;
using TPositionPointer = TDoubleLinkedList<Position>::TDoubleLinkedListNode;

struct Dim
{
    uint32 width;
    uint32 height;
};

struct Input
{
    int8 x;
    int8 y;

    FORCEINLINE bool opposite(const Input& rhs) const { return (x != 0 && x == -rhs.x) || (y != 0 && y == -rhs.y); }
};

enum class CellType
{
    Empty = 0,
    Wall,
    Snake
    // Food
};

struct Settings
{
    Dim gridDims{30, 10};
    struct Snake
    {
        uint32 defaultSize{4};
        Position startPosition{0, 0};
    } snake;
    float gameSpeed{1.0f};
};

class TSnakeList : public TDoubleLinkedList<Position>
{
public:
    void MoveTail(TPositionPointer* Tail, TPositionPointer* Head, const Position& Position)
    {
        RemoveNode(Tail);
        InsertNode(Position, Head);
    }
};

}  // namespace SnakeGame