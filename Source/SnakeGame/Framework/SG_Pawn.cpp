// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#include "Framework/SG_Pawn.h"
#include "Camera/CameraComponent.h"

namespace
{
double HalfFOVTan(double FOVDegrees)
{
    return FMath::Tan(FMath::DegreesToRadians(FOVDegrees * 0.5));
}

double VerticalFOV(double HFOVDegrees, double ViewportAspectHW)
{
    return FMath::RadiansToDegrees(2.0 * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HFOVDegrees) * 0.5) * ViewportAspectHW));
}

constexpr double GridMargin{2.0};

}  // namespace

ASG_Pawn::ASG_Pawn()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    check(Camera);
    Camera->SetupAttachment(Origin);
    Camera->AddRelativeRotation(FRotator(-90.0, 0.0, 0.0));
}

void ASG_Pawn::UpdateLocation(const SnakeGame::Dim& InDim, uint32 InCellSize, const FTransform& InGridOrigin)
{
    Dim = InDim;
    CellSize = InCellSize;
    GridOrigin = InGridOrigin;

    check(GEngine);
    check(GEngine->GameViewport);
    check(GEngine->GameViewport->Viewport);

    FViewport* Viewport = GEngine->GameViewport->Viewport;
    Viewport->ViewportResizedEvent.Remove(ResizeHandle);
    ResizeHandle = Viewport->ViewportResizedEvent.AddUObject(this, &ASG_Pawn::OnViewportResized);

#if WITH_EDITOR
    OnViewportResized(Viewport, 0);
#endif
}

void ASG_Pawn::OnViewportResized(FViewport* Viewport, uint32 Val)
{
    if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.height == 0)
    {
        return;
    }

    const double WorldWidth = Dim.width * CellSize;
    const double WorldHeight = Dim.height * CellSize;
    const double ViewportAspect = static_cast<double>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const double GridAspect = static_cast<double>(Dim.width) / Dim.height;
    const double HFOV = Camera->FieldOfView;

    double LocationZ{0.0};

    if (ViewportAspect <= GridAspect)
    {
        const double MarginWidth = (Dim.width + GridMargin) * CellSize;
        LocationZ = MarginWidth / HalfFOVTan(HFOV);
    }
    else
    {
        check(ViewportAspect);
        const double VFOV = VerticalFOV(HFOV, 1.0 / ViewportAspect);
        const double MarginWidth = (Dim.height + GridMargin) * CellSize;
        LocationZ = MarginWidth / HalfFOVTan(VFOV);
    }

    const FVector NewPawnLocation = GridOrigin.GetLocation() + 0.5 * FVector(WorldHeight, WorldWidth, LocationZ);
    SetActorLocation(NewPawnLocation);
}
