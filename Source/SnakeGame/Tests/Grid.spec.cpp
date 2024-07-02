// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Grid.h"

DEFINE_SPEC(FSnakeGrid, "Snake", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FSnakeGrid::Define()
{
	using namespace Snake;

	Describe("CoreGrid", [this]() {  //
        It("DimsMightIncludeWalls",
            [this]()
            {
                const Grid grid(Dim{20, 10});
                TestTrueExpr(grid.dim().height == 12);
                TestTrueExpr(grid.dim().width == 22);
            });
    });
}

#endif
