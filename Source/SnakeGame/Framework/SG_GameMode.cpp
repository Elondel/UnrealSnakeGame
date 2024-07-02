// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#include "Framework/SG_GameMode.h"
#include "SnakeGame/Core/Types.h"
#include "SnakeGame/Core/Grid.h"
#include "World/SG_Grid.h"
#include "Framework/SG_Pawn.h"

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    // init core game
    const Snake::Settings GS{gridDims.X, gridDims.Y};
    CoreGame = MakeUnique<Snake::Game>(GS);
    check(CoreGame.IsValid());

    // init world grid
    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
    check(GridVisual);
    GridVisual->SetModel(CoreGame->grid(), CellSize);
    GridVisual->FinishSpawning(GridOrigin);

    // set pawn location fitting grid in viewport
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    check(PC);

    ASG_Pawn* Pawn = Cast<ASG_Pawn>(PC->GetPawn());
    check(Pawn);
    check(CoreGame->grid().IsValid());
    Pawn->UpdateLocation(CoreGame->grid()->dim(), CellSize, GridOrigin);
}
