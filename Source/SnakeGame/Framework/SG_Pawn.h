// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/Types.h"
#include "SG_Pawn.generated.h"

class UCameraComponent;

UCLASS()
class SNAKEGAME_API ASG_Pawn : public APawn
{
    GENERATED_BODY()

public:
    ASG_Pawn();

    /**
     * Update pawn height fitting grid in viewport
     * @param InDim  grid dimensions
     * @param InCellSize  world size of cell
     * @param InGridOigin  world trasformation of the grid
     */
    void UpdateLocation(const Snake::Dim& Dim, uint32 CellSize, const FTransform& GridOrigin);

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;

private:
    Snake::Dim Dim;
    uint32 CellSize;
    FTransform GridOrigin;
    FDelegateHandle ResizeHandle;

    void OnViewportResized(FViewport* Viewport, uint32 Val);
};
