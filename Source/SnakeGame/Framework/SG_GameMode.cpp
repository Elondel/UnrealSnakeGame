// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#include "Framework/SG_GameMode.h"
#include "SnakeGame/Core/Types.h"
#include "SnakeGame/Core/Grid.h"
#include "World/SG_Grid.h"
#include "World/SG_WorldTypes.h"
#include "Framework/SG_Pawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    // init core game
    const SnakeGame::Settings GS{gridDims.X, gridDims.Y};
    CoreGame = MakeUnique<SnakeGame::Game>(GS);
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

    //
    FindFog();

    // update colors
    check(ColorsTable);
    const int32 RowsCount = ColorsTable->GetRowNames().Num();
    check(RowsCount > 0);
    ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
    UpdateColors();
}

void ASG_GameMode::NextColor()
{
    if (ColorsTable)
    {
        ColorTableIndex = (ColorTableIndex + 1) % ColorsTable->GetRowNames().Num();
        UpdateColors();
    }
}

void ASG_GameMode::FindFog()
{
    TArray<AActor*> Fogs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), Fogs);

    check(Fogs.Num() == 1);

    Fog = Cast<AExponentialHeightFog>(Fogs[0]);
}

void ASG_GameMode::UpdateColors()
{
    const auto RowName = ColorsTable->GetRowNames()[ColorTableIndex];
    const FSnakeColors* ColorSet = ColorsTable->FindRow<FSnakeColors>(RowName, {});

    if (ColorSet)
    {
        // update grid
        GridVisual->UpdateColors(*ColorSet);

        // update scene ambient color via fog
        if (Fog && Fog->GetComponent())
        {
            Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
            Fog->MarkComponentsRenderStateDirty();
        }
    }
}
