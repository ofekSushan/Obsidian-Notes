#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_CloseCover.generated.h"

/**
 * * IMPORTANT you need blackboard with Vector(LastSeenLocation),Actor(Hostile).
 * Close cover task node.
 * A task node that checks if you are close to the cover.
 */
UCLASS()
class VRPROTOTYPE_API UEnvQueryTest_CloseCover : public UEnvQueryTest
{
public:
	inline static float CoverDistanceThreshold = 120.0f;

	// AI can go inside the cover, which can cause issues when using move to in behavior tree.
	inline static float CoverDistanceToClose = 30.0f;

protected:
	GENERATED_BODY()

	explicit UEnvQueryTest_CloseCover(const FObjectInitializer& ObjectInitializer);

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;

	virtual FText GetDescriptionDetails() const override;

private:
	FBlackboardKeySelector LastSeenLocationBlackboard;
	FBlackboardKeySelector HostileBlackboard;
};
