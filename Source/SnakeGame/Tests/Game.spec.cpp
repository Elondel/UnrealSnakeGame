// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Core/Game.h"

DEFINE_SPEC(FSnakeGame, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FSnakeGame::Define()
{
    using namespace SnakeGame;

    Describe("CoreGame", [this]() {  //
        It("GridShouldExist",
            [this]()
            {
                const Settings GS{10, 10};
                const auto CoreGame = Game(GS);
                TestTrueExpr(CoreGame.grid().IsValid());
            });
    });
}

#endif
