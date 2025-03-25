#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_IsSuitableCover.generated.h"

/*
 * IMPORTANT you need blackboard with Vector(LastSeenLocation),Actor(Hostile).
 * IsSuitable environment query test.
 * A environment query test that checks what type of cover and if is suitable for the size.
 */
UCLASS()
class VRPROTOTYPE_API UEnvQueryTest_IsSuitableCover : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	static FVector FindPlayerLocation(const UBlackboardComponent* Blackboard, const FName HostileBlackboardName,
	                                  const FName LastSeenLocationBlackboardName, const ACharacter* Character,
	                                  const FName BoneToSearchFor = "head");

protected:
	explicit UEnvQueryTest_IsSuitableCover(const FObjectInitializer& ObjectInitializer);

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;

private:
	FName BoneToSearchFor = TEXT("head");

	FBlackboardKeySelector LastSeenLocationBlackboard;
	FBlackboardKeySelector HostileBlackboard;

	float CoverDistanceThreshold;
};
