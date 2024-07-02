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

    void UpdateLocation(const Snake::Dim& InDim, uint32 InCellSize, const FTransform& InGridOrigin);

protected:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Origin;
    
    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;

private:
    Snake::Dim Dim;
    uint32 CellSize;
    FTransform GridOrigin;

    void OnViewportResized(FViewport* Viewport, uint32 Val);

};
