#include "AI/EnvQueryTests/EnvQueryTest_CloseCover.h"
#include "AIController.h"
#include "VRFirstPerson.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Math/Vector.h"


UEnvQueryTest_CloseCover::UEnvQueryTest_CloseCover(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer)
{
	LastSeenLocationBlackboard.SelectedKeyName = "LastSeenLocation";
	HostileBlackboard.SelectedKeyName = "Hostile";
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

FText UEnvQueryTest_CloseCover::GetDescriptionTitle() const
{
	return FText::FromString(FString(TEXT("Close Cover")));
}

FText UEnvQueryTest_CloseCover::GetDescriptionDetails() const
{
	return FText::FromString(
		FString(TEXT("IMPORTANT you need blackboard with Vector(LastSeenLocation),Actor(Hostile).")));
}

void UEnvQueryTest_CloseCover::RunTest(FEnvQueryInstance& QueryInstance) const
{
	if (ACharacter* Character = Cast<ACharacter>(QueryInstance.Owner.Get()))
	{
		if (AAIController* AIController = Cast<AAIController>(Character->GetController()))
		{
			FCollisionQueryParams CollisionParams(FName(TEXT("PlayerVisibilityTest")), true, Character);
			if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
			{
				FVector PlayerLocation;
				if (AActor* Hostile = Cast<AActor>(Blackboard->GetValueAsObject(HostileBlackboard.SelectedKeyName)))
				{
					PlayerLocation = Hostile->GetActorLocation();
				}
				else
				{
					PlayerLocation = Blackboard->GetValueAsVector(
						LastSeenLocationBlackboard.SelectedKeyName);
				}
				FHitResult HitResult;

				for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
				{
					bool IsCover = false;
					const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
					GetWorld()->LineTraceSingleByChannel(HitResult, ItemLocation,
					                                     PlayerLocation,
					                                     ECC_Visibility, CollisionParams);

					if (HitResult.bBlockingHit)
					{
						float Distance = FVector::Distance(HitResult.Location, ItemLocation);
						if (Distance < CoverDistanceThreshold && Distance > CoverDistanceToClose)
						{
							IsCover = true;
						}
					}

					if (IsCover)
					{
						DrawDebugLine(GetWorld(), ItemLocation, HitResult.Location, FColor::Green, false,
						              20.f);
					}
					else
					{
						DrawDebugLine(GetWorld(), ItemLocation, HitResult.Location, FColor::Red, false, 20.f);
					}
					
					It.SetScore(TestPurpose, FilterType, IsCover, true);
				}
			}
		}
	}
}
