// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#include "Framework/SG_GameMode.h"
#include "SnakeGame/Core/Types.h"
#include "SnakeGame/Core/Grid.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "World/SG_WorldTypes.h"
#include "Framework/SG_Pawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ASG_GameMode::ASG_GameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    // init core game
    CoreGame = MakeUnique<SnakeGame::Game>(MakeSettings());
    check(CoreGame.IsValid());

    // init world grid
    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
    check(GridVisual);
    GridVisual->SetModel(CoreGame->grid(), CellSize);
    GridVisual->FinishSpawning(GridOrigin);

    // init world snake
    SnakeVisual = GetWorld()->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, GridOrigin);
    SnakeVisual->SetModel(CoreGame->snake(), CellSize, CoreGame->grid()->dim());
    SnakeVisual->FinishSpawning(GridOrigin);

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

    //
    SetupInput();
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

        // update snake
        SnakeVisual->UpdateColors(*ColorSet);

        // update scene ambient color via fog
        if (Fog && Fog->GetComponent())
        {
            Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
            Fog->MarkComponentsRenderStateDirty();
        }
    }
}

void ASG_GameMode::SetupInput()
{
    if (!GetWorld())
    {
        return;
    }

    if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(InputMapping, 0);
        }

        UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PC->InputComponent);
        check(Input);
        Input->BindAction(MoveFowrwardInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveForward);
        Input->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveRight);
        Input->BindAction(ResetGameInputAction, ETriggerEvent::Started, this, &ThisClass::OnGameReset);
    }
}

void ASG_GameMode::OnMoveForward(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();

    if (InputValue == 0.0f)
    {
        return;
    }

    SnakeInput = SnakeGame::Input(0, InputValue);
}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();

    if (InputValue == 0.0f)
    {
        return;
    }

    SnakeInput = SnakeGame::Input(InputValue, 0);
}

void ASG_GameMode::OnGameReset(const FInputActionValue& Value)
{
    const bool InputValue = Value.Get<bool>();
    if (InputValue)
    {
        CoreGame.Reset(new SnakeGame::Game(MakeSettings()));
        check(CoreGame.IsValid());
        GridVisual->SetModel(CoreGame->grid(), CellSize);
        SnakeVisual->SetModel(CoreGame->snake(), CellSize, CoreGame->grid()->dim());
        SnakeInput = SnakeGame::Input{1, 0};
        NextColor();
    }
}

void ASG_GameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (CoreGame.IsValid())
    {
        CoreGame->update(DeltaSeconds, SnakeInput);
    }
}

SnakeGame::Settings ASG_GameMode::MakeSettings() const
{
    SnakeGame::Settings GS;
    GS.gridDims = SnakeGame::Dim{gridDims.X, gridDims.Y};
    GS.gameSpeed = GameSpeed;
    GS.snake.defaultSize = SnakeDefaultSize;
    GS.snake.startPosition = SnakeGame::Position{gridDims.X / 2 + 1, gridDims.Y / 2 + 1};

    return GS;
}