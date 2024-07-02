// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace Snake
{

class Grid
{
public:
    Grid(const Dim& dim);

    Dim dim() const { return c_dim; }

private:
    const Dim c_dim;
    TArray<CellType> m_cells;

    void initWalls();
    void printDebug() const;
    FORCEINLINE uint32 posToIndex(uint32 x, uint32 y) const;
};

}  // namespace SnakeGame
